#include "server.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "crypto.h"
#include "socketio.h"

/* Accept a connection on the specified socket and return the new fd */
int acceptConnection(int fd) {
    int result;
    result = accept(fd, NULL, NULL);
    if (result < 0)
        perror("accept");

    printf("Accepted connection, new fd %d\n", fd);
    return result;
}

/* Wait on any terminated child processes */
void reapChildren(int sig) {
    int oldErrno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        continue;
    }
    errno = oldErrno;
}

/* Set signal handler for cleaning up zombies */
void registerChildHandler() {
    struct sigaction sigact;
    sigact.sa_handler = reapChildren;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_RESTART;   /* Restart system calls */
    if (sigaction(SIGCHLD, &sigact, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

unsigned short getRandPort() {
	return (unsigned short)(rand() % (USHRT_MAX - 2000) + 2000);
}

void handleRequest(const char *prog, int fd, const char *type) {
    char *msg = NULL;       /* stores the message data */
    char *key = NULL;       /* stores the key data */
    char buf[BUFFER_SIZE];  /* stores handshake data */
    int listenfd, newfd;    /* stores the socket fds */
    char *tmp, *str;        /* for parsing handshake data with strtok */
    size_t keylen, msglen;  /* stores the key and message sizes */
    enum Result res;        /* stores result of en/decryption operation */
    
    /* Validate that client is correct one */
    receiveAny(fd, buf, BUFFER_SIZE);
    str = strtok_r(buf, " \n\r", &tmp);
    if (strcmp(str, type) != 0) {
        fprintf(stderr, "%s error: invalid request type '%s' from client\n",
            prog, str);
        sendAll(fd, "INVALID");
        exit(EXIT_FAILURE);
    }
    
    /* get key and message sizes */
    str = strtok_r(NULL, " \n\r", &tmp);
    keylen = atoi(str);
    str = strtok_r(NULL, " \n\r", &tmp);
    msglen = atoi(str);
    
    /* Tell client which port to connect to */
    snprintf(buf, BUFFER_SIZE, "%hu", getRandPort());
    sendAll(fd, buf);
    
    /* Listen for the client on that port */
    listenfd = listenPort(buf);
    
    /* Accept the connection on the new port */
    newfd = acceptConnection(listenfd);
    
    /* We no longer need the listen fd */
    close(listenfd);
    
    /* Allocate memory for the key and buffer data */
    /* Include 1 extra byte for NULL terminator */
    key = malloc(keylen + 1);
    if (key == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    msg = malloc(msglen + 1);
    if (msg == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    /* Tell client ready for key data */
    sendAll(newfd, "KEY");
    
    /* Get the key data, followed by the message data */
    receiveAll(newfd, key, keylen);
    sendAll(newfd, "MSG");
    receiveAll(newfd, msg, msglen);
    
    /* Encrypt or decrypt message */
    if (strcmp(type, "ENCRYPT") == 0) {
        res = encryptText(key, msg);
    } else if (strcmp(type, "DECRYPT") == 0) {
        res = decryptText(key, msg);
    } else {
        fprintf(stderr, "%s error: invalid request type '%s'\n", prog, type);
        exit(EXIT_FAILURE);
    }

    /* Send encrypted text back if successful;
       Otherwise determine which error occurred and display message */
    switch (res) {
    case Result_SUCCESS:
        sendAll(newfd, msg);
        break;
    case Result_KEY_ERROR:
        sendAll(newfd, "KEYERROR");
        break;
    case Result_INVALID_CHAR:
        sendAll(newfd, "INVALIDCHAR");
        break;
    }

    /* Child process is done--clean up resources and return */
    if (key != NULL) free(key);
    if (msg != NULL) free(msg);

    if (res == Result_SUCCESS)
        exit(EXIT_SUCCESS);
    else
        exit(EXIT_FAILURE);
}

int listenPort(const char *port) {
    int fd, val;
    int yes = 1;
    struct addrinfo hints, *result, *rp;

    /* Zero-initialize and set addrinfo structure */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* IPv4 or IPv6, whichever is available */
    hints.ai_socktype = SOCK_STREAM;/* TCP */
    hints.ai_flags = AI_PASSIVE;    /* Use localhost IP */

    /* Look up the localhost address info */
    val = getaddrinfo(NULL, port, &hints, &result);
    if (val != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(val));
        exit(EXIT_FAILURE);
    }

    /* Loop through address structure results until bind succeeds */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        /* Attempt to open a socket based on the localhost's address info */
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd == -1) {
            perror("server: socket");
            continue; /* Try next on error */
        }
        
        /* Attempt to reuse the socket if it's already in use */
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            perror("setsockopt");
            freeaddrinfo(result);
            exit(EXIT_FAILURE);
        }

        /* Attempt to bind the socket to the port */
        if (bind(fd, rp->ai_addr, rp->ai_addrlen) == 0) {
            break;  /* Break from loop if bind was successful */
        }
        
        /* Bind failed. Close file descriptor and try next address */
        close(fd);
        perror("server: bind");
    }

    /* Exit on error if bind failed on all returned addresses */
    if (rp == NULL) {
        freeaddrinfo(result);
        fprintf(stderr, "bind: No valid address found.\n");
        exit(EXIT_FAILURE);
    }
    
    /* Free memory used by localhost's address info */
    freeaddrinfo(result);

    /* Listen on port for up to MAX_CONNECTIONS */
    if (listen(fd, MAX_CONNECTIONS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* Return the listening socket file descriptor */
    return fd;
}

void verifyArgs(int argc, char *argv[]) {
    if (argc < 2 ||
        strcmp(argv[1], "--help") == 0 ||
        strcmp(argv[1], "-h") == 0) {
        fprintf(stderr, "usage: %s listening_port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 4 - OTP
* File:         server.c
* Description:  Implementation file for server.h
\*********************************************************/
#include "server.h"

#include <stdlib.h>     /* EXIT_*, rand, srand, malloc, free */
#include <stdio.h>      /* perror, fprintf */
#include <string.h>     /* strtok_r, strcmp */
#include <errno.h>      /* perror, errno */
#include <limits.h>     /* USHRT_MAX */
#include <signal.h>     /* sigaction */
#include <netdb.h>      /* addrinfo, getaddrinfo, gai_strerror */
#include <sys/socket.h> /* addrinfo, getaddrinfo, gai_strerror */
#include <sys/types.h>  /* ssize_t, size_t */
#include <sys/wait.h>   /* waitpid */
#include <unistd.h>     /* close */

#include "crypto.h"
#include "socketio.h"

/*========================================================*
 * Macro definitions
 *========================================================*/
/* Defines the max number of connections to queue when waiting */
#define MAX_CONNECTIONS 5

/**
 * Accepts a connection on the specified socket and returns the new fd.
 *
 * This function accepts any incoming connection on the specified socket.
 * If no connection is waiting, it blocks until one is available.
 *
 *  fd      The socket descriptor to wait for a connection on.
 *
 * Returns the new socket descriptor once the connection is established,
 * or -1 if the accept operation fails.
 */
int acceptConnection(int fd) {
    int result;
    result = accept(fd, NULL, NULL);
    if (result < 0)
        perror("accept");

    return result;
}

/**
 * Checks for any terminated child processes and waits on them.
 *
 * This function performs a non-blocking check for zombie child processes
 * and reaps any that are around at the time of calling.
 *
 *  sig     The signal that triggered the call.
 */
void reapChildren(int sig) {
    int oldErrno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        continue;
    }
    errno = oldErrno;
}

/**
 * Sets the signal handler for cleaning up zombies.
 *
 * This function sets the reapChildren function as the signal handler
 * for cleaning up any zombie child processes. The handler is invoked
 * whenever SIGCHLD occurs.
 */
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

/**
 * Generates a random port number between 2000 and the maximum allowable port.
 *
 * Returns the random port number.
 */
unsigned short getRandPort() {
    return (unsigned short)(rand() % (USHRT_MAX - 2000) + 2000);
}

/**
 * Handles a client request for encryption or decryption.
 *
 * This function performs all of the necessary operations to handle
 * a request from one of the client programs. As part of the process,
 * it verifies that the client type is valid and provides a different
 * port for the client to connect to. If any of the steps fail,
 * it exits with an error code of 1. If all steps succeed, it exits
 * with an error code of 0.
 * 
 * This function always terminates the child process.
 *
 *  prog    The name of the calling program.
 *  fd      The socket descriptor over which to handle the request.
 *  type    The type of request. Must be ENCRYPT_REQ or DECRYPT_REQ.
 */
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
        /* fprintf(stderr, "%s error: invalid request type '%s' from client\n",
            prog, str); */
        sendAll(fd, INVALID_TYPE);
        exit(EXIT_FAILURE);
    }

    /* get key and message sizes */
    str = strtok_r(NULL, " \n\r", &tmp);
    keylen = atoi(str);
    str = strtok_r(NULL, " \n\r", &tmp);
    msglen = atoi(str);

    /* Attempt to bind and listen on a random port */
    do {
        snprintf(buf, BUFFER_SIZE, "%hu", getRandPort());
        listenfd = listenPort(buf);
    } while (listenfd == -1);

    /* Tell client which port to connect to */
    sendAll(fd, buf);

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
    sendAll(newfd, KEY_REQ);

    /* Get the key data, followed by the message data */
    receiveAll(newfd, key, keylen);
    sendAll(newfd, MESSAGE_REQ);
    receiveAll(newfd, msg, msglen);

    /* Encrypt or decrypt message */
    if (strcmp(type, ENCRYPT_REQ) == 0) {
        res = encryptText(key, msg);
    } else if (strcmp(type, DECRYPT_REQ) == 0) {
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
        sendAll(newfd, KEY_ERROR);
        break;
    case Result_INVALID_CHAR:
        sendAll(newfd, INVALID_CHAR);
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

/**
 * Binds and starts listening on the specified port.
 *
 * This function binds the specified port on the localhost using
 * either IPv4 or IPv6, whichever is available. It then begins listening
 * for connections on that port.
 *
 *  port    The port to bind, as a c-style string.
 *
 * Returns the socket descriptor for the bound and listened port,
 * or -1 if unsuccessful.
 */
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
        return -1;
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
            return -1;
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
        return -1;
    }

    /* Free memory used by localhost's address info */
    freeaddrinfo(result);

    /* Listen on port for up to MAX_CONNECTIONS */
    if (listen(fd, MAX_CONNECTIONS) < 0) {
        perror("listen");
        return -1;
    }

    /* Return the listening socket file descriptor */
    return fd;
}

/**
 * Verifies that the correct number of command-line arguments were specified.
 *
 * This function makes sure the correct number of command-line arguments were
 * specified and displays help if not, or if the --help or -h argument was
 * specified. This function does not verify the validity of the argument values
 * themselves.
 *
 *  argc    The number of arguments.
 *  argv    The values of the arguments.
 */
void verifyArgs(int argc, char *argv[]) {
    if (argc < 2 ||
        strcmp(argv[1], "--help") == 0 ||
        strcmp(argv[1], "-h") == 0) {
        fprintf(stderr, "usage: %s listening_port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

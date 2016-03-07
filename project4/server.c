#include "server.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "socketio.h"

/* Accept a connection on the specified socket and return the new fd */
int acceptConnection(int fd) {
    int result;
    result = accept(fd, NULL, NULL);
    if (result < 0)
        perror("accept");

    return result;
}

/* Wait on any terminated child processes */
void childHandler(int sig) {
    int oldErrno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        continue;
    }
    errno = oldErrno;
}

/* Set signal handler for cleaning up zombies */
void registerChildHandler() {
    struct sigaction sigact;
    sigact.sa_handler = childHandler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_RESTART;   /* Restart system calls */
    if (sigaction(SIGCHLD, &sigact, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

int startListening(const char *port) {
    int fd, val;
    int yes = 1;
    struct addrinfo hints, *res;

    /* Zero-initialize and set addrinfo structure */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* IPv4 or IPv6, whichever is available */
    hints.ai_socktype = SOCK_STREAM;/* TCP */
    hints.ai_flags = AI_PASSIVE;    /* Use localhost IP */

    /* Look up the localhost address info */
    val = getaddrinfo(NULL, port, &hints, &res);
    if (val != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(val));
        exit(EXIT_FAILURE);
    }

    /* Open a socket based on the local host's address info */
    if ((fd = socket(
                        res->ai_family,
                        res->ai_socktype,
                        res->ai_protocol
                    )) < 0) {
        perror("server: socket");
    }

    /* Attempt to reuse the socket if it's already in use */
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    /* Bind the socket to the port */
    if (bind(fd, res->ai_addr, res->ai_addrlen) < 0) {
        close(fd);
        perror("server: bind");
        exit(EXIT_FAILURE);
    }

    /* Free memory used by localhost's address info */
    freeaddrinfo(res);

    /* Listen on port for up to MAX_CONNECTIONS */
    if (listen(fd, MAX_CONNECTIONS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* Return the listening socket file descriptor */
    return fd;
}

int validateClient(int fd, char *expected) {
    int result = 1;
    char *buf;
    
    /* Get client request type */
    if (receiveAll(fd, &buf) < 0) {
        result = 0;
    }

    /* Abort connection if invalid request */
    if (result && strcmp(buf, expected) != 0) {
        /* Send error message to client and disconnect */
        sendAll(fd, "INVALID");
        result = 0;
    }

    /* Free buffer */
    if (buf != NULL) free(buf);

    return result;
}


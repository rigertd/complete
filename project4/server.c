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
#include <unistd.h>

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


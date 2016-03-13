#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "socketio.h"
#include "server.h"

int main(int argc, char *argv[]) {
    /* Verify command line arguments */
    verifyArgs(argc, argv);

    int listen_fd, new_fd;

    /* Seed random number generator */
    srand(time(0));

    /* Configure, bind, and start listening on the listening socket */
    listen_fd = listenPort(argv[1]);

    /* Set up signal handler to reap child processes */
    registerChildHandler();

    /* Stay in accept loop until terminated by a KILL or STOP signal */
    while(1) {
        /* Block until an incoming connection arrives */
        new_fd = acceptConnection(listen_fd);

        // If a connection fails, just continue
        if (new_fd < 0) {
            continue;
        }

        /* Spawn connection in a new child process. */
        switch (fork()) {
        case -1:
            /* Fork error--cancel connection */
            perror("fork");
            continue;
        case 0:
            /* Child process doesn't need listen socket descriptor */
            close(listen_fd);
            handleRequest(argv[0], new_fd, "ENCRYPT");
            break;
        default:
            /* Parent process doesn't need new socket descriptor */
            close(new_fd);
            break;
        }
    }

    return EXIT_SUCCESS;
}

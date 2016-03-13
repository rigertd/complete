/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 4 - OTP
* File:         otp_dec_d.c
* Description:  A one-time-pad decryption daemon written in C.
*
*               This program accepts TCP connections from decryption clients
*               and decrypts encrypted data using the provided encryption
*               key. It then sends the decrypted data back to the client.
*
*               The command line syntax is as follows:
*
*                   otp_dec_d port
*
*               This program takes the following arguments:
*               - port      -- The TCP port on which to wait for client
*                              connections.
\*********************************************************/
#include <stdlib.h> /* srand */
#include <stdio.h>  /* perror */
#include <unistd.h> /* fork */
#include <errno.h>  /* perror */
#include <time.h>   /* time */

#include "server.h"
#include "socketio.h"   /* DECRYPT_REQ */

/*========================================================*
 * main function
 *========================================================*/
int main(int argc, char *argv[]) {
    /* Verify command line arguments */
    verifyArgs(argc, argv);

    int listen_fd, new_fd;

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
            /* Seed random number generator here based on time OR pid */
            /* Otherwise, every child process will inherit the same sequence */
            srand(time(0) ^ getpid());
            handleRequest(argv[0], new_fd, DECRYPT_REQ);
            break;
        default:
            /* Parent process doesn't need new socket descriptor */
            close(new_fd);
            break;
        }
    }

    return EXIT_SUCCESS;
}

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <sys/wait.h>

#include "crypto.h"
#include "globals.h"

#define MAX_CONNECTIONS 5

/* Wait on any terminated child processes */
void childHandler(int sig) {
    int oldErrno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        continue;
    }
    errno = oldErrno;
}

/* Set signal handler for cleaning up zombies */
void setupChildHandler() {
    struct sigaction sigact;
    sigact.sa_handler = childHandler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_RESTART;   /* Restart system calls */
    if (sigaction(SIGCHLD, &sigact, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

int setupListenSocket(char *port) {
    int fd, val;
    int yes = 1;
    struct addrinfo hints, *res;
    struct sockaddr_storage client;
    socklen_t sin_size;
    
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

    /* Free memory used by local host's address info */
    freeaddrinfo(res);

    /* Return the listening socket file descriptor */
    return fd;
}

int receiveAll(int fd, char **buf) {
    int bytes, total = 0;
    char buffer[BUFFER_SIZE];
    char *tmp = NULL;
    
    /* Make sure buf is not already allocated */
    if (*buf != NULL) {
        free(*buf);
        *buf = NULL;
    }
    
    /* Receive until there is nothing left to receive */
    while ((bytes = recv(fd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        printf("received %d bytes, %d total: '%s'\n", bytes, bytes + total, buffer);
        tmp = malloc(total + bytes + 1);
        if (tmp == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        
        /* Copy old data (if any) to new string */
        if (total > 0) {
            strncpy(tmp, *buf, total);
        }
        
        /* Append new data to end of new string */
        strncpy(&tmp[total], buffer, bytes);
        
        /* Update total */
        total += bytes;
        
        /* Add null terminator */
        tmp[total] = '\0';
        
        /* Free old buffer */
        free(*buf);
        
        /* Update buffer pointer to new string */
        *buf = tmp;
        tmp = NULL;
    }
    
    /* Return -1 if receive error, or total bytes received otherwise */
    if (bytes == -1) return -1;
    else return total;
}

void spawnChildProcess(int listen_fd, int remote_fd) {
    int ret_val, bytes_rcvd;
    char *msg = NULL;
    char *key = NULL;
    
    ret_val = fork();
    if (ret_val < 0) {
        /* Fork failed */
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (ret_val == 0) {
        /* Child process doesn't need listen socket */
        close(listen_fd);
        /* Get client request type */
        bytes_rcvd = receiveAll(remote_fd, &msg);
        if (bytes_rcvd < 0) {
            perror("recv");
            if (msg != NULL) free(msg);
            close(remote_fd);
            exit(EXIT_FAILURE);
        }
        
        /* Abort connection if invalid request */
        if (strncmp(msg, "ENCRYPT", bytes_rcvd) != 0) {
            fprintf(stderr, "otp_enc_d: Invalid request type\n");
            if (msg != NULL) free(msg);
            close(remote_fd);
            exit(EXIT_FAILURE);
        }
        
        /* Free msg buffer */
        if (msg != NULL) free(msg);
        
        /* Request key data */
        if (send(remote_fd, "KEY", 3, 0) < 0) {
            perror("send");
            close(remote_fd);
            exit(EXIT_FAILURE);
        }
        
        /* Receive key data */
        bytes_rcvd = receiveAll(remote_fd, &key);
        if (bytes_rcvd < 0) {
            perror("recv");
            if (key != NULL) free(key);
            close(remote_fd);
            exit(EXIT_FAILURE);
        }
        
        /* Request plaintext message data */
        if (send(remote_fd, "MSG", 3, 0) < 0) {
            perror("send");
            close(remote_fd);
            exit(EXIT_FAILURE);
        }
        
        /* Receive plaintext message data */
        bytes_rcvd = receiveAll(remote_fd, &msg);
        if (bytes_rcvd < 0) {
            perror("recv");
            if (key != NULL) free(key);
            if (msg != NULL) free(msg);
            close(remote_fd);
            exit(EXIT_FAILURE);
        }
        
        /* Encrypt plaintext message */
        enum Result res = encryptText(key, msg);
        
        /* Send encrypted text back if successful */
        if (res == Result_SUCCESS) {
            if (send(remote_fd, msg, bytes_rcvd, 0) < 0) {
                perror("send");
                close(remote_fd);
                exit(EXIT_FAILURE);
            }
            
            /* Child process is done--clean up resources and exit */
            if (key != NULL) free(key);
            if (msg != NULL) free(msg);
            close(remote_fd);
            exit(EXIT_SUCCESS);
        }
        
        /* Determine which error occurred and display error message */
        switch (res) {
        case Result_KEY_ERROR:
            fprintf(stderr, "otp_enc_d error: key '%s' is too short\n", key);
            break;
        case Result_INVALID_CHAR:
            fprintf(stderr, "otp_enc_d error: input contains bad characters\n");
            break;
        }
        
        /* Clean up resources and exit with error */
        if (key != NULL) free(key);
        if (msg != NULL) free(msg);
        close(remote_fd);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    int listen_fd, new_fd;
    int ret_val;
    struct sockaddr_storage client;
    socklen_t sin_size;
    //char s[INET6_ADDRSTRLEN];

    /* Verify command line arguments */
    if (argc < 2 ||
        strcmp(argv[1], "--help") == 0 ||
        strcmp(argv[1], "-h") == 0) {
        fprintf(stderr, "usage: %s listening_port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Configure and bind the listening socket */
    listen_fd = setupListenSocket(argv[1]);

    /* Listen on port for up to MAX_CONNECTIONS */
    if (listen(listen_fd, MAX_CONNECTIONS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* Set up signal handler to reap child processes */
    setupChildHandler();
    
    /* Stay in accept loop until terminated by a KILL or STOP signal */
    while(1) {
        sin_size = sizeof(client);
        new_fd = accept(listen_fd, (struct sockaddr *)&client, &sin_size);

        // If a connection fails, just print an error and continue
        if (new_fd < 0) {
            perror("accept");
            continue;
        }

        /* Spawn connection in a new child process.
           Only the parent process returns. */
        spawnChildProcess(listen_fd, new_fd);

        /* Parent process resumes here */
        close(new_fd);
    }

    return 0;
}

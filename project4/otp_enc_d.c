#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#include "crypto.h"
#include "socketio.h"
#include "server.h"

unsigned short getRandPort() {
	return (unsigned short)(rand() % (USHRT_MAX - 2000) + 2000);
}

int handleClient(int fd) {
    char *msg = NULL;      /* stores the message data */
    char *key = NULL;      /* stores the key data */
	char buf[BUFFER_SIZE]; /* stores handshake data */
	int listenfd, newfd;   /* stores the socket fds */
	char *tmp, *str;
	size_t keylen, msglen;    /* stores the key and message sizes */
    
    /* Validate that client is correct one */
	receive(fd, buf, BUFFER_SIZE);
	str = strtok_r(buf, " ", &tmp);
    if (strcmp(str, "ENCRYPT") != 0) {
        fprintf(stderr, "otp_enc_d: Invalid request from client\n");
		exit(EXIT_FAILURE);
    }
	
	/* get key and message sizes */
	str = strtok_r(NULL, " ", &tmp);
	keylen = atoi(str);
	str = strtok_r(NULL, " ", &tmp);
	msglen = atoi(str);
    
	/* Tell client which port to connect to */
	snprintf(buf, BUFFER_SIZE, "%hu", getRandPort());
	sendAll(fd, buf, strlen(buf));
	
	/* Listen for the client on that port */
	listenfd = listenPort(buf);
	
	/* Accept the connection on the new port */
	newfd = acceptConnection(listenfd);
	
	/* We no longer need the listen fd */
	close(listenfd);
	
	/* Allocate memory for the key and buffer data */
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
	
	/* Get the key data, followed by the message data */
	receiveAll(newfd, key, keylen + 1);
	receiveAll(newfd, msg, msglen + 1);
	
    /* Encrypt plaintext message */
    enum Result res = encryptText(key, msg);

    /* Send encrypted text back if successful;
       Otherwise determine which error occurred and display message */
    switch (res) {
    case Result_SUCCESS:
        sendAll(fd, msg, msglen);
        break;
    case Result_KEY_ERROR:
        fprintf(stderr, "otp_enc_d error: key '%s' is too short\n", key);
        break;
    case Result_INVALID_CHAR:
        fprintf(stderr, "otp_enc_d error: input contains bad characters\n");
        break;
    }

    /* Child process is done--clean up resources and return */
    if (key != NULL) free(key);
    if (msg != NULL) free(msg);

    if (res == Result_SUCCESS)
        return 1;
    else
        return 0;
}

void spawnChildProcess(int listen_fd, int remote_fd) {
    int retval;
    retval = fork();
    if (retval < 0) {
        /* Fork failed */
        perror("fork");
    } else if (retval == 0) {
        /* Child process doesn't need listen socket */
        close(listen_fd);
        
        retval = handleClient(remote_fd);
        
        close(remote_fd);
        
        if (retval) {
            exit(EXIT_SUCCESS);
        }
        else {
            fprintf(stderr, "otp_enc_d error: Encrypt request failed\n");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[]) {
    int listen_fd, new_fd;

    /* Verify command line arguments */
    if (argc < 2 ||
        strcmp(argv[1], "--help") == 0 ||
        strcmp(argv[1], "-h") == 0) {
        fprintf(stderr, "usage: %s listening_port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
	
	/* Seed random number generator */
	srand(time(0));

    /* Configure, bind, and start listening on the listening socket */
    listen_fd = startListening(argv[1]);

    /* Set up signal handler to reap child processes */
    registerChildHandler();

    /* Stay in accept loop until terminated by a KILL or STOP signal */
    while(1) {
        /* Block until an incoming connection arrives */
        new_fd = acceptConnection(listen_fd);

        // If a connection fails, just continue
        if (new_fd < 0) {
            printf("Connected to client\n");
            continue;
        }

        /* Spawn connection in a new child process.
           Only the parent process returns. */
        spawnChildProcess(listen_fd, new_fd);

        /* Parent process does not need the new socket */
        close(new_fd);
    }

    return EXIT_SUCCESS;
}

#include "socketio.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#define BUFFER_SIZE 512

ssize_t receive(int fd, char *buf, size_t len) {
    int bytes;

    bytes = recv(fd, &buf[running], len, 0);

    /* Return -1 if receive error, or total bytes received otherwise */
    if (bytes == -1) {
        perror("recv");
        return -1;
    }
    else {
		/* Null-terminate buffer */
		buf[bytes] = '\0';
		return bytes;
	}
}

ssize_t receiveAll(int fd, char *buf, size_t len) {
    int bytes, running = 0;

    while (running < len - 1) {
        /* Block until all data is received */
        bytes = recv(fd, &buffer[running], len, MSG_WAITALL);
        
        /* Stop if an error other than a signal interrupt occurred,
           or if socket was closed */
        if ((bytes == -1 && errno != EINTR) || bytes == 0) {
            break;
        }
        running += bytes;
        
        printf("received %d bytes, %d of %d total\n", bytes, running, len);
        
    }

    /* Return -1 if receive error, or total bytes received otherwise */
    if (bytes == -1) {
        perror("recv");
        return -1;
    }
    else {
		/* Null-terminate buffer */
		buf[bytes] = '\0';
		return bytes;
	}
}

ssize_t sendAll(int fd, const char *buf) {
    int bytes, total;
    int running = 0;

    /* Make sure buf is not NULL */
    if (buf == NULL) {
        fprintf(stderr, "NULL buffer specified in send function.\n");
        return -1;
    }
    
    total = strlen(buf);

    /* Send until there is nothing left to send */
    while (running < total) {
        bytes = send(fd, &buf[running], total, 0);
        if (bytes == -1 && errno != EINTR) {
            /* Some non-interrupt error occurred. Abort. */
            perror("send");
            break;
        }
        printf("sent %d bytes of %d total: %.*s\n", running + bytes, total, bytes, &buf[running]);
        /* Update running total */
        running += bytes;
    }

    /* Return -1 if send error, or total bytes sent otherwise */
    if (bytes == -1) return -1;
    else return total;
}

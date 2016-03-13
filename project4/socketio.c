#include "socketio.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

ssize_t receiveAny(int fd, char *buf, size_t len) {
    ssize_t bytes;

    bytes = recv(fd, buf, len, 0);

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
    ssize_t bytes;
    size_t running = 0;

    while (running < len - 1) {
        /* Block until all data is received */
        bytes = recv(fd, &buf[running], len, MSG_WAITALL);
        
        /* Stop if an error other than a signal interrupt occurred,
           or if socket was closed */
        if (bytes == 0) {
            break;
        } else if (bytes == -1) {
            /* Try again if interrupted by signal */
            if (errno == EINTR) continue;
            /* otherwise, return error */
            else break;
        }
        running += bytes;
        
        printf("received %d bytes, %du of %du total\n", bytes, running, len);
        
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
    ssize_t bytes;
    size_t total, running = 0;

    /* Use NULL terminator to determine total length */
    total = strlen(buf);

    /* Send until there is nothing left to send */
    while (running < total) {
        bytes = send(fd, &buf[running], total, 0);
        if (bytes == 0) {
            /* Socket was closed */
            break;
        } else if (bytes == -1) {
            if (errno == EINTR) continue;
            else {
                /* Some non-interrupt error occurred. Abort. */
                perror("send");
                break;
            }
        }
        printf("sent %du bytes of %du total: %.*s\n", running + bytes, total, &buf[running]);
        /* Update running total */
        running += bytes;
    }

    /* Return -1 if send error, or total bytes sent otherwise */
    if (bytes == -1) return -1;
    else return total;
}

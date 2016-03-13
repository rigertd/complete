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
        printf("received %d bytes total: '%s'\n", (int)bytes, buf);
        return bytes;
    }
}

ssize_t receiveAll(int fd, char *buf, size_t len) {
    ssize_t bytes;
    size_t running = 0;

    /* Keep trying until all data is received */
    while (running < len) {
        bytes = recv(fd, &buf[running], len, 0);
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
        
        printf("received %d bytes, %d of %d total: '%s'\n", (int)bytes, (int)running, (int)len, &buf[running - bytes]);
        
    }

    /* Return -1 if receive error, or total bytes received otherwise */
    if (bytes == -1) {
        perror("recv");
        return -1;
    }
    else {
        /* Null-terminate buffer */
        buf[running] = '\0';
        printf("received %d bytes total: '%s'\n", (int)running, buf);
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
        printf("sent %d bytes of %d total: '%s'\n", (int)(running + bytes), (int)total, &buf[running]);
        /* Update running total */
        running += bytes;
    }

    /* Return -1 if send error, or total bytes sent otherwise */
    if (bytes == -1) return -1;
    else return total;
}

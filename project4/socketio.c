#include "socketio.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#define BUFFER_SIZE 512

int getData(int fd, const char *request, char **buf) {
    int result = -1;
    
    /* Free data buffer if already allocated */
    if (buf != NULL) {
        free(*buf);
        *buf = NULL;
    }
    
    /* Send request */
    if (sendAll(fd, request) < 0) {
        result = 0;
    }

    /* Receive data */
    if (result && receiveAll(fd, buf) < 0) {
        if (*buf != NULL) free(*buf);
    }

    return result;
}

ssize_t receiveAll(int fd, char **buf) {
    int bytes, total = 0;
    char buffer[BUFFER_SIZE + 1];
    char *tmp = NULL;

    /* Make sure buf is not already allocated */
    if (*buf != NULL) {
        free(*buf);
        *buf = NULL;
    }

    while ((bytes = recv(fd, buffer, BUFFER_SIZE, 0)) > 0) {
        printf("received %d bytes, %d total\n", bytes, bytes + total);
        
        /* Allocate enough memory to store total data received so far */
        tmp = malloc(bytes + total + 1);
        if (tmp == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        /* Copy old data (if any) to new string */
        if (total > bytes) {
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
        
        /* Exit loop if nothing left to receive */
        if (recv(fd, buffer, BUFFER_SIZE, MSG_PEEK | MSG_DONTWAIT) <= 0)
            break;
    }
    printf("data received: '%s'\n", *buf);

    /* Return -1 if receive error, or total bytes received otherwise */
    if (bytes == -1) {
        perror("recv");
        return -1;
    }
    else return total;
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

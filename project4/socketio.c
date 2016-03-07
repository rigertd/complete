#include "socketio.h"

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
    if (bytes == -1) {
        perror("recv");
        return -1;
    }
    else return total;
}

int sendAll(int fd, const char *buf) {
    int bytes, total;
    int running = 0;

    /* Make sure buf is not NULL */
    if (buf == NULL) {
        fprintf(stderr, "NULL buffer specified in send function.\n");
        return -1;
    }
    
    total = strlen(buf);

    /* Send until there is nothing left to send */
    while ((bytes = send(fd, &buf[running], BUFFER_SIZE - 1, 0)) > 0) {
        printf("sent %d bytes of %d total: %.*s\n", running + bytes, total, bytes, &buf[running]);
        /* Update running total */
        running += bytes;
    }

    /* Return -1 if send error, or total bytes sent otherwise */
    if (bytes == -1) return -1;
    else return total;
}

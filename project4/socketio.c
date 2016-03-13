/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 4 - OTP
* File:         socketio.c
* Description:  Implementation file for socketio.h
\*********************************************************/
#include "socketio.h"

#include <errno.h>      /* perror, errno */
#include <stdio.h>      /* perror */
#include <string.h>     /* strlen */
#include <sys/socket.h> /* recv, send */

/**
 * Receives the specified amount data over the specified socket.
 *
 * This function continues to read the socket until the specified
 * amount of data is received or an error is encountered.
 * If only part of the data is received due to an error or closed socket,
 * it returns the amount of data that was actually received.
 *
 *  fd      Socket descriptor to receive data from.
 *  buf     Buffer to store the data in. Must be large enough to hold
 *          all data plus the NULL terminator.
 *  len     Amount of data to receive.
 *
 * Returns the number of bytes received, or -1 if an error occurred.
 */
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
    }

    /* Return -1 if receive error, or total bytes received otherwise */
    if (bytes == -1) {
        perror("recv");
        return -1;
    }
    else {
        /* Null-terminate buffer */
        buf[running] = '\0';
        /* printf("received %d bytes total: '%s'\n", (int)running, buf); */
        return bytes;
    }
}

/**
 * Receives the data is currently available on the specified socket.
 *
 * This function only reads what is immediately available on the socket.
 * It returns the amount of data that was actually received.
 *
 *  fd      Socket descriptor to receive data from.
 *  buf     Buffer to store the data in. Must be at least len + 1 in size.
 *  len     Maximum amount of data to receive.
 *
 * Returns the number of bytes received, or -1 if an error occurred.
 */
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
        /* printf("received %d bytes total: '%s'\n", (int)bytes, buf); */
        return bytes;
    }
}

/**
 * Sends the specified amount data over the specified socket.
 *
 * This function continues to send data over the socket until the specified
 * amount of data is sent or an error is encountered.
 * If only part of the data is sent due to an error or closed socket,
 * it returns the amount of data that was actually sent.
 *
 *  fd      Socket descriptor to send data over.
 *  buf     Data to send. Must be NULL terminated.
 *
 * Returns the number of bytes sent, or -1 if an error occurred.
 */
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
        /* printf("sent %d bytes of %d total: '%s'\n", (int)(running + bytes), (int)total, &buf[running]); */
        /* Update running total */
        running += bytes;
    }

    /* Return -1 if send error, or total bytes sent otherwise */
    if (bytes == -1) return -1;
    else return total;
}

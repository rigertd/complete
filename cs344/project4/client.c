/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 4 - OTP
* File:         client.c
* Description:  Implementation file for client.h
\*********************************************************/
#include "client.h"

#include <stdlib.h>     /* malloc, free */
#include <stdio.h>      /* printf, fprintf, perror */
#include <errno.h>      /* perror */
#include <string.h>     /* memset, strcmp */
#include <netdb.h>      /* addrinfo, getaddrinfo, gai_strerror */
#include <sys/socket.h> /* addrinfo, getaddrinfo, gai_strerror */
#include <sys/stat.h>   /* open */
#include <fcntl.h>      /* open */
#include <unistd.h>     /* close, lseek */

#include "socketio.h"

/*========================================================*
 * Macro definitions
 *========================================================*/
/* Length of buffer for converting plaintext and key sizes to a string */
#define BUF_SIZE 50
/* Exit code for attempts to contact the incorrect server type */
#define EXIT_INVALID_SERVER 2

/**
 * Connects to the server on the localhost at the specified port.
 *
 * This function attempts to connect to a server running on the specified
 * port on the localhost using IPv6 or IPv4, whichever is available.
 *
 *  prog    The name of the calling program.
 *  port    The port to bind, as a c-style string.
 *
 * Returns the socket descriptor for the connection.
 */
int connectServer(const char *prog, const char *port) {
    int fd, val;
    struct addrinfo hints, *result, *rp;

    /* Zero-initialize and set addrinfo structure */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* IPv4 or IPv6, whichever is available */
    hints.ai_socktype = SOCK_STREAM;/* TCP */
    hints.ai_flags = AI_PASSIVE;    /* Use localhost IP */

    /* Look up the localhost address info */
    val = getaddrinfo(NULL, port, &hints, &result);
    if (val != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(val));
        exit(EXIT_FAILURE);
    }

    /* Loop through address structure results until connect succeeds */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        /* Attempt to open a socket based on the localhost's address info */
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd == -1) {
            perror("server: socket");
            continue; /* Try next on error */
        }

        /* Attempt to connect to the open socket */
        if (connect(fd, rp->ai_addr, rp->ai_addrlen) == -1) {
            fprintf(stderr, "%s: ", prog);
            perror("connect error");
            close(fd);
            continue; /* Try next on error */
        }

        /* Connection succeeded if we reach here */
        break;
    }

    if (rp == NULL) {
        fprintf(stderr, "%s: No valid address found\n", prog);
        exit(EXIT_FAILURE);
    }

    /* Free memory used by localhost's address info */
    freeaddrinfo(result);

    /* Return the connected socket file descriptor */
    return fd;
}

/**
 * Requests the specified operation on the specified port.
 *
 * This function attempts to load message and key data from the specified
 * files, connect to a server running on localhost on the specified port,
 * and request the specified type of operation.
 *
 *  prog    The name of the calling program.
 *  msgpath The path to the file that contains the message data.
 *  msgpath The path to the file that contains the key data.
 *  port    The port to bind, as a c-style string.
 *  type    The type of request. Must be ENCRYPT_REQ or DECRYPT_REQ.
 *
 * Returns 0 for success, 1 for failure, and 2 for invalid server type.
 */
int requestOp(  const char *prog,
                const char *msgpath,
                const char *keypath,
                const char *port,
                const char *type    ) {
    int serverfd;
    char *msg = NULL, *key = NULL;
    ssize_t msglen, keylen;

    /* Load plaintext data */
    msglen = loadData(msgpath, &msg);

    /* Load key data */
    keylen = loadData(keypath, &key);

    /* Verify key length */
    if (keylen < msglen) {
        fprintf(stderr, "%s error: key '%s' is too short\n", prog, keypath);
        if (key != NULL) free(key);
        if (msg != NULL) free(msg);
        return EXIT_FAILURE;
    }

    /* Connect to the server */
    serverfd = connectServer(prog, port);

    /* Send request type and key/message lengths to server */
    char buffer[BUF_SIZE];
    snprintf(buffer, BUF_SIZE, "%s %d %d", type, (int)keylen, (int)msglen);
    sendAll(serverfd, buffer);

    /* Get new port number from server */
    receiveAny(serverfd, buffer, BUF_SIZE);

    /* Verify response from server */
    if (strcmp(buffer, INVALID_TYPE) == 0) {
        char othertype[8];
        if (strcmp(type, ENCRYPT_REQ) == 0)
            strcpy(othertype, DECRYPT_REQ);
        else
            stpcpy(othertype, ENCRYPT_REQ);
        fprintf(stderr,
            "%s error: cannot use %s server on port %s from %s client\n",
            prog, othertype, port, type);
        if (key != NULL) free(key);
        if (msg != NULL) free(msg);
        return EXIT_INVALID_SERVER;
    }

    /* Close current connection and open new one on new port */
    close(serverfd);

    serverfd = connectServer(prog, buffer);

    /* Wait for server to ask for key data */
    receiveAny(serverfd, buffer, BUF_SIZE);
    if (strcmp(buffer, KEY_REQ) != 0) {
        fprintf(stderr, "%s error: no key request received\n", prog);
        if (key != NULL) free(key);
        if (msg != NULL) free(msg);
        return EXIT_FAILURE;
    }

    /* Send the key data */
    if (sendAll(serverfd, key) == -1) {
        fprintf(stderr, "%s error: sending key data failed\n", prog);
        if (key != NULL) free(key);
        if (msg != NULL) free(msg);
        return EXIT_FAILURE;
    }

    /* Free memory for key */
    if (key != NULL) free(key);
    key = NULL;

    /* Receive acknowledgement */
    receiveAny(serverfd, buffer, BUF_SIZE);
    if (strncmp(buffer, MESSAGE_REQ, 3) != 0) {
        fprintf(stderr, "%s error: unexpected response from server\n", prog);
        if (msg != NULL) free(msg);
        return EXIT_FAILURE;
    }

    /* Send message data */
    if (sendAll(serverfd, msg) == -1) {
        fprintf(stderr, "%s error: sending plaintext message failed\n", prog);
        if (msg != NULL) free(msg);
        return EXIT_FAILURE;
    }

    /* Receive encrypted data */
    msglen = receiveAll(serverfd, msg, msglen);

    /* Verify result */
    if (strcmp(msg, KEY_ERROR) == 0) {
        fprintf(stderr, "%s error: key '%s' is too short\n", prog, keypath);
        if (msg != NULL) free(msg);
        return EXIT_FAILURE;
    } else if (strcmp(msg, INVALID_CHAR) == 0) {
        fprintf(stderr, "%s error: input contains bad characters\n", prog);
        if (msg != NULL) free(msg);
        return EXIT_FAILURE;
    }

    /* Print the encrypted message to stdout */
    printf("%s\n", msg);

    /* Free memory allocated for plaintext and key */
    if (msg != NULL) free(msg);
    if (key != NULL) free(key);

    return EXIT_SUCCESS;
}

/**
 * Loads the content of the specified file into the specified buffer.
 *
 * This function allocates memory and loads the data contained in
 * the specified file. The specified pointer points to the data.
 *
 *  path    The path of the file to load.
 *  data    NULL pointer that will point to the file data.
 *
 * Returns the bytes of data loaded.
 */
ssize_t loadData(const char *path, char **data) {
    int fd = openFile(path);
    ssize_t bytes = readline(fd, data);
    close(fd);
    return bytes;
}

/**
 * Opens the file at the specified path for reading.
 *
 *  path    The path of the file to load.
 *
 * Returns a valid file descriptor for the file.
 */
int openFile(const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Failed to open file %s\n", path);
        perror("open");
        exit(EXIT_FAILURE);
    }
    return fd;
}

/**
 * Reads from the specified file descriptor until an EOL char is found.
 * The line does not include the EOL character(s).
 *
 *  fd      The file descriptor to read from. Must be open and readable.
 *  buf     The buffer to store the result in. Must be NULL.
 *
 * Returns the number of bytes in the line.
 * The data is stored in the buf pointer.
 * The data buffer must be freed after it is no longer needed.
 */
ssize_t readline(int fd, char **buf) {
    char *tmp;
    char buffer[BUFFER_SIZE];
    int i, flag;
    ssize_t bytes;
    size_t total = 0;

    flag = 0;

    /* Make sure buf is NULL */
    if (*buf != NULL) {
        fprintf(stderr, "readline: buffer pointer must be NULL\n");
        exit(EXIT_FAILURE);
    }

    while (!flag && (bytes = read(fd, buffer, BUFFER_SIZE)) > 0) {
        /* Search for newline character in read data */
        for (i = 0; i < bytes; ++i) {
            if (buffer[i] == '\n' || buffer[i] == '\r') {
                buffer[i] = '\0';
                flag = 1;
                break;
            }
        }

        total += i;

        /* Append data to end of heap buffer */
        /* Add one for NULL terminator */
        tmp = malloc(total + 1);
        if (tmp == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        /* Copy existing data to new heap buffer */
        if (*buf != NULL) {
            strncpy(tmp, *buf, total - i);
            free(*buf);
            *buf = NULL;
        }
        /* Copy stack buffer to heap buffer */
        strncpy(&tmp[total - i], buffer, i);

        /* Add NULL terminator to end of buffer */
        tmp[total] = '\0';

        /* Assign new string to buf pointer */
        *buf = tmp;
    }

    /* Return -1 if read fails */
    if (bytes < 0) {
        return -1;
    }

    /* Reset position of fd to next non-whitespace character */
    while (++i < bytes
        && (buffer[i] == ' '
            || buffer[i] == '\n'
            || buffer[i] == '\t'
            || buffer[i] == '\r')
        ) {
    }
    lseek(fd, -(bytes - i), SEEK_CUR);

    return total;
}

/**
 * Verifies that the correct number of command-line arguments were specified.
 *
 * This function makes sure the correct number of command-line arguments were
 * specified and displays help if not, or if the --help or -h argument was
 * specified. This function does not verify the validity of the argument values
 * themselves.
 *
 *  argc    The number of arguments.
 *  argv    The values of the arguments.
 */
void verifyArgs(int argc, char *argv[]) {
    if (argc < 4 ||
        strcmp(argv[1], "--help") == 0 ||
        strcmp(argv[1], "-h") == 0) {
        fprintf(stderr, "usage: %s message key port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

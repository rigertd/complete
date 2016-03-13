#include "client.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "socketio.h"

int connectServer(char *port) {
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
            perror("client: connect");
            close(fd);
            continue; /* Try next on error */
        }
        
        /* Connection succeeded if we reach here */
        break;
    }
    
    if (rp == NULL) {
        fprintf(stderr, "connect: No valid address found");
        exit(EXIT_FAILURE);
    }
    
    /* Free memory used by localhost's address info */
    freeaddrinfo(result);

    /* Return the connected socket file descriptor */
    return fd;
}

int openFile(char *path) {
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
                buffer[i] == '\0';
                flag = 1;
                break;
            }
        }
        
        total += i;
        
        /* Append data to end of heap buffer */
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
        buffer[i] = '\0';
    }
    lseek(fd, -(bytes - i), SEEK_CUR);
    
    return total;
}


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "socketio.h"

/* Length of buffer for converting plaintext and key sizes to a string */
#define BUF_SIZE 50

int main(int argc, char *argv[]) {
    verifyArgs(argc, argv);
    
    int serverfd;
    char *msg = NULL, *key = NULL;
    ssize_t msglen, keylen;
    
    /* Load plaintext data */
    msglen = loadData(argv[1], &msg);
    
    /* Load key data */
    keylen = loadData(argv[2], &key);
    
    /* Verify key length */
    if (keylen < msglen) {
        fprintf(stderr, "%s error: key '%s' is too short\n", argv[0], argv[2]);
        if (key != NULL) free(key);
        if (msg != NULL) free(msg);
        exit(EXIT_FAILURE);
    }
    
    /* Connect to the server */
    serverfd = connectServer(argv[3]);
    
    /* Send request type and key/message lengths to server */
    char buffer[BUF_SIZE];
    snprintf(buffer, BUF_SIZE, "ENCRYPT %d %d", (int)keylen, (int)msglen);
    sendAll(serverfd, buffer);
    
    /* Get new port number from server */
    receiveAny(serverfd, buffer, BUF_SIZE);
    
    /* Verify response from server */
    if (strncmp(buffer, "INVALID", BUF_SIZE) == 0) {
        fprintf(stderr, "%s error: invalid server type\n", argv[0]);
        if (key != NULL) free(key);
        if (msg != NULL) free(msg);
        exit(EXIT_FAILURE);
    }
    
    /* Close current connection and open new one on new port */
    close(serverfd);

    serverfd = connectServer(buffer);
    
    /* Wait for server to ask for key data */
    receiveAny(serverfd, buffer, BUF_SIZE);
    if (strcmp(buffer, "KEY") != 0) {
        fprintf(stderr, "%s error: no key request received\n", argv[0]);
        if (key != NULL) free(key);
        if (msg != NULL) free(msg);
        exit(EXIT_FAILURE);
    }
    
    /* Send the key data */
    if (sendAll(serverfd, key) == -1) {
        fprintf(stderr, "%s error: sending key data failed\n", argv[0]);
        if (key != NULL) free(key);
        if (msg != NULL) free(msg);
        exit(EXIT_FAILURE);
    }
    
    /* Free memory for key */
    if (key != NULL) free(key);
    key = NULL;
    
    /* Receive acknowledgement */
    receiveAny(serverfd, buffer, BUF_SIZE);
    if (strncmp(buffer, "MSG", 3) != 0) {
        fprintf(stderr, "%s error: unexpected response from server\n", argv[0]);
        if (msg != NULL) free(msg);
        exit(EXIT_FAILURE);
    }
    
    /* Send plaintext message data */
    if (sendAll(serverfd, msg) == -1) {
        fprintf(stderr, "%s error: sending plaintext message failed\n", argv[0]);
        if (msg != NULL) free(msg);
        exit(EXIT_FAILURE);
    }
    
    /* Receive encrypted data */
    msglen = receiveAll(serverfd, msg, msglen);
    
    /* Verify result */
    if (strncmp(msg, "key_error", 9) == 0) {
        fprintf(stderr, "%s error: key '%s' is too short\n", argv[0], argv[2]);
        if (msg != NULL) free(msg);
        exit(EXIT_FAILURE);
    } else if (strncmp(msg, "invalid_char", 12) == 0) {
        fprintf(stderr, "%s error: input contains bad characters\n", argv[0]);
        if (msg != NULL) free(msg);
        exit(EXIT_FAILURE);
    }
    
    /* Print the encrypted message to stdout */
    printf("%s\n", msg);
    
    /* Free memory allocated for plaintext and key */
    if (msg != NULL) free(msg);
    if (key != NULL) free(key);

    return EXIT_SUCCESS;
}

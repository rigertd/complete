#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "socketio.h"

#define BUF_SIZE 32

void requestEncryption() {
}

int main(int argc, char *argv[]) {
    if (argc < 4 ||
        strcmp(argv[1], "--help") == 0 ||
        strcmp(argv[1], "-h") == 0) {
        fprintf(stderr, "usage: %s plaintext key port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    int serverfd, msgfd, keyfd;
    char *msg = NULL, *key = NULL;
    ssize_t msglen, keylen;
    
    /* Open files and socket */
    msgfd = openFile(argv[1]);
    keyfd = openFile(argv[2]);
    serverfd = connectServer(argv[3]);
    
    /* Attempt to read plaintext and key from files */
    msglen = readline(msgfd, &msg);
    keylen = readline(keyfd, &key);
    
    /* Close files */
    close(msgfd);
    close(keyfd);
    
    /* Verify key length */
    if (keylen < msglen) {
        fprintf(stderr, "Error: key '%s' is too short", argv[2]);
        if (key != NULL) free(key);
        if (msg != NULL) free(msg);
        exit(EXIT_FAILURE);
    }
    
    /* Send request type and key/message lengths to server */
    char buffer[BUF_SIZE]; /* Make buffer long enough to hold max len of 2 ints */
    snprintf(buffer, BUF_SIZE, "ENCRYPT %d %d", (int)keylen, (int)msglen);
    printf("otp_enc: sending '%s'\n", buffer);
    sendAll(serverfd, buffer);
    
    /* Get new port number from server */
    printf("otp_enc: waiting for port\n");
    receiveAny(serverfd, buffer, BUF_SIZE);
    printf("otp_enc: received port '%s'\n", buffer);
    
    /* Verify response from server */
    if (strncmp(buffer, "INVALID", BUF_SIZE) == 0) {
        fprintf(stderr, "otp_enc error: invalid server type\n");
        if (key != NULL) free(key);
        if (msg != NULL) free(msg);
        exit(EXIT_FAILURE);
    }
    
    /* Close current connection and open new one on new port */
    close(serverfd);
    printf("otp_enc: connecting to '%s'\n", buffer);

    serverfd = connectServer(buffer);
    printf("otp_enc: connected\n", buffer);
    
    /* Send the key data */
    printf("otp_enc: sending key '%s'\n", key);
    if (sendAll(serverfd, key) == -1) {
        fprintf(stderr, "otp_enc error: sending key data failed\n");
        if (key != NULL) free(key);
        if (msg != NULL) free(msg);
        exit(EXIT_FAILURE);
    }
    printf("otp_enc: sent key\n");
    
    /* Free memory for key */
    if (key != NULL) free(key);
    key = NULL;
    
    /* Receive acknowledgement */
    printf("otp_enc: waiting for ACK\n");
    receiveAny(serverfd, buffer, BUF_SIZE);
    printf("otp_enc: received '%s'\n", buffer);
    if (strncmp(buffer, "MSG", 3) != 0) {
        fprintf(stderr, "otp_enc error: unexpected response from server\n");
        if (msg != NULL) free(msg);
        exit(EXIT_FAILURE);
    }
    
    /* Send plaintext message data */
    printf("otp_enc: sending msg '%s'\n", msg);
    if (sendAll(serverfd, msg) == -1) {
        fprintf(stderr, "otp_enc error: sending plaintext message failed\n");
        if (msg != NULL) free(msg);
        exit(EXIT_FAILURE);
    }
    printf("otp_enc: sent msg\n");
    
    /* Receive encrypted data */
    printf("otp_enc: waiting for encrypted data\n");
    receiveAll(serverfd, msg, msglen);
    printf("otp_enc: received '%s'\n", msg);
    
    /* Verify result */
    if (strcmp(msg, "KEYERROR") == 0) {
        fprintf(stderr, "otp_enc error: key '%s' is too short\n", argv[2]);
        if (msg != NULL) free(msg);
        exit(EXIT_FAILURE);
    } else if (strcmp(msg, "INVALIDCHAR") == 0) {
        fprintf(stderr, "otp_enc error: input contains bad characters\n");
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

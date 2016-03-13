#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "client.h"

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
    
    printf("msg: '%s'\n", msg);
    printf("key: '%s'\n", key);
    
    /* Free memory allocated for plaintext and key */
    if (msg != NULL) free(msg);
    if (key != NULL) free(key);

    return EXIT_SUCCESS;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "socketio.h"

/**
 * Gets a pointer to the sin_addr for IPv4 and sin6_addr for IPv6.
 * Copied from https://beej.us/guide/bgnet/
 */
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
    if (argc < 4 ||
        strcmp(argv[1], "--help") == 0 ||
        strcmp(argv[1], "-h") == 0) {
        fprintf(stderr, "usage: %s plaintext key port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

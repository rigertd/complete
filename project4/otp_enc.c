#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 512


int main(int argc, char *argv[]) {
    if (argc < 4 || 
        strcmp(argv[1], "--help") == 0 || 
        strcmp(argv[1], "-h") == 0) {
        fprintf(stderr, "usage: %s plaintext key port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
}

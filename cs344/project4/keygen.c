/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 4 - OTP
* File:         keygen.c
* Description:  An encryption key generator for a one-time-pad.
*
*               This program generates a pseudorandom encryption key of
*               the specified length and outputs it to stdout.
*
*               The command line syntax is as follows:
*
*                   keygen key_length
*
*               This program takes the following arguments:
*               - key_length    -- The length of the key to generate.
\*********************************************************/
#include <errno.h>      /* perror */
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>      /* fprintf, printf */
#include <string.h>     /* strcmp */
#include <time.h>       /* time */
#include <unistd.h>     /* write, getpid */
#include <sys/types.h>  /* getpid */

/*========================================================*
 * main function
 *========================================================*/
int main(int argc, char *argv[]) {
    long i, keylen; /* Loop variable and key length */
    char *end;      /* Stores first invalid char in strtol */
    char *buf;      /* Stores the key prior to output */

    /* Verify number of arguments and print help if needed */
    if (argc < 2 ||
        strcmp(argv[1], "--help") == 0 ||
        strcmp(argv[1], "-h") == 0) {
        fprintf(stderr, "usage: %s key_length\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Convert argument to number and validate */
    keylen = 0;
    end = NULL;
    keylen = strtol(argv[1], &end, 10);
    if (end != NULL && end[0] != '\0') {
        fprintf(stderr, "%s: Non-integer key length specified\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (errno == ERANGE) {
        fprintf(stderr, "%s: Specified key length too long\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Seed random number generator */
    /* Use PID for additional randomness when called in rapid succession */
    srand(time(0) ^ getpid());

    /* Allocate memory to store key */
    buf = malloc(keylen + 1);
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    /* Write a random sequence of uppercase letters and spaces to buffer */
    for (i = 0; i < keylen; ++i) {
        buf[i] = '@' + (rand() % 27);
        /* Replace '@' with space */
        if (buf[i] == '@') buf[i] = ' ';
    }

    /* Add line feed to end */
    buf[keylen] = '\n';

    /* Write buffer to stdout */
    write(STDOUT_FILENO, buf, (unsigned int)keylen + 1);
    
    /* Free key buffer */
    free(buf);
    buf = NULL;

    return EXIT_SUCCESS;
}
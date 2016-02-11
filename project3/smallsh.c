/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 3 - smallsh
* File:         smallsh.c
* Description:  Implementation file for smallsh.h
\*********************************************************/

#include "smallsh.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

/*========================================================*
 * Macro definitions
 *
 * Define constants here for easy modification and to avoid magic numbers.
 *========================================================*/

/*========================================================*
 * Structs and enumerations
 *========================================================*/
/**
 *
 */
typedef struct Command {
    int argc;
    char** argv;
    int background;
    char* infile;
    char* outfile;
} Command;

/*========================================================*
 * Function prototypes
 *========================================================*/
void parseCommand(Command*, char*);
void catchint();
void registerIntHandler(void (*)(int));

/*========================================================*
 * Constant definitions
 *========================================================*/


/*========================================================*
 * main function
 *========================================================*/
int main(int argc, char* argv[]) {
    /* start by registering signal handler for CTRL+C*/
    registerIntHandler(catchint);
    
    return 0;
}

/*========================================================*
 * Function definitions
 *========================================================*/
/**
 *
 */
void parseCommand(Command* cmd, char* input) {
    if (cmd == 0) {
        fprintf(stderr, "NULL Command object specified.\n");
        exit(1);
    }
}

void catchint() {
    printf("Caught interrupt signal.\n");
}

void registerIntHandler(void (*handler)(int)) {
    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigfillset(&act.sa_mask);
    sigaction(SIGINT, &act, NULL);
}
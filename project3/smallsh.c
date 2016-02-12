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
#include <stdlib.h>
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
void fputs_r(const char*, FILE*);

/*========================================================*
 * Constant definitions
 *========================================================*/


/*========================================================*
 * main function
 *========================================================*/
int main(int argc, char* argv[]) {
    /* start by registering signal handler for CTRL+C*/
    registerIntHandler(catchint);
    
    while (1) {
		sleep(1);
	}
	
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
        fputs_r("NULL Command object specified.\n", stderr);
        exit(1);
    }
}

void catchint() {
    fputs_r("Caught interrupt signal.\n", stdout);
}

void registerIntHandler(void (*handler)(int)) {
    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigfillset(&act.sa_mask);
    sigaction(SIGINT, &act, NULL);
}

void fputs_r(const char* str, FILE* stream) {
	sigset_t all, old, empty;
	sigfillset(&all);
	sigemptyset(&empty);
	if (sigprocmask(SIG_BLOCK, &all, &old) < 0) {
		fputs("Error blocking signals.\n", stderr);
		exit(1);
	}
	fputs(str, stream);
	sigsuspend(&empty);
	if (sigprocmask(SIG_UNBLOCK, &old, NULL) < 0) {
		fputs("Error unblocking signals.\n", stderr);
		exit(1);
	}
}

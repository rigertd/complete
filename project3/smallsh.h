/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 3 - smallsh
* File:         smallsh.h
* Description:  
\*********************************************************/
#ifndef SMALLSH_H
#define SMALLSH_H

#include "bgvector.h"
#include <unistd.h>

/*========================================================*
 * Macro definitions
 *
 * Define constants here for easy modification and to avoid magic numbers.
 *========================================================*/
#define MAX_CMD_LINE_LEN 2048   /* Specifies max length of command line */
#define MAX_ARGS 512            /* Specifies max command line arguments */
#define UNUSED_FD -5            /* Identifies an unused file descriptor */

/*========================================================*
 * Structs
 *========================================================*/
/**
 * Stores the required information to run a command from the shell.
 *
 *  argc        The number of arguments to pass to the command.
 *  argv        The array of argument strings.
 *  buffer      The buffer to store the argument strings.
 *  background  Whether to run the command in the background.
 *  infile      The filename to use for input, or NULL.
 *  outfile     The filename to use for output, or NULL.
 *  infd        The file descriptor of the input file.
 *  outfd       The file descriptor of the output file.
 */
typedef struct Command {
    int argc;
    char *argv[MAX_ARGS];
    char buffer[MAX_CMD_LINE_LEN];
    int background;
    char *infile;
    char *outfile;
    int infd;
    int outfd;
} Command;

/*========================================================*
 * Function prototypes
 *========================================================*/
void changeDirectory(char *);
void intToString(int, char *, int);
void parseCommand(Command *);
void printBgStatus(pid_t, int);
void printInt(int, int);
void printStatus(int);
void printString(int, char *);
void readLine(int, char *, size_t);
int runCommand(Command *, pid_t *);
void waitBgChildren(BgProcessVector *);

#endif /* end of SMALLSH_H */

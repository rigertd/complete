#ifndef COMMAND_H
#define COMMAND_H

#include <sys/types.h>

#define MAX_CMD_LINE_LEN 2048
#define MAX_ARGS 512
#define UNUSED_FD -5

/**
 *
 */
typedef struct Command {
    char buffer[MAX_CMD_LINE_LEN];
    int argc;
    char* argv[MAX_ARGS];
    int background;
    char* infile;
    int infd;
    char* outfile;
    int outfd;
} Command;

void parseCommand(Command*);
pid_t runCommand(Command*, int*);

#endif
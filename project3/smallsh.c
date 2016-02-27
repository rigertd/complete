/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 3 - smallsh
* File:         smallsh.c
* Description:  Implementation file for smallsh.h
\*********************************************************/

#include "smallsh.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

/*========================================================*
 * main function
 *========================================================*/
int main(int argc, char* argv[]) {
    /* Ignore the interrupt signal in the shell itself */
    signal(SIGINT, SIG_IGN);

    BgProcessVector bgPids; /* Stores PIDs of background processes */
    Command cmd;            /* Stores the parsed user command */
    int lastCmdStatus = 0;  /* Status of the last foreground command */
    pid_t cpid;             /* PID of new process spawned */
    
    /* Initialize the bgPids BgProcessVector */
    initBgProcessVector(&bgPids, 4);
    
    /* Command loop. Loop until user enters "exit" */
    do {
        /* Check if any background processes can be waited */
        waitBgChildren(&bgPids);
        
        /* Display the prompt */
        write(STDOUT_FILENO, ": ", 2);
        
        /* Get a line of user (or other) input */
        readLine(STDIN_FILENO, cmd.buffer, MAX_CMD_LINE_LEN);
        
        /* Parse the line of input and populate a Command object */
        parseCommand(&cmd);
        
        /* Only run the command if the user entered something */
        if (cmd.argc > 0) {
            /* Check for built-in commands: 'exit', 'cd', or 'status' */
            if (strcmp(cmd.argv[0], "exit") == 0) {
                /* Terminate the shell by breaking out of command loop */
                break;
            } else if (strcmp(cmd.argv[0], "cd") == 0) {
                changeDirectory(cmd.argv[1]);
            } else if (strcmp(cmd.argv[0], "status") == 0) {
                printStatus(lastCmdStatus);
            } else {
                /* Not a built-in command--run it.
                   Only store exit status if foreground command. */
                if (cmd.background)
                    runCommand(&cmd, &cpid);
                else
                    lastCmdStatus = runCommand(&cmd, &cpid);
                
                /* Add PID to bgPids if a background command succeeds */
                if (cpid > -1 && cmd.background) {
                    /* Keep track of file descriptors to ensure they are
                       closed after the child process terminates. */
                    BgProcess bgp = {cpid, cmd.infd, cmd.outfd};
                    pushBackBgProcessVector(&bgPids, bgp);
                }
            }
        }
    } while (1);
    
    /* Finalize the bgPids BgProcessVector */
    finalizeBgProcessVector(&bgPids);
    
    return EXIT_SUCCESS;
}

/*========================================================*
 * Function definitions
 *========================================================*/
/**
 * Changes the current working directory to the specified path.
 * Displays an error message to STDERR if invalid path is specified.
 * Changes to the user's HOME directory if path is NULL.
 *
 *  path    The path to change to, or NULL for user's HOME directory.
 */
void changeDirectory(char* path) {
    struct stat fldr;   /* for checking if a path exists */
    
    /* parseCommand sets argv[1] to NULL if nothing entered after cd */
    if (path == NULL) {
        path = getenv("HOME");
    }
    
    /* Check whether specified path exists, and display error if not */
    if (stat(path, &fldr) == -1) {
        printString(STDERR_FILENO, "smallsh: cd: ");
        printString(STDERR_FILENO, path);
        switch (errno) {
        case ENOENT:
            printString(STDERR_FILENO, ": No such file or directory\n");
            break;
        case EACCES:
            printString(STDERR_FILENO, ": Access denied\n");
            break;
        case ENOTDIR:
            printString(STDERR_FILENO, ": Not a directory\n");
            break;
        default:
            printString(STDERR_FILENO, ": Unknown error\n");
            break;
        }
    } else {
        /* Path found--change directory to specified path */
        chdir(path);
    }
}

/**
 * Converts the specified integer value to a string and
 * stores it in the specified buffer.
 *
 *  val     The integer value to convert.
 *  buf     The buffer to store the string in.
 *  size    The size of the buffer--must be big enough to hold the value.
 */
void intToString(int val, char *buf, int size) {
    int i, j;
    char tmp;
    i = 0;
    j = val;
    
    /* Get the absolute value of val and write the string in reverse */
    j = j > 0 ? j : j * -1;
    while (j > 0) {
        buf[i++] = '0' + j % 10;
        j /= 10;
    }
    
    /* Append the negative sign at the end if value is negative */
    if (val < 0) buf[i++] = '-';
    
    /* Reverse the string */
    for (j = 0; j < i / 2; ++j) {
        tmp = buf[j];
        buf[j] = buf[i - j - 1];
        buf[i - j - 1] = tmp;
    }
    
    /* Fill the rest of the buffer with NULL */
    for (j = i; j < size; ++j) {
        buf[j] = '\0';
    }
}

/**
 * Parses a line of user input and stores it in the specified 
 * Command structure.
 *
 *  cmd     The Command structure for storing the command.
 */
void parseCommand(Command* cmd) {
    char *saveptr, *token, *temp;
    
    /* Initialize Command structure members */
    cmd->background = 0;
    cmd->argc = 0;
    cmd->infile = NULL;
    cmd->outfile = NULL;

    /* Get the first token using the reentrant strtok */
    token = strtok_r(cmd->buffer, " ", &saveptr);
    
    /* Parse input until EOF is reached */
    while (token != NULL) {
        
        if (strncmp(token, "#", 1) == 0) {
            /* Comment marker--ignore rest of line */
            break;
        } else if (strcmp(token, "<") == 0) {
            /* Input redirection */
            if (cmd->argc == 0) {
                /* Print error if redirection comes first */
                printString(STDERR_FILENO, "smallsh: invalid syntax\n");
                break;
            }
            /* Set input file to next token */
            cmd->infile = strtok_r(NULL, " ", &saveptr);
        } else if (strcmp(token, ">") == 0) {
            /* Output redirection */
            if (cmd->argc == 0) {
                /* Print error if redirection comes first */
                printString(STDERR_FILENO, "smallsh: invalid syntax\n");
                break;
            }
            /* Set output file to next token */
            cmd->outfile = strtok_r(NULL, " ", &saveptr);
        } else if (strcmp(token, "&") == 0) {
            /* Possible background command */
            temp = strtok_r(NULL, " ", &saveptr);
            if (temp != NULL) {
                /* Token found after ampersand--add both as arguments */
                cmd->argv[cmd->argc++] = token;
                cmd->argv[cmd->argc++] = temp;
            } else {
                /* No additional tokens found--set as background command */
                cmd->background = 1;
            }
        } else {
            /* Not a special symbol--add to arguments */
            cmd->argv[cmd->argc++] = token;
        }
        
        /* Read the next token */
        token = strtok_r(NULL, " ", &saveptr);
    }
    
    /* Set the final argv to NULL, as required by execvp */
    cmd->argv[cmd->argc] = NULL;
}

/**
 * Prints out the status of the specified background process ID.
 *
 *  pid     The ID of the process to print the status of.
 *  status  The status value associated with the specified pid.
 */
void printBgStatus(pid_t pid, int status) {
    if (WIFEXITED(status)) {
        printString(STDOUT_FILENO, "background pid ");
        printInt(STDOUT_FILENO, (int)pid);
        printString(STDOUT_FILENO, " is done: exit value ");
        printInt(STDOUT_FILENO, (int)WEXITSTATUS(status));
        printString(STDOUT_FILENO, "\n");
    } else if (WIFSIGNALED(status)) {
        printString(STDOUT_FILENO, "background pid ");
        printInt(STDOUT_FILENO, (int)pid);
        printString(STDOUT_FILENO, " is done: terminated by signal ");
        printInt(STDOUT_FILENO, (int)WTERMSIG(status));
        printString(STDOUT_FILENO, "\n");
    }
}

/**
 * Prints the specified integer to the specified file descriptor.
 * This function is thread-safe.
 *
 *  fd      The file descriptor to print to.
 *  str     The string to print.
 */
void printInt(int fd, int val) {
    size_t maxlen = sizeof(int) * 8 / 3 + 1;
    char *buf = malloc(maxlen);
    if (buf == NULL)
    {
        printString(STDERR_FILENO, "smallsh: failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    write(fd, buf, (int)maxlen);
    free(buf);
}

/**
 * Prints out the status of the specified foreground process ID.
 *
 *  pid     The ID of the process to print the status of.
 *  status  The status value associated with the specified pid.
 */
void printStatus(int status) {
    if (WIFEXITED(status)) {
        printString(STDOUT_FILENO, "exit value ");
        printInt(STDOUT_FILENO, (int)WEXITSTATUS(status));
        printString(STDOUT_FILENO, "\n");
    } else if (WIFSIGNALED(status)) {
        printString(STDOUT_FILENO, "terminated by signal ");
        printInt(STDOUT_FILENO, (int)WTERMSIG(status));
        printString(STDOUT_FILENO, "\n");
    }
}

/**
 * Prints the specified string to the specified file descriptor
 * and blocks all signals until it completes.
 *
 *  fd      The file descriptor to print to.
 *  str     The string to print.
 */
void printString(int fd, char *str) {
    int len = strlen(str);
    write(fd, str, len);
}

/**
 * Reads from fd until the first newline or EOF and writes it to buf.
 * Points fd to next unread non-whitespace char.
 * Terminates the program with exit code 1 if cannot read from file.
 *
 *  fd      File descriptor to read from. Must be open and readable.
 *  buf     Buffer to store line of text into.
 *  size    Maximum amount of data to read into buffer.
 */
void readLine(int fd, char* buf, size_t size) {
    int i;
    ssize_t bytes = read(fd, buf, size);

    if (bytes < 0) {
        buf[0] = '\0';
        return;
    }
    /* Search string for newline and replace with NULL terminator,
       then count until non-whitespace char is found
       and set the file offset to that character. */
    for (i = 0; i < bytes; ++i) {
        if (buf[i] == '\n' || buf[i] == '\r') {
            buf[i] = '\0';
            while (++i < bytes
                && (buf[i] == ' '
                    || buf[i] == '\n'
                    || buf[i] == '\t'
                    || buf[i] == '\r')
                ) {
                buf[i] = '\0';
            }
            lseek(fd, -(bytes - i), SEEK_CUR);
            return;
        }
    }

}

/**
 * Runs the specified command structure based on the configured values.
 * You must call parseCommand on the Command structure before passing it
 * to this function.
 *
 *  cmd     The Command structure to run.
 *  cpid    Stores the PID of the resulting child process.
 *
 * Returns the exit status of 
 */
int runCommand(Command* cmd, pid_t* cpid) {
    int status = 0;

    /* Fork the process */
    *cpid = fork();

    if (*cpid == 0) {
        /* This is the child process */
        
        /* Set SIGINT signal handler to default behavior if foreground */
        if (!cmd->background)
            signal(SIGINT, SIG_DFL);
        
        /* Redirect input file to stdin if one is specified */
        if (cmd->infile != NULL) {
            cmd->infd = open(cmd->infile, O_RDONLY);
            if (dup2(cmd->infd, STDIN_FILENO) == -1) {
                printString(STDERR_FILENO, "smallsh: cannot open ");
                printString(STDERR_FILENO, cmd->infile);
                printString(STDERR_FILENO, "for input\n");
                exit(EXIT_FAILURE);
            }
        } else if (cmd->background) {
            /* Redirect /dev/null to stdin if background process */
            cmd->infd = open("/dev/null", O_RDONLY);
            if (dup2(cmd->infd, STDIN_FILENO) == -1) {
                printString(STDERR_FILENO, "smallsh: cannot open /dev/null for input\n");
                exit(EXIT_FAILURE);
            }
        }
        
        /* Redirect stdout to output file if one is specified */
        if (cmd->outfile != NULL) {
            cmd->outfd = open(cmd->outfile, 
                         O_WRONLY | O_CREAT | O_TRUNC, 
                         S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
            if (dup2(cmd->outfd, STDOUT_FILENO) == -1) {
                printString(STDERR_FILENO, "smallsh: cannot open ");
                printString(STDERR_FILENO, cmd->outfile);
                printString(STDERR_FILENO, "for output\n");
                exit(EXIT_FAILURE);
            }
        } else if (cmd->background) {
            /* Redirect stdout to /dev/null if background process */
            cmd->outfd = open("/dev/null", O_WRONLY);
            if (dup2(cmd->outfd, STDOUT_FILENO) == -1) {
                printString(STDERR_FILENO, "smallsh: cannot open /dev/null for output\n");
                exit(EXIT_FAILURE);
            }
        }
        
        /* Execute the command */
        execvp(cmd->argv[0], cmd->argv);
        
        /* If we get here, the command failed to execute */
        printString(STDERR_FILENO, cmd->argv[0]);
        printString(STDERR_FILENO, ": no such file or directory\n");
        exit(EXIT_FAILURE);
    } else if (*cpid == -1) {
        /* Print an error if the fork failed */
        printString(STDERR_FILENO, "Failed to fork process.\n");
    } else {
        /* This is the parent process */
        if (!cmd->background) {
            /* Child is running in foreground. Wait for it to terminate.
               Resume waiting if interrupted by signal */
            while (waitpid(*cpid, &status, 0) < 0 && errno == EINTR) {
                continue;
            }
            
            /* Print message if child process was terminated by a signal */
            if (WIFSIGNALED(status)) {
                printString(STDOUT_FILENO, "background pid is ");
                printInt(STDOUT_FILENO, (int)*cpid);
                printString(STDOUT_FILENO, "\n");
                printf("terminated by signal %d\n", WTERMSIG(status));
            }
            /* Close any open file descriptors */
            close(cmd->infd);
            close(cmd->outfd);
        } else {
            /* Child is running in background. 
               Print PID and return to prompt. */
            printString(STDOUT_FILENO, "background pid is ");
            printInt(STDOUT_FILENO, (int)*cpid);
            printString(STDOUT_FILENO, "\n");

        }
    }
    
    return status;
}

/**
 * Checks if any of the PIDs in the specified BgProcessVector are ready
 * to be waited on. If a PID is ready, it waits on it and prints out the
 * PID and exit value or terminating signal. If a PID is not ready, it
 * skips it and checks the next one. Any invalid PIDs are removed.
 * Any file descriptors that were opened for input or output are closed.
 *
 *  vec     The BgProcessVector containing the PIDs to check.
 */
void waitBgChildren(BgProcessVector *vec) {
    int i, status;
    pid_t cpid;

    /* Loop through vector and attempt to wait for each background process.
       Do not block if process is not finished. */
    for (i = 0; i < vec->size; ++i) {
        cpid = waitpid(getAtBgProcessVector(vec, i).id, &status, WNOHANG);
        
        /* If process successfully waited, remove it and close FDs */
        if (cpid > 0) {
            close(getAtBgProcessVector(vec, i).inFd);
            close(getAtBgProcessVector(vec, i).outFd);
            removeIndexBgProcessVector(vec, i);
            --i;
            printBgStatus(cpid, status);
        } else if (cpid < 0) {
            close(getAtBgProcessVector(vec, i).inFd);
            close(getAtBgProcessVector(vec, i).outFd);
            removeIndexBgProcessVector(vec, i);
            --i;
        }
    }
}


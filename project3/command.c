#include "command.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

void parseCommand(Command* cmd) {
    char *saveptr, *token, *temp;
    
    /* initialize structure members */
    cmd->background = 0;
    cmd->argc = 0;
    cmd->infile = NULL;
    cmd->outfile = NULL;

    token = strtok_r(cmd->buffer, " ", &saveptr);
    
    /* parse input until EOF is reached */
    while (token != NULL) {
        //printf("Token found: %s\n", token);
        
        if (strncmp(token, "#", 1) == 0) {
            /* comment marker found--ignore rest of line */
            break;
        } else if (strcmp(token, "<") == 0) {
            /* print error if redirection comes first */
            if (cmd->argc == 0) {
                write(STDERR_FILENO, "smallsh: invalid syntax\n", 24);
                break;
            }
            cmd->infile = strtok_r(NULL, " ", &saveptr);
            //printf("Set infile to %s\n", cmd->infile);
        } else if (strcmp(token, ">") == 0) {
            /* print error if redirection comes first */
            if (cmd->argc == 0) {
                write(STDERR_FILENO, "smallsh: invalid syntax\n", 24);
                break;
            }
            cmd->outfile = strtok_r(NULL, " ", &saveptr);
            //printf("Set outfile to %s\n", cmd->outfile);
        } else if (strcmp(token, "&") == 0) {
            temp = strtok_r(NULL, " ", &saveptr);
            if (temp != NULL) {
                cmd->argv[cmd->argc++] = token;
                //printf("Token found: %s\n", temp);
                cmd->argv[cmd->argc++] = temp;
            } else {
                cmd->background = 1;
            }
        } else {
            cmd->argv[cmd->argc++] = token;
        }
        
        /* read next token */
        token = strtok_r(NULL, " ", &saveptr);
    }
    
    /* set the final argv to NULL */
    cmd->argv[cmd->argc] = NULL;
}

int runCommand(Command* cmd, pid_t* cpid) {
	*cpid = fork();

    int status = 0;
    
    if (*cpid == 0) {
        // this is the child process--exec the specified program
        //printf("running command '%s'\n", cmd->argv[0]);
        
        if (cmd->infile != NULL) {
            cmd->infd = open(cmd->infile, O_RDONLY);
            if (dup2(cmd->infd, STDIN_FILENO) == -1) {
                fprintf(stderr, "smallsh: cannot open %s for input\n", cmd->infile);
                exit(EXIT_FAILURE);
            }
        } else if (cmd->background) {
			// redirect /dev/null to stdin if background process
			cmd->infd = open("/dev/null", O_RDONLY);
			if (dup2(cmd->infd, STDIN_FILENO) == -1) {
				fprintf(stderr, "smallsh: cannot open /dev/null for input\n");
				exit(EXIT_FAILURE);
			}
		}
        if (cmd->outfile != NULL) {
            cmd->outfd = open(cmd->outfile, 
                         O_WRONLY | O_CREAT | O_TRUNC, 
                         S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
            if (dup2(cmd->outfd, STDOUT_FILENO) == -1) {
                fprintf(stderr, "smallsh: cannot open %s for output\n", cmd->outfile);
                exit(EXIT_FAILURE);
            }
        } else if (cmd->background) {
			// background process--ignore SIGINT
			signal(SIGINT, SIG_IGN);

            // redirect stdout to /dev/null if background process
            cmd->outfd = open("/dev/null", O_WRONLY);
            if (dup2(cmd->outfd, STDOUT_FILENO) == -1) {
                fprintf(stderr, "smallsh: cannot open /dev/null for output\n");
                exit(EXIT_FAILURE);
            }
        }
        
        execvp(cmd->argv[0], cmd->argv);
        
        fprintf(stderr, "%s: no such file or directory\n", cmd->argv[0]);
        exit(EXIT_FAILURE);
    } else if (*cpid == -1) {
        printWarning("Failed to fork process.\n");
    } else {
        // this is the parent process--wait for child to terminate if not background
        if (!cmd->background) {
			while (waitpid(*cpid, &status, 0) < 0 && errno == EINTR) {
				continue;
			}
			if (WIFSIGNALED(status)) {
				printf("terminated by signal %d\n", WTERMSIG(status));
			}
            // close any open file descriptors
            if (cmd->infd >= 0)
                close(cmd->infd);
            if (cmd->outfd >= 0)
                close(cmd->outfd);
        } else {
			printf("background pid is %d\n", *cpid);

		}
    }
    
    return status;
}


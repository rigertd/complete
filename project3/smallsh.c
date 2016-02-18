/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 3 - smallsh
* File:         smallsh.c
* Description:  Implementation file for smallsh.h
\*********************************************************/

#include "smallsh.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/* These function calls will likely be needed:
    fork()
        - Both resulting processes have same open file descriptors
        - Variables are copied to child process (managed separately)
        - Each returns a different value from fork: 0 for child, pid of child for parent, -1 for error (sets errno)
        - pid_t for pid
    int execl(char *path, char *arg1, ...)
        - Replaces current program with new program (process)
        - Destroys the previous process
        - Can specify arguments that become argc/argv command line arguments
        - Executes program in path with specified args
        - Last arg must be NULL
    int execv(char *path, char *argv[])
        - Executes program in path and gives command line arguments in c-string array
        - First arg must be program path
        - Last element in argv must be NULL
    int execlp(char *path, char *arg1, ...)
    int execvp(char *path, char *argv[])
        - These search PATH for the executable
    atexit()
        - Arranges for function to be called before exit()
    exit()
        - Calls all functions registered by atexit()
        - Flushes all stdio output streams
        - Removes files created by tmpfile()
        - Calls _exit()
    _exit()
        - Closes all files
        - Cleans up everything--see man page for wait() for complete list
    pid_t wait(int &exitMethod)
        - Waits for any one child process to complete, then returns that pid
    pid_t waitpid(int pid, int &exitMethod, 0)
        - Waits until the specified PID process terminates
        - Specify -1 for pid to wait for any child process
        - waitpid(-1, &status, WNOHANG) to check if any process has completed
          and return immediately with 0 if none have
    pid_t getpid()
        - Returns the pid of the current process
    getpgrp()
        - 
    setpgid()
    setsid()
    getsid()
    char* getenv("MY_VAR")
        - Gets an environment variable
        - Can be used to pass info between shell and C program
    putenv("MY_VAR= Some text string 1234")
        - Adds an environment variable
    nice()
    char* strdup(const char*)
        - Returns a duplicate string allocated with malloc--must be free'd
        - To prevent stack strings from going out of scope (eg when adding to array)
    getcwd()
        - Gets the current working dir
    chdir()
        - Sets the current working dir

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
void spawnBgProcess();
void spawnFgProcess(char* argv[]);
void printFatalError(char*);
void printWarning(char*);
int getExitStatus(pid_t);

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
void spawnFgProcess(char* argv[]) {
    pid_t cpid = fork();
    
    if (cpid == 0) {
        // this is the child process--exec the specified program
        execv(argv[0], argv);
        
        printFatalError("Failed to spawn new foreground process.\n");
    } else if (cpid == -1) {
        printWarning("Failed to fork process.\n");
    }
}
/**
 *
 */
void parseCommand(Command* cmd, char* input) {
    if (cmd == 0) {
        //fputs_r("NULL Command object specified.\n", stderr);
        exit(1);
    }
}

void catchint() {
    char* msg = "Caught interrupt signal.\n";
    write(STDOUT_FILENO, msg, strlen(msg));
}

int getExitStatus(pid_t cpid) {
    int status;
    cpid = wait(&status);
    
    if (cpid == -1)
        printFatalError("Wait call failed.\n");
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else
        return -1;
}

void registerIntHandler(void (*handler)(int)) {
    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigfillset(&act.sa_mask);
    sigaction(SIGINT, &act, NULL);
}

// void fputs_r(const char* str, FILE* stream) {
    // sigset_t all, old, empty;
    // sigfillset(&all);
    // sigemptyset(&empty);
    // if (sigprocmask(SIG_BLOCK, &all, &old) < 0) {
        // fputs("Error blocking signals.\n", stderr);
        // exit(1);
    // }
    // fputs(str, stream);
    // sigsuspend(&empty);
    // if (sigprocmask(SIG_UNBLOCK, &old, NULL) < 0) {
        // fputs("Error unblocking signals.\n", stderr);
        // exit(1);
    // }
// }

void printFatalError(char* msg) {
    write(STDERR_FILENO, msg, strlen(msg));
    exit(1);
}

void printWarning(char* msg) {
    write(STDERR_FILENO, msg, strlen(msg));
}
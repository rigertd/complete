/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 3 - smallsh
* File:         smallsh.c
* Description:  Implementation file for smallsh.h
\*********************************************************/

#include "smallsh.h"
#include <errno.h>
#include <fcntl.h>  // for close-on-exec
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/*  File descriptors automatically opened by kernel:
        0 = stdin
        1 = stdout
        2 = stderr
    Creating a pipe:
        - Parent process creates a pipe, resulting in an input fd and an output fd
        - Parent forks (and possibly execs) and both parent and child have copies of fds
            - One reads from input, and the other writes to output
        
    Signals:
        SIGBUS = bus error (read invalid mem address), SIGSEGV = seg fault
        SIGILL = illegal machine instruction
        SIGFPE = floating point error
        SIGSYS = illegal system call
        SIGCHILD = child process terminated
        SIGTERM can be caught and ignored, but not SIGKILL
        SIGUSR1 and SIGUSR2 are for user-defined signals
        SIGQUIT
        SIGINT = interrupt signal (ctrl+c)
    struct sigaction
    {
        void (*sa_handler)(int);
        sigset_t sa_mask;
        int sa_flags;
        void (*sa_sigaction)(int, siginfo_t *, void *);
    }
    First member should be set to SIG_DFL, SIG_IGN, or pointer to function to call when signal is received.
    Second member tells which signals to block while sig handler is executing
    Third one can be SA_RESTHAND (reset handler to SIG_DFL) after signal is received and handled
        - SA_SIGINFO (tells kernel to call function specified in 4th member instead of first)
        - Set to 0 to set no flags.
        - Set to SA_RESTART to automatically restart system calls
    These function calls will likely be needed:
    sigemptyset(sigset_t*)
        - creates an empty sigset_t
    sigfillset(sigset_t*)
        - creates a full sigset_t
    sigaddset(sigset_t*, int)
        - adds the specified mask to a set
    sigdelset(sigset_t*, int)
        - removes the specified mask from a set
    int sigaction(int signo, struct sigaction *newact, struct sigaction *origact)
        - sets the signal handler to call when a signal occurs
        - Set third parameter to NULL if you don't want to store original action
    sigprocmask()
        - Blocks signals (delays) until unblocked
    int pause()
        - Blocks until any unblocked signal is received
        - When signal is received, pause returns -1 and sets errno to EINTR
    int raise(int sig)
        - Sends a signal to yourself
    unsigned int alarm(unsigned int seconds) 
        - Kernal sends SIGALARM after specified amount of time passes
        - returns immediately
        - can only have one alarm set at a time
        - sleep() calls alarm() followed by pause()
    ualarm()
    int mkfifo(const char* fifoName, 0644(perms))
        - returns new file descriptor
        - Creates a named pipe (FIFO), which is a special file in the file system
        - Must be at least one reader and one writer process before you can open a FIFO
        - Process A calls open(..., O_RDONLY), process B calls open(..., O_WRONLY)
        - open() call blocks until both processes call open
    pipe(int[2])
        - Stores fd in the argument array
        - First is input, second is output
        int r, pipeFDs[2];
        char message[512];
        pid_t cpid;
        
        if (pipe(pipeFDs) == -1) {
            exit(1);
        }
        cpid = fork();
        switch (cpid) {
            case 0: // child
            close(pipeFDs[0]); // close input fd
            write(pipeFDs[1], "message to parent from child", 41);
            exit(0);
            
            default: // parent
            close(pipeFDs[1]); // close output fd
            r = read(pipeFDs[0], message, sizeof(message)); // message from child
            exit(0)
        }
    dup2(fd_a, fd_b)
        - Duplicates fd_b to point to same target as fd_a (both point to fd_a)
        - fd2 = dup2(fd, 1) makes stdout point to same file as fd
        - If fd2 == -1, dup2 call failed
        - fd2 = dup2(fd, 0) makes stdin point to same file as fd (gets input from that file)
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
    fcntl(fd, flags, int)
        - FD_SETFD flag = close-on-exec
        
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
    int read(int fd, char* buf, int sizeToRead)
        - Returns number of bytes read
        - Succeeds if data was available to read
        - Blocks until data is available in pipe
        - If output pipe is closed, read() returns 0
    write()
        - Blocks until all data has been written
        - If input pipe is closed, write() returns -1 and errno set to EPIPE
        - also receives SIGPIPE signal
*/
char* strtok_r(
    char *str, 
    const char *delim, 
    char **nextp)
{
    char *ret;

    if (str == NULL)
    {
        str = *nextp;
    }

    str += strspn(str, delim);

    if (*str == '\0')
    {
        return NULL;
    }

    ret = str;

    str += strcspn(str, delim);

    if (*str)
    {
        *str++ = '\0';
    }

    *nextp = str;

    return ret;
}
/*========================================================*
 * Macro definitions
 *
 * Define constants here for easy modification and to avoid magic numbers.
 *========================================================*/
#define MAX_CMD_LINE_LEN 2048
#define MAX_ARGS 512


/*========================================================*
 * Structs and enumerations
 *========================================================*/
/**
 *
 */
typedef struct Command {
    char buffer[MAX_CMD_LINE_LEN];
    int argc;
    char* argv[MAX_ARGS];
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
void readLine(int, char*, size_t);

/*========================================================*
 * Constant definitions
 *========================================================*/

/*========================================================*
 * main function
 *========================================================*/
int main(int argc, char* argv[]) {
    /* start by registering signal handler for CTRL+C*/
    // registerIntHandler(catchint);
    
    char inputBuffer[MAX_CMD_LINE_LEN];
    Command cmd;
    
    do {
        write(STDOUT_FILENO, ": ", 2);
        readLine(STDIN_FILENO, inputBuffer, MAX_CMD_LINE_LEN);
        parseCommand(&cmd, inputBuffer);
    } while (cmd.argc > 0 && strcmp(cmd.argv[0], "exit") != 0);
    
    return EXIT_SUCCESS;
}

/*========================================================*
 * Function definitions
 *========================================================*/
void parseCommand(Command* cmd, char* input) {
    char *saveptr, *token, *temp;
    
    /* initialize structure members */
    strcpy(cmd->buffer, input);
    cmd->background = 0;
    cmd->argc = 0;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    
    /* parse input until EOF is reached */
    do {
    token = strtok_r(cmd->buffer, " ", &saveptr);
    
    if (strcmp(token, "<") == 0) {
        cmd->infile = strtok_r(cmd->buffer, " ", &saveptr);
    } else if (strcmp(token, ">") == 0) {
        cmd->outfile = strtok_r(cmd->buffer, " ", &saveptr);
    } else if (strcmp(token, "&") == 0) {
        temp = strtok_r(cmd->buffer, " ", &saveptr);
        if (temp != NULL) {
            cmd->argv[cmd->argc++] = token;
            cmd->argv[cmd->argc++] = temp;
        } else {
            cmd->background = 1;
        }
    } else {
        cmd->argv[cmd->argc++] = token;
    }
    } while (token != NULL);
}
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
    exit(EXIT_FAILURE);
}

void printWarning(char* msg) {
    write(STDERR_FILENO, msg, strlen(msg));
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
        fprintf(stderr, "Error reading from file. (errno %d)", errno);
        exit(1);
    }
    // search string for newline and replace with NULL terminator,
    // then count until non-whitespace char is found
    // and set the file offset to that character.
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

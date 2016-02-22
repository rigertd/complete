#ifndef BG_PROCESS_H
#define BG_PROCESS_H
typedef struct BgProcess {
    pid_t id;
    int inFd;
    int outFd;
} BgProcess;

#endif
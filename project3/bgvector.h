/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 3 - smallsh
* File:         bgvector.c
* Description:  Defines a custom vector structure for storing
*               background process IDs and file descriptors.
*               This structure and associated functions use
*               dynamic memory allocation and are thread-safe.
\*********************************************************/
#ifndef BG_VECTOR_H
#define BG_VECTOR_H

#include <unistd.h>
typedef struct BgProcess {
    pid_t id;
    int inFd;
    int outFd;
} BgProcess;

typedef struct BgProcessVector {
    unsigned int size;
    unsigned int capacity;
    BgProcess* arr;
    int lock;
} BgProcessVector;

void initBgProcessVector(BgProcessVector*, unsigned int);
void finalizeIntVector(BgProcessVector*);

void pushBackBgProcessVector(BgProcessVector*, BgProcess);
void popBackBgProcessVector(BgProcessVector*);
void removeIndexBgProcessVector(BgProcessVector*, unsigned int);
BgProcess getAtBgProcessVector(BgProcessVector*, unsigned int);

#endif
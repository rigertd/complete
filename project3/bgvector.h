#ifndef BG_VECTOR_H
#define BG_VECTOR_H

#include "bgprocess.h"

typedef struct BgProcessVector {
    unsigned int size;
    unsigned int capacity;
    BgProcess* arr;
    int lock;
} IntVector;

void initBgProcessVector(BgProcessVector*, unsigned int);
void finalizeIntVector(BgProcessVector*);

void pushBackBgProcessVector(BgProcessVector*, BgProcess);
void popBackBgProcessVector(BgProcessVector*);
void removeIndexBgProcessVector(BgProcessVector*, unsigned int);
int getAtBgProcessVector(BgProcessVector*, unsigned int);

#endif
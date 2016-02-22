#include "bgvector.h"
#include <unistd.h>
#include <stdio.h>

void initBgProcessVector(BgProcessVector *vec, unsigned int capacity) {
    vec->size = 0;
    vec->capacity = capacity;
    vec->arr = malloc(sizeof(BgProcess) * capacity);
    vec->lock = 0;
}

void finalizeBgProcessVector(BgProcessVector* vec) {
    if (vec->arr != NULL) free(vec->arr);
}

void pushBackBgProcessVector(BgProcessVector* vec, BgProcess value) {
    // use a counting semaphore for critical section
    // attempt to get a lock and sleep for 10 usecs until one is acquired
    while (++vec->lock != 1) {
        --vec->lock;
        usleep(10);
    }
    int* tempArr;
    
    if (vec->capacity <= vec->size) {
        tempArr = malloc(sizeof(BgProcess) * vec->capacity * 2);
        memcpy(tempArr, vec->arr, vec->size);
        free(vec->arr);
        vec->arr = tempArr;
        vec->capacity *= 2;
    }
    
    vec->arr[vec->size++] = value;
    
    // release semaphore
    --vec->lock;
}

void popBackBgProcessVector(BgProcessVector* vec) {
    // no lock necessary--decrementing is an atomic operation
    --vec->size;
}

void removeAtBgProcessVector(BgProcessVector* vec, unsigned int index) {
    int i;
    // use a counting semaphore for critical section
    // attempt to get a lock and sleep for 10 usecs until one is acquired
    while (++vec->lock != 1) {
        --vec->lock;
        usleep(10);
    }
    
    for (i = index; i < vec->size - 1; ++i) {
        vec->arr[i] = vec->arr[i + 1];
    }
    --vec->size;
    
    // release semaphore
    --vec->lock;
}

BgProcess getAtBgProcessVector(BgProcessVector* vec, unsigned int index) {
    if (index < 0 || index > vec->size) {
        fprintf(stderr, "Illegal index %d specified\n", index);
        exit(1);
    }
    return vec->arr[index];
}

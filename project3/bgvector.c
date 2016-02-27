/*********************************************************\
* Author:       David Rigert
* Class:        CS344 Winter 2016
* Assignment:   Program 3 - smallsh
* File:         bgvector.c
* Description:  Implementation file for bgvector.h
\*********************************************************/
#include "bgvector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Initializes the specified BgProcessVector structure.
 *
 * This function initializes the values and allocates enough memory for 
 * the specified capacity. If memory allocation fails, this function
 * displays an error and exits the program.
 * You must call finalizeBgProcessVector to finalize the structure.
 *
 *  vec         The BgProcessVector to initialize.
 *  capacity    The initial capacity of the vector.
 */
void initBgProcessVector(BgProcessVector *vec, unsigned int capacity) {
    vec->size = 0;
    vec->capacity = capacity;
    vec->arr = malloc(sizeof(BgProcess) * capacity);
    if (vec->arr == NULL) {
        fprintf(stderr, "Failed to allocate memory for BgProcessVector\n");
        exit(EXIT_FAILURE);
    }
    vec->lock = 0;
}

/**
 * Finalizes the specified BgProcessVector structure.
 *
 * This function frees the memory allocated for the specified BgProcessVector.
 * The BgProcessVector can no longer be used after calling this function
 * unless initBgProcessVector is called again.
 *
 *  vec         The BgProcessVector to finalize.
 */
void finalizeBgProcessVector(BgProcessVector* vec) {
    if (vec->arr != NULL) {
        free(vec->arr);
        vec->arr = NULL;
    }
}

/**
 * Adds the specified value to the end of the vector in a thread-safe manner.
 * This function doubles the capacity of the BgProcessVector if there is
 * insufficient space to store the value.
 *
 *  vec     The BgProcessVector to add the value to.
 *  value   The BgProcess struct value to add to the vector.
 */
void pushBackBgProcessVector(BgProcessVector* vec, BgProcess value) {
    /* Use a counting semaphore for critical section
       attempt to get a lock and sleep for 10 usecs until one is acquired */
    int i = 0;
    while (++vec->lock != 1) {
        --vec->lock;
        ++i;
        usleep(10);
        if (i > 6000) {
            /* If we wait a full second, timeout to avoid deadlocks */
            fprintf(stderr, "BgProcessVector cannot acquire lock\n");
            exit(EXIT_FAILURE);
        }
    }
    BgProcess* tempArr;
    
    /* Check if vector needs resizing */
    if (vec->capacity <= vec->size) {
        tempArr = malloc(sizeof(BgProcess) * vec->capacity * 2);
        if (tempArr == NULL) {
            fprintf(stderr, "Failed to allocate memory for BgProcessVector\n");
            exit(EXIT_FAILURE);
        }
        memcpy(tempArr, vec->arr, vec->size);
        free(vec->arr);
        vec->arr = tempArr;
        vec->capacity *= 2;
    }
    
    vec->arr[vec->size++] = value;
    
    /* Release semaphore */
    --vec->lock;
}

/**
 * Removes the last value in the specified vector.
 * This function does not free any memory or reduce capacity.
 *
 *  vec     The BgProcessVector to remove the value from.
 */
void popBackBgProcessVector(BgProcessVector* vec) {
    /* No lock necessary--decrementing is an atomic operation */
    --vec->size;
}

/**
 * Removes the value at the specified index in a thread-safe manner.
 * This function does not free any memory or reduce capacity.
 *
 *  vec     The BgProcessVector to remove the value from.
 *  index   The index of the value to remove.
 */
void removeIndexBgProcessVector(BgProcessVector* vec, unsigned int index) {
    int i = 0;
    /* Use a counting semaphore for critical section
       attempt to get a lock and sleep for 10 usecs until one is acquired */
    while (++vec->lock != 1) {
        --vec->lock;
        ++i;
        usleep(10);
        if (i > 6000) {
            /* If we wait a full second, timeout to avoid deadlocks */
            fprintf(stderr, "BgProcessVector cannot acquire lock\n");
            exit(EXIT_FAILURE);
        }
    }
    
    for (i = index; i < vec->size - 1; ++i) {
        vec->arr[i] = vec->arr[i + 1];
    }
    --vec->size;
    
    /* Release semaphore */
    --vec->lock;
}

/**
 * Gets the value at the specified index in the specified BgProcessVector.
 * This function throws an error if the index is out of bounds.
 *
 *  vec     The BgProcessVector to get the value from.
 *  index   The index of the value to get.
 */
BgProcess getAtBgProcessVector(BgProcessVector* vec, unsigned int index) {
    if (index < 0 || index > vec->size) {
        fprintf(stderr, "Illegal index %d specified\n", index);
        exit(EXIT_FAILURE);
    }
    return vec->arr[index];
}

#ifndef __TODOLIST_H
#define __TODOLIST_H

#include "dynamicArrayXC.h"
#include "typeXC.h"

Task* createTask (int priority, char *desc);

void saveList(DynArr *heap, FILE *filePtr);

void loadList(DynArr *heap, FILE *filePtr);

void printList(DynArr *heap);

void deleteList(DynArr *heap);

int compare(void *left, void *right);

void print_type(void *curval);

#endif

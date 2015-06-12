/* 	dynArr.h : Dynamic Array implementation. */
#ifndef DYNAMIC_ARRAY_INCLUDED
#define DYNAMIC_ARRAY_INCLUDED 1

/* function used to compare two user-defined values to each other */
typedef int (*comparator)(void *left, void *right);
/* function used to print a user-defined value */
typedef void (*printer)(void *val);

typedef struct DynArr DynArr;
struct bag;

/* Dynamic Array Functions */
DynArr *createDynArr(int cap);
void deleteDynArr(DynArr *v);

int sizeDynArr(DynArr *v);

void addDynArr(DynArr *v, void *val);
void *getDynArr(DynArr *v, int pos);
void putDynArr(DynArr *v, int pos, void *val);
void swapDynArr(DynArr *v, int i, int  j);
void removeAtDynArr(DynArr *v, int idx);

/* Stack interface. */
int isEmptyDynArr(DynArr *v);
void pushDynArr(DynArr *v, void *val);
void *topDynArr(DynArr *v);
void popDynArr(DynArr *v);

/* Bag Interface */
int containsDynArr(DynArr *v, void *val, comparator compare);
void removeDynArr(DynArr *v, void *val, comparator compare);


/* Heap-based Priority Queue Interface */
void *getMinHeap(DynArr *heap);
void addHeap(DynArr *heap, void *node, comparator compare);
void removeMinHeap(DynArr *heap, comparator compare);
void sortHeap(DynArr *heap, comparator compare);


/* Utility function*/
void copyDynArr(DynArr *source, DynArr *destination);


#endif

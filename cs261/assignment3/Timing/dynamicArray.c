/* CS261- Assignment 2 - dynamicArray.c
 * Name:        David Rigert
 * Description: Implementation of a dynamically-resized array structure
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "dynamicArray.h"

struct DynArr
{
	TYPE *data;		/* pointer to the data array */
	int size;		/* Number of elements in the array */
	int capacity;	/* capacity ofthe array */
};


/* ************************************************************************
	Dynamic Array Functions
************************************************************************ */

/* Initialize (including allocation of data array) dynamic array.

	param: 	v		pointer to the dynamic array
	param:	cap 	capacity of the dynamic array
	pre:	v is not null
	post:	internal data array can hold cap elements
	post:	v->data is not null
*/
void initDynArr(DynArr *v, int capacity)
{
	assert(capacity > 0);
	assert(v!= 0);
	v->data = (TYPE *) malloc(sizeof(TYPE) * capacity);
	assert(v->data != 0);
	v->size = 0;
	v->capacity = capacity;	
}

/* Allocate and initialize dynamic array.

	param:	cap 	desired capacity for the dyn array
	pre:	none
	post:	none
	ret:	a non-null pointer to a dynArr of cap capacity
			and 0 elements in it.		
*/
DynArr* createDynArr(int cap)
{
	assert(cap > 0);
	DynArr *r = (DynArr *)malloc(sizeof( DynArr));
	assert(r != 0);
	initDynArr(r,cap);
	return r;
}

/* Deallocate data array in dynamic array. 

	param: 	v		pointer to the dynamic array
	pre:	none
	post:	d.data points to null
	post:	size and capacity are 0
	post:	the memory used by v->data is freed
*/
void freeDynArr(DynArr *v)
{
	if(v->data != 0)
	{
		free(v->data); 	/* free the space on the heap */
		v->data = 0;   	/* make it point to null */
	}
	v->size = 0;
	v->capacity = 0;
}

/* Deallocate data array and the dynamic array ure. 

	param: 	v		pointer to the dynamic array
	pre:	none
	post:	the memory used by v->data is freed
	post:	the memory used by d is freed
*/
void deleteDynArr(DynArr *v)
{
	freeDynArr(v);
	free(v);
}

/* Resizes the underlying array to be the size cap 

	param: 	v		pointer to the dynamic array
	param:	cap		the new desired capacity
	pre:	v is not null
	post:	v has capacity newCap
*/
void _dynArrSetCapacity(DynArr *v, int newCap)
{	
	/* check if v is null */
	assert(v != 0);
    /* check for valid capacity */
    assert(newCap > 0);
    
    /* declare new array with size of newCap */
    TYPE *new_data = (TYPE *)malloc(newCap * sizeof(TYPE));
    /* make sure memory was allocated */
    assert(new_data != 0);
    
    /* copy old array to new array */
    memcpy(new_data, v->data, v->size * sizeof(TYPE));
    
    /* delete old array */
    free(v->data);
    
    /* set pointer to new array */
    v->data = new_data;
    
    /* update capacity */
    v->capacity = newCap;
}

/* Get the size of the dynamic array

	param: 	v		pointer to the dynamic array
	pre:	v is not null
	post:	none
	ret:	the size of the dynamic array
*/
int sizeDynArr(DynArr *v)
{
	return v->size;
}

/* 	Adds an element to the end of the dynamic array

	param: 	v		pointer to the dynamic array
	param:	val		the value to add to the end of the dynamic array
	pre:	the dynArry is not null
	post:	size increases by 1
	post:	if reached capacity, capacity is doubled
	post:	val is in the last utilized position in the array
*/
void addDynArr(DynArr *v, TYPE val)
{
	/* verify precondition (v is not null) */
    assert(v != 0);
    
    /* check capacity and double if full */
    if (v->capacity <= v->size) {
        _dynArrSetCapacity(v, v->capacity * 2);
    }
    
    /* add value to array and increment by 1*/
    v->data[v->size++] = val;
}

/*	Get an element from the dynamic array from a specified position
	
	param: 	v		pointer to the dynamic array
	param:	pos		integer index to get the element from
	pre:	v is not null
	pre:	v is not empty
	pre:	pos < size of the dyn array and >= 0
	post:	no changes to the dyn Array
	ret:	value stored at index pos
*/

TYPE getDynArr(DynArr *v, int pos)
{
	/* verify preconditions */
    assert(v != 0);                           /* v is not null */
    assert(sizeDynArr(v) > 0);                /* v is not empty */
    assert(pos >= 0 && pos < sizeDynArr(v));  /* pos is valid index */

	/* return value in specified position */
	return v->data[pos]; 
}

/*	Put an item into the dynamic array at the specified location,
	overwriting the element that was there

	param: 	v		pointer to the dynamic array
	param:	pos		the index to put the value into
	param:	val		the value to insert 
	pre:	v is not null
	pre:	v is not empty
	pre:	pos >= 0 and pos < size of the array
	post:	index pos contains new value, val
*/
void putDynArr(DynArr *v, int pos, TYPE val)
{
	/* verify preconditions */
    assert(v != 0);                           /* v is not null */
    assert(sizeDynArr(v) > 0);                /* v is not empty */
    assert(pos >= 0 && pos < sizeDynArr(v));  /* pos is valid index */
    
    /* set element with index of 'pos' to 'val' */
    v->data[pos] = val;
}

/*	Swap two specified elements in the dynamic array

	param: 	v		pointer to the dynamic array
	param:	i,j		the elements to be swapped
	pre:	v is not null
	pre:	v is not empty
	pre:	i, j >= 0 and i,j < size of the dynamic array
	post:	index i now holds the value at j and index j now holds the value at i
*/
void swapDynArr(DynArr *v, int i, int  j)
{
	/* verify preconditions */
    assert(v != 0);                     /* v is not null */
    assert(sizeDynArr(v) > 0);          /* v is not empty */
    
    /* assign first value to temp variable */
    TYPE temp = getDynArr(v, i);
    /* assign first value to second */
    putDynArr(v, i, getDynArr(v, j));
    /* assign second value to first (in temp) */
    putDynArr(v, j, temp);
}

/*	Remove the element at the specified location from the array,
	shifts other elements back one to fill the gap

	param: 	v		pointer to the dynamic array
	param:	idx		location of element to remove
	pre:	v is not null
	pre:	v is not empty
	pre:	idx < size and idx >= 0
	post:	the element at idx is removed
	post:	the elements past idx are moved back one
*/
void removeAtDynArr(DynArr *v, int idx)
{
	/* verify preconditions */
    assert(v != 0);                     /* v is not null */
    assert(v->size > 0);                /* v is not empty */
    assert(idx >= 0 && idx < v->size);  /* idx is valid index */
    
    /* shift every element from idx+1 until end of array to left by 1*/
    int i;
    for (i = idx + 1; i < v->size; ++i) {
        putDynArr(v, i - 1, getDynArr(v, i));
    }
    
    /* decrement size by 1 */
    v->size--;
}



/* ************************************************************************
	Stack Interface Functions
************************************************************************ */

/*	Returns boolean (encoded in an int) demonstrating whether or not the 
	dynamic array stack has an item on it.

	param:	v		pointer to the dynamic array
	pre:	the dynArr is not null
	post:	none
	ret:	1 if empty, otherwise 0
*/
int isEmptyDynArr(DynArr *v)
{
	/* verify precondition that v is not null */
    assert (v != 0);
    
	/* return whether size equals 0*/
	return sizeDynArr(v) == 0;
}

/* 	Push an element onto the top of the stack

	param:	v		pointer to the dynamic array
	param:	val		the value to push onto the stack
	pre:	v is not null
	post:	size increases by 1
			if reached capacity, capacity is doubled
			val is on the top of the stack
*/
void pushDynArr(DynArr *v, TYPE val)
{
	/* verify precondition that v is not null */
    assert (v != 0);
    
    /* add value to end of array */
    addDynArr(v, val);
}

/*	Returns the element at the top of the stack 

	param:	v		pointer to the dynamic array
	pre:	v is not null
	pre:	v is not empty
	post:	no changes to the stack
*/
TYPE topDynArr(DynArr *v)
{
	/* verify preconditions */
    assert(v != 0);                     /* v is not null */
	
	/* return last value in array */
	return getDynArr(v, sizeDynArr(v) - 1);
}

/* Removes the element on top of the stack 

	param:	v		pointer to the dynamic array
	pre:	v is not null
	pre:	v is not empty
	post:	size is decremented by 1
			the top has been removed
*/
void popDynArr(DynArr *v)
{
	/* verify preconditions */
    assert(v != 0);                     /* v is not null */
    assert(sizeDynArr(v) > 0);          /* v is not empty */
    
    /* remove last element in array */
    removeAtDynArr(v, sizeDynArr(v) - 1);
}

/* ************************************************************************
	Bag Interface Functions
************************************************************************ */

/*	Returns boolean (encoded as an int) demonstrating whether or not
	the specified value is in the collection
	true = 1
	false = 0

	param:	v		pointer to the dynamic array
	param:	val		the value to look for in the bag
	pre:	v is not null
	pre:	v is not empty
	post:	no changes to the bag
*/
int containsDynArr(DynArr *v, TYPE val)
{
	/* verify preconditions */
    assert(v != 0);                     /* v is not null */
    assert(sizeDynArr(v) > 0);          /* v is not empty */
	
	/* loop through array until value is found (or not) */
    int i;
    for (i = 0; i < sizeDynArr(v); ++i) {
        if (EQ(v->data[i], val)) {
            return 1;
        }
    }
    
    /* array does not contain value if execution reaches here */
	return 0;
}

/*	Removes the first occurrence of the specified value from the collection
	if it occurs

	param:	v		pointer to the dynamic array
	param:	val		the value to remove from the array
	pre:	v is not null
	pre:	v is not empty
	post:	val has been removed
	post:	size of the bag is reduced by 1
*/
void removeDynArr(DynArr *v, TYPE val)
{
	/* verify preconditions */
    assert(v != 0);                     /* v is not null */
    assert(sizeDynArr(v) > 0);          /* v is not empty */
    
    /* loop through array from start and remove first value if found */
    int i;
    for (i = 0; i < sizeDynArr(v); ++i) {
        if (EQ(getDynArr(v, i), val)) {
            removeAtDynArr(v, i);
            /* exit loop after removing first occurrence */
            break;
        }
    }
}

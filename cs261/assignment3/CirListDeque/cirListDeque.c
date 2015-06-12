/* CS261- Assignment 3 - linkedList.c
 * Name:        David Rigert
 * Description: Implementation of a circularly-linked list deque
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include "cirListDeque.h"

/* Double Link Struture */
struct DLink {
    TYPE value;/* value of the link */
    struct DLink * next;/* pointer to the next link */
    struct DLink * prev;/* pointer to the previous link */
};

# define TYPE_SENTINEL_VALUE DBL_MAX 


/* ************************************************************************
 Deque ADT based on Circularly-Doubly-Linked List WITH Sentinel
 ************************************************************************ */

struct cirListDeque {
    int size;/* number of links in the deque */
    struct DLink *Sentinel; /* pointer to the sentinel */
};
/* internal functions prototypes */
struct DLink* _createLink (TYPE val);
void _addLinkAfter(struct cirListDeque *q, struct DLink *lnk, TYPE v);
void _removeLink(struct cirListDeque *q, struct DLink *lnk);



/* ************************************************************************
    Deque Functions
************************************************************************ */

/* Initialize deque.

    param:  q       pointer to the deque
    pre:    q is not null
    post:   q->Sentinel is allocated and q->size equals zero
*/
void _initCirListDeque (struct cirListDeque *q) 
{
    /* verify precondition */
    assert(q != 0);
    
    /* allocate memory for sentinel */
    q->Sentinel = malloc(sizeof(struct DLink));
    assert(q->Sentinel != 0);
    
    /* point sentinel to itself */
    q->Sentinel->next = q->Sentinel;
    q->Sentinel->prev = q->Sentinel;
    
    /* initialize size of deque to 0 */
    q->size = 0;
}

/*
    create a new circular list deque
 */
struct cirListDeque *createCirListDeque()
{
    struct cirListDeque *newCL = malloc(sizeof(struct cirListDeque));
    _initCirListDeque(newCL);
    return(newCL);
}


/* Create a link for a value.

    param:  val     the value to create a link for
    pre:    none
    post:   a link to store the value
*/
struct DLink * _createLink (TYPE val)
{
    /* allocate memory for link */
    struct DLink *newLink = malloc(sizeof(struct DLink));
    assert(newLink != 0);
    
    /* set value in new link */
    newLink->value = val;

    /* return a pointer to the new link */
    return newLink;
}

/* Adds a link after another link

    param:  q       pointer to the deque
    param:  lnk     pointer to the existing link in the deque
    param:  v       value of the new link to be added after the existing link
    pre:    q is not null
    pre:    lnk is not null
    pre:    lnk is in the deque 
    post:   the new link is added into the deque after the existing link
*/
void _addLinkAfter(struct cirListDeque *q, struct DLink *lnk, TYPE v)
{
    /* verify preconditions */
    assert(q != 0);
    assert(lnk != 0);
    
    struct DLink *newLink = _createLink(v);
    /* insert newLink after lnk */
    newLink->next = lnk->next;
    newLink->prev = lnk;
    lnk->next->prev = newLink;
    lnk->next = newLink;
    
    /* increment size */
    q->size++;
}

/* Adds a link to the back of the deque

    param:  q       pointer to the deque
    param:  val     value for the link to be added
    pre:    q is not null
    post:   a link storing val is added to the back of the deque
*/
void addBackCirListDeque (struct cirListDeque *q, TYPE val) 
{
    /* verify precondition */
    assert(q != 0);
    
    /* add val after prev of sentinel */
    _addLinkAfter(q, q->Sentinel->prev, val);
}

/* Adds a link to the front of the deque

    param:  q       pointer to the deque
    param:  val     value for the link to be added
    pre:    q is not null
    post:   a link storing val is added to the front of the deque
*/
void addFrontCirListDeque(struct cirListDeque *q, TYPE val)
{
    /* verify precondition */
    assert(q != 0);
    
    /* add val after sentinel */
    _addLinkAfter(q, q->Sentinel, val);
}

/* Get the value of the front of the deque

    param:  q       pointer to the deque
    pre:    q is not null and q is not empty
    post:   none
    ret:    value of the front of the deque
*/
TYPE frontCirListDeque(struct cirListDeque *q) 
{
    /* verify preconditions */
    assert(q != 0);
    assert(!isEmptyCirListDeque(q));
    
    /* return value of link after sentinel */
    return(q->Sentinel->next->value);
}

/* Get the value of the back of the deque

    param:  q       pointer to the deque
    pre:    q is not null and q is not empty
    post:   none
    ret:    value of the back of the deque
*/
TYPE backCirListDeque(struct cirListDeque *q)
{
    /* verify preconditions */
    assert(q != 0);
    assert(!isEmptyCirListDeque(q));
    
    /* return value of link before sentinel */
    return(q->Sentinel->prev->value);
}

/* Remove a link from the deque

    param:  q       pointer to the deque
    param:  lnk     pointer to the link to be removed
    pre:    q is not null and q is not empty
    post:   the link is removed from the deque
*/
void _removeLink(struct cirListDeque *q, struct DLink *lnk)
{
    /* verify preconditions */
    assert(q != 0);
    assert(!isEmptyCirListDeque(q));
    
    /* remove lnk from list */
    lnk->prev->next = lnk->next;
    lnk->prev->next->prev = lnk->prev;
    free(lnk);
    
    /* decrement counter and return */
    q->size--;
}

/* Remove the front of the deque

    param:  q       pointer to the deque
    pre:    q is not null and q is not empty
    post:   the front is removed from the deque
*/
void removeFrontCirListDeque (struct cirListDeque *q) {
    /* verify preconditions */
    assert(q != 0);
    assert(!isEmptyCirListDeque(q));
    
    /* remove link after sentinel */
    _removeLink(q, q->Sentinel->next);
}


/* Remove the back of the deque

    param:  q       pointer to the deque
    pre:    q is not null and q is not empty
    post:   the back is removed from the deque
*/
void removeBackCirListDeque(struct cirListDeque *q)
{
    /* verify preconditions */
    assert(q != 0);
    assert(!isEmptyCirListDeque(q));
    
    /* remove link before sentinel */
    _removeLink(q, q->Sentinel->prev);
}

/* De-allocate all links of the deque

    param:  q       pointer to the deque
    pre:    q is not null
    post:   All links (including Sentinel) are de-allocated
*/
void freeCirListDeque(struct cirListDeque *q)
{
    /* verify precondition */
    assert(q != 0);
    
    /* loop through and remove every link */
    while (q->Sentinel->next != q->Sentinel) {
        _removeLink(q, q->Sentinel->next);
    }
    
    /* remove the sentinel */
    free(q->Sentinel);
    q->Sentinel = 0;
}

/* Check whether the deque is empty

    param:  q       pointer to the deque
    pre:    q is not null
    ret:    1 if the deque is empty. Otherwise, 0.
*/
int isEmptyCirListDeque(struct cirListDeque *q) {
    /* verify precondition */
    assert(q != 0);
    
    /* should I compare sentinel or just use size? */
    return q->size < 1;
}

/* Print the links in the deque from front to back

    param:  q       pointer to the deque
    pre:    q is not null and q is not empty
    post:   the links in the deque are printed from front to back
*/
void printCirListDeque(struct cirListDeque *q)
{
    /* verify preconditions */
    assert(q != 0);
    assert(!isEmptyCirListDeque(q));
    
    /* print start of list notation */
    printf("[");
    
    /* set currentLink to first link in list */
    struct DLink *currentLink = q->Sentinel->next;
    
    /* print fencepost if list is not empty */
    printf("%g", currentLink->value);
    currentLink = currentLink->next;
    
    /* keep printing until currentLink reaches Sentinel */
    while (currentLink != q->Sentinel) {
        printf(", %g", currentLink->value);
        currentLink = currentLink->next;
    }
    
    /* print end of list notation */
    printf("]\n");
}

/* Reverse the deque

    param:  q       pointer to the deque
    pre:    q is not null and q is not empty
    post:   the deque is reversed
*/
void reverseCirListDeque(struct cirListDeque *q)
{
    /* verify preconditions */
    assert(q != 0);
    assert(!isEmptyCirListDeque(q));
    
    /* first reverse Sentinel next and prev */
    struct DLink *temp = q->Sentinel->next;
    q->Sentinel->next = q->Sentinel->prev;
    q->Sentinel->prev = temp;
    
    /* traverse list and reverse all next and prev pointers */
    struct DLink *current = temp;
    while (current != q->Sentinel) {
        temp = current->next;
        current->next = current->prev;
        current->prev = temp;
        
        /* go to next link */
        current = temp;
    }
}

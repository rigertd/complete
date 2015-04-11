/* CS261- Assignment 3 - linkedList.c
 * Name:        David Rigert
 * Description: Implementation of a doubly-linked list structure
 */

#include "linkedList.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


/* Double Link*/
struct DLink {
    TYPE value;
    struct DLink * next;
    struct DLink * prev;
};

/* Double Linked List with Head and Tail Sentinels  */
struct linkedList{
    int size;
    struct DLink *firstLink;
    struct DLink *lastLink;
};

/*
    initList
    param lst   the linkedList
    pre: lst is not null
    post: lst size is 0
*/
void _initList (struct linkedList *lst) {
    /* Allocate memory for head and tail sentinels */
    struct DLink *head = malloc(sizeof(struct DLink));
    struct DLink *tail = malloc(sizeof(struct DLink));

    /* verify that malloc succeeded */
    assert(head != 0 && tail != 0);

    /* configure head and tail sentinels */
    head->next = tail;    /* points to tail */
    head->prev = 0;       /* points to null */
    tail->prev = head;    /* points to head */
    tail->next = 0;       /* points to null */

    /* set size and sentinels in linked list */
    lst->size = 0;
    lst->firstLink = head;
    lst->lastLink = tail;
}

/*
    createLinkedList
    param: none
    pre: none
    post: firstLink and lastLink reference sentinels
 */
struct linkedList *createLinkedList()
{
    struct linkedList *newList = malloc(sizeof(struct linkedList));
    _initList(newList);
    return(newList);
}

/*
    _addLinkBefore
    param: lst  the linkedList
    param: l    the link to add before
    param: v    the value to add
    pre: lst is not null
    pre: l is not null
    post: lst is not empty
*/

/* Adds Before the provided link, l */
void _addLinkBefore(struct linkedList *lst, struct DLink *l, TYPE v)
{
    /* verify preconditions */
    assert(lst != 0);
    assert(l != 0);
    
    /* allocate new link and verify that malloc succeeded */
    struct DLink *newLink = malloc(sizeof(struct DLink));
    assert(newLink != 0);
    
    /* set newLink */
    newLink->value = v;
    newLink->prev = l->prev;    /* set prev to DLink before l */
    newLink->next = l;          /* set next to l */
    
    /* set next of DLink before l */
    l->prev->next = newLink;
    
    /* set prev of l */
    l->prev = newLink;
    
    /* increment size of lst by 1 */
    lst->size++;
}


/*
    addFrontList
    param: lst  the linkedList
    param: e    the element to be added
    pre: lst is not null
    post: lst is not empty, increased size by 1
*/
void addFrontList(struct linkedList *lst, TYPE e)
{
    /* verify precondition */
    assert(lst != 0);
    
    /* add e to front of list (after head sentinel) */
    _addLinkBefore(lst, lst->firstLink->next, e);
}

/*
    addBackList
    param: lst  the linkedList
    param: e    the element to be added
    pre: lst is not null
    post: lst is not empty, increased size by 1
*/
void addBackList(struct linkedList *lst, TYPE e) {
    /* verify precondition */
    assert(lst != 0);
    
    /* add e to back of list (before tail sentinel) */
    _addLinkBefore(lst, lst->lastLink, e);
}

/*
    frontList
    param: lst  the linkedList
    pre: lst is not null
    pre: lst is not empty
    post: none
*/
TYPE frontList (struct linkedList *lst) {
    /* verify preconditions */
    assert(lst != 0);           /* not null */
    assert(!isEmptyList(lst));  /* not empty */
    
    /* return the value of the first link after the head sentinel */
    return lst->firstLink->next->value;
}

/*
    backList
    param: lst  the linkedList
    pre: lst is not null
    pre: lst is not empty
    post: lst is not empty
*/
TYPE backList(struct linkedList *lst)
{
    /* verify preconditions */
    assert(lst != 0);           /* not null */
    assert(!isEmptyList(lst));  /* not empty */
    
    /* return the value of the first link before the tail sentinel */
    return lst->lastLink->prev->value;
}

/*
    _removeLink
    param: lst  the linkedList
    param: l    the linke to be removed
    pre: lst is not null
    pre: l is not null
    post: lst size is reduced by 1
*/
void _removeLink(struct linkedList *lst, struct DLink *l)
{
    /* verify preconditions */
    assert(lst != 0);           /* lst not null */
    assert(l != 0);             /* l not null */
    assert(!isEmptyList(lst));  /* not empty */
    
    /* link prev to next and next to prev */
    l->prev->next = l->next;
    l->next->prev = l->prev;
    
    /* deallocate l memory */
    free(l);
    
    /* decrement size by 1 */
    lst->size--;
}

/*
    removeFrontList
    param: lst  the linkedList
    pre:lst is not null
    pre: lst is not empty
    post: size is reduced by 1
*/
void removeFrontList(struct linkedList *lst) {
    /* verify preconditions */
    assert(lst != 0);           /* not null */
    assert(!isEmptyList(lst));  /* not empty */
    
    /* remove first link after head sentinel */
    _removeLink(lst, lst->firstLink->next);

}

/*
    removeBackList
    param: lst  the linkedList
    pre: lst is not null
    pre:lst is not empty
    post: size reduced by 1
*/
void removeBackList(struct linkedList *lst)
{    
    /* verify preconditions */
    assert(lst != 0);           /* not null */
    assert(!isEmptyList(lst));  /* not empty */
    
    /* remove first link before tail sentinel */
    _removeLink(lst, lst->lastLink->prev);
}

/*
    isEmptyList
    param: lst  the linkedList
    pre: lst is not null
    post: none
*/
int isEmptyList(struct linkedList *lst) {
    /* verify precondition */
    assert(lst != 0);           /* not null */
    
    /* just use size variable to determine if the list is empty */
    return lst->size < 1;
}


/*  Function to print list
    Pre: lst is not null
 */
void _printList(struct linkedList* lst)
{
    /* verify precondition */
    assert(lst != 0);           /* not null */
    
    /* print start of list notation */
    printf("[");
    
    /* set currentLink to first link in list */
    struct DLink *currentLink = lst->firstLink->next;
    
    /* print fencepost if list is not empty */
    if (!isEmptyList(lst)) {
        printf("%d", currentLink->value);
        currentLink = currentLink->next;
    }
    
    /* keep printing until currentLink reaches tail sentinel */
    while (currentLink != lst->lastLink) {
        printf(", %d", currentLink->value);
        currentLink = currentLink->next;
    }
    
    /* print end of list notation */
    printf("]");
}

/* 
    Add an item to the bag
    param:  lst     pointer to the bag
    param:  v       value to be added
    pre:    lst is not null
    post:   a link storing val is added to the bag
 */
void addList(struct linkedList *lst, TYPE v)
{
    /* verify precondition */
    assert(lst != 0);           /* not null */
    
    /* just add to back of list using existing addBackList function */
    addBackList(lst, v);
}

/*  Returns boolean (encoded as an int) demonstrating whether or not
    the specified value is in the collection
    true = 1
    false = 0

    param:  lst     pointer to the bag
    param:  e       the value to look for in the bag
    pre:    lst is not null
    pre:    lst is not empty
    post:   no changes to the bag
*/
int containsList (struct linkedList *lst, TYPE e) {
    /* verify preconditions */
    assert(lst != 0);           /* not null */
    assert(!isEmptyList(lst));  /* not empty */
    
    /* traverse list and return true if value is found */
    struct DLink *current = lst->firstLink->next;
    while (current != lst->lastLink) {
        if (EQ(current->value, e)) {
            /* value found; return true */
            return 1;
        }
        /* update current to next link */
        current = current->next;
    }
    
    /* value not found; return false */
    return 0;
}

/*  Removes the first occurrence of the specified value from the collection
    if it occurs

    param:  lst     pointer to the bag
    param:  e       the value to be removed from the bag
    pre:    lst is not null
    pre:    lst is not empty
    post:   e has been removed
    post:   size of the bag is reduced by 1
*/
void removeList (struct linkedList *lst, TYPE e) {
    /* verify preconditions */
    assert(lst != 0);           /* not null */
    assert(!isEmptyList(lst));  /* not empty */
    
    /* traverse list and remove first instance of value if found */
    struct DLink *current = lst->firstLink->next;
    while (current != lst->lastLink) {
        if (EQ(current->value, e)) {
            /* value found; remove link and return */
            _removeLink(lst, current);
            return;
        }
        /* update current to next link */
        current = current->next;
    }
}
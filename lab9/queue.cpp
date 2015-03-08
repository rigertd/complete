/*************************************************************************
 * Author:          David Rigert
 * Date Created:    3/7/2015
 * Last Modified:   3/7/2015
 * Assignment:      Lab 9
 * Filename:        queue.cpp
 *
 * Description:     Implementation of the queue class template. 
 ************************************************************************/
#include <stdexcept>
#include "queue.hpp"

// adds value to back of queue
template <class T>
void queue<T>::addBack(const T &value)
{
    back = new node(value, NULL, back);
    if (front)
    {
        // set return pointer of old back
        back->next->prev = back;
    }
    else
    {
        // set front to new node
        front = back;
    }
}

// returns value from front of queue
template <class T>
T &queue<T>::getFront()
{
    if (front)
        return front->value;
    else
        throw std::underflow_error("The queue is empty");
}

// removes value from front of queue
template <class T>
void queue<T>::removeFront()
{
    if (!front)
        throw std::underflow_error("The queue is empty");

    // store front in temp pointer
    node *tmp = front;
    // set front to previous node
    front = front->prev;
    // delete old front node
    delete tmp;
    // set next of new front to NULL if node exists
    if (front)
        front->next = NULL;
    
}

// determines whether queue is empty
template <class T>
bool queue<T>::isEmpty()
{
    // empty if front is NULL
    return !front;
}

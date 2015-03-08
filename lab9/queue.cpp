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
#include <cassert>
#include "queue.hpp"

// copy constructor
template <typename T>
queue<T>::queue(const queue<T> &obj)
{
    // assign source obj front to temp pointer
    node *temp = obj.front;
    // copy over nodes
    while (temp)
    {
        addBack(temp->value);
        temp = temp->prev;
    }
}

// assignment operator
template <typename T>
queue<T> &queue<T>::operator=(const queue<T> &rhs)
{
    // delete existing nodes
    while (!isEmpty())
        removeFront();

    // assign source rhs front to temp pointer
    node *temp = rhs.front;
    // copy over nodes
    while (temp)
    {
        addBack(temp->value);
        temp = temp->prev;
    }
    return *this;
}

// adds value to back of queue
template <typename T>
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
template <typename T>
T &queue<T>::getFront()
{
    if (front)
        return front->value;
    else
        throw std::underflow_error("The queue is empty");
}

// removes value from front of queue
template <typename T>
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
template <typename T>
bool queue<T>::isEmpty()
{
    // empty if front is NULL
    return !front;
}

/*************************************************************************
 * Author:          David Rigert
 * Date Created:    3/7/2015
 * Last Modified:   3/7/2015
 * Assignment:      Lab 9
 * Filename:        queue.hpp
 *
 * Description:     This class template represents a queue structure
 *                  with an underlying container of a doubly-linked list. 
 ************************************************************************/
#ifndef QUEUE_HPP
#define QUEUE_HPP

template <class T>
class queue
{
private:
    struct node
    {
        T value;    // holds the value
        node *prev; // points to previous node
        node *next; // points to next node
        
        node(T value, node *prev, node *next)
        {
            this->value = value;
            this->prev = prev;
            this->next = next;
        }
    };
    node *front;    // points to front of queue
    node *back;     // points to back of queue
    
public:
    queue()                 // constructor
    { 
        front = NULL;
        back = NULL;
    }
    queue(const queue<T> &);   // copy constructor
    ~queue()                // destructor
    {
        while (!isEmpty())
            removeFront();
    }
    queue<T> &operator=(const queue<T> &);    // assignment operator
    void addBack(const T &);// adds value to back of queue
    T &getFront();          // returns value from front of queue
    void removeFront();     // removes value from front of queue
    bool isEmpty();         // determines whether queue is empty
};

#endif  // end of QUEUE_HPP definition

/*************************************************************************
 * Author:          David Rigert
 * Date Created:    3/6/2015
 * Last Modified:   3/7/2015
 * Assignment:      Lab 9
 * Filename:        stack.hpp
 *
 * Description:     This class template represents a stack structure
 *                  with an underlying container of a singly-linked list. 
 ************************************************************************/
#ifndef STACK_HPP
#define STACK_HPP

template <class T>
class stack
{
private:
    struct node
    {
        T value;    // holds the value
        node *next; // points to next node
        
        node(T value, node *next)
        {
            this->value = value;
            this->next = next;
        }
    };
    node *top; // points to top of stack
    
public:
    stack() { top = NULL; }
    void push(const T &);   // adds value to top of stack
    T &peek();              // returns value on top of stack
    void pop();             // removes top value of stack
    bool isEmpty();         // determines whether stack is empty
};

#endif  // end of STACK_HPP definition

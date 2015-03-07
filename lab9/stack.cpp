#include <stdexcept>
#include "stack.hpp"

// adds value to top of stack
template <class T>
void stack<T>::push(const T &value)
{
    // add value to new node, link new node to
    // old top, and assign to top variable
    top = new node(value, top);
}

// returns value on top of stack
template <class T>
T &stack<T>::peek()
{
    if (top)
        return top->value;
    else
        throw std::underflow_error("The stack is empty.");
}

// removes top value of stack
template <class T>
void stack<T>::pop()
{
    // test for empty stack
    if (!top)
        throw std::underflow_error("The stack is empty.");
    
    // store current top in temp pointer
    node *tmp = top;
    // assign top to next node
    top = top->next;
    // delete original top node
    delete tmp;
}

// determines whether stack is empty
template <class T>
bool stack<T>::isEmpty()
{
    return !top;
}

// need to instantiate a class object for all types that will be used
// with this class to avoid linker error
void CreateClassObject()
{
    stack<int> s1;
}
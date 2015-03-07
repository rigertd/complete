#ifndef STACK_HPP
#define STACK_HPP

template <class T>
class stack
{
private:
    T *top; // points to top of stack
    struct node
    {
        T value;    // holds the value
        T *next;    // points to next node
        
        node(T value, T *next)
        {
            this->value = value;
            this->next = next;
        }
    };
    
public:
    stack() { top = NULL; }
    void push(const T &);   // adds value to top of stack
    T &peek();              // returns value on top of stack
    void pop();             // removes top value of stack
    bool isEmpty();         // determines whether stack is empty
};

#endif  // end of STACK_HPP definition

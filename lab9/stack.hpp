template <typename T>
class stack
{
private:
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
    
    T *top; // points to top of stack
public:
    void push(T value); // adds value to top of stack
    T peek();           // returns value on top of stack
    void pop(T &value); // removes top value of stack and saves it to param
    isEmpty();          // determines whether stack is empty
};

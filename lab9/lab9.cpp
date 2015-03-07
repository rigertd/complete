#include <iostream>
#include "stack.hpp"

int main()
{
    stack<int> s;
    std::cout << "Pushing even values onto stack: ";
    for (int i = 2; i <= 10; i += 2)
    {
        std::cout << i << " ";
        s.push(i);
    }
    std::cout << std::endl;
    
    std::cout << "Peeking and popping values from stack: ";
    while (!s.isEmpty())
    {
        std::cout << s.peek() << " "
        s.pop();
    }
    std::cout << std::endl;
    
    return 0;
}
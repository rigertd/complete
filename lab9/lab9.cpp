/*************************************************************************
 * Author:          David Rigert
 * Date Created:    3/6/2015
 * Last Modified:   3/7/2015
 * Assignment:      Lab 9
 * Filename:        lab9.cpp
 *
 * Description:     Prompts the user for a series of numbers and stores them
 *                  in a stack object and a queue object.
 *                  Then, removes the numbers from the containers and
 *                  displays them back to the user.
 * Input:           A series of integers, until a non-integer is entered
 * Output:          The same series of integers reversed for the stack and
 *                  in the same order from the queue.
 ************************************************************************/
#include <cstdlib>      // std::atoi
#include <iostream>     // std::cin, std::cout, std::getline
#include <string>       // std::string
#include "stack.hpp"    // stack
#include "stack.cpp"    // have to include this too because of template
#include "queue.hpp"    // queue
#include "queue.cpp"    // have to include this too because of template
#include "utility.hpp"  // isInteger()


int main()
{
    stack<int> s;
    queue<int> q;
    std::string input;
    
    // display instructions
    std::cout << "Type integers to push on the stack or the queue.\n"
              << "Press Enter or type a non-integer when you are done.\n\n";
              
    // get first number
    std::cout << "Type an integer: ";
    std::getline(std::cin, input);
    while (isInteger(input))
    {
        // convert to int and add to stack and queue
        int value = std::atoi(input.c_str());
        s.push(value);
        q.addBack(value);
        
        // prompt for next number
        std::cout << "Type an integer: ";
        std::getline(std::cin, input);
    }
    
    // display content of stack
    std::cout << std::endl;
    std::cout << "Peeking and popping values from stack: ";
    while (!s.isEmpty())
    {
        std::cout << s.peek() << " ";
        s.pop();
    }
    std::cout << std::endl;
    
    // demonstrate exception
    std::cout << "Popping one more...";
    try
    {
        s.pop();
    }
    catch (std::underflow_error e)
    {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    
    // display content of queue
    std::cout << std::endl;
    std::cout << "Getting and removing values from queue: ";
    while (!q.isEmpty())
    {
        std::cout << q.getFront() << " ";
        q.removeFront();
    }
    std::cout << std::endl;
    
    // demonstrate exception
    std::cout << "Removing one more...";
    try
    {
        q.removeFront();
    }
    catch (std::underflow_error e)
    {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    
    return 0;
}
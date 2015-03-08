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
    stack<int> s1;
    queue<int> q1;
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
        s1.push(value);
        q1.addBack(value);
        
        // prompt for next number
        std::cout << "Type an integer: ";
        std::getline(std::cin, input);
    }
    std::cout << std::endl;
    
    std::cout << "Stack Objects:" << std::endl << std::endl;
    
    // copy constructor
    std::cout << "Using copy constructor to initialize stack 2 to stack 1...";
    stack<int> s2 = s1;
    
    // display content of stack 1
    std::cout << std::endl;
    std::cout << "Peeking and popping values from stack 1: ";
    while (!s1.isEmpty())
    {
        std::cout << s1.peek() << " ";
        s1.pop();
    }
    std::cout << std::endl;
    
    // assignment operator
    std::cout << "Using assignment operator to set stack 1 to stack 2...";
    std::cout << std::endl;
    s1 = s2;
    
    // display content of stack 1
    std::cout << "Peeking and popping values from stack 1: ";
    while (!s1.isEmpty())
    {
        std::cout << s1.peek() << " ";
        s1.pop();
    }
    std::cout << std::endl;
    
    // demonstrate exception
    std::cout << "Popping one more...";
    try
    {
        s1.pop();
    }
    catch (std::underflow_error e)
    {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << "Queue Objects:" << std::endl << std::endl;
    
    // copy constructor
    std::cout << "Using copy constructor to initialize queue 2 to queue 1...";
    std::cout << std::endl;
    queue<int> q2 = q1;
    
    // display content of queue 1
    std::cout << "Getting and removing values from queue 1: ";
    while (!q1.isEmpty())
    {
        std::cout << q1.getFront() << " ";
        q1.removeFront();
    }
    std::cout << std::endl;
    
    // assignment operator
    std::cout << "Using assignment operator to set queue 1 to queue 2...";
    std::cout << std::endl;
    q1 = q2;
    
    // display content of queue 1
    std::cout << "Getting and removing values from queue 1: ";
    while (!q1.isEmpty())
    {
        std::cout << q1.getFront() << " ";
        q1.removeFront();
    }
    std::cout << std::endl;

    // demonstrate exception
    std::cout << "Removing one more...";
    try
    {
        q1.removeFront();
    }
    catch (std::underflow_error e)
    {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    
    return 0;
}
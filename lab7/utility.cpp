/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           2/18/2015
 * Last Modification Date: 2/18/2015
 * Course:                 CS162_400
 * Assignment:             Lab 7
 * Filename:               utility.cpp
 *
 * Overview:
 *     Implementations of functions in utility.hpp.
 ************************************************************************/
#include <iostream>

#include "utility.hpp"

// Gets a non-empty string.
std::string getString(std::string prompt)
{
    std::string input;  // for storing user input
    
    // prompt user and get input
    std::cout << prompt;
    std::getline(std::cin, input);
    
    // validate input
    while (input.empty())
    {
        std::cout << "ERROR: You must enter a value. Try again.\n";
        std::cout << prompt;
        std::getline(std::cin, input);
    }
    
    return input;
}


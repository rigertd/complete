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
#include <string>

#include "utility.hpp"

// Gets a non-empty string.
std::string getFilename(std::string prompt, std::string defaultFilename)
{
    std::string input;  // for storing user input
    
    // prompt user and get input
    std::cout << prompt;
    std::getline(std::cin, input);
    
    // validate input
    if (input.empty())
    {
        std::cout << "Using default filename of "
                  << defaultFilename << ".\n";
        input = defaultFilename;
    }
    
    return input;
}


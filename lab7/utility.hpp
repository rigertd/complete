/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           2/18/2015
 * Last Modification Date: 2/18/2015
 * Course:                 CS162_400
 * Assignment:             Lab 7
 * Filename:               utility.hpp
 *
 * Overview:
 *     Several utility functions for getting validated user input.
 ************************************************************************/
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include <limits>   // numeric_limits

// Gets valid user input of type T.
// Have to include in header due to template limitations.
template <typename T>
T getInput(std::string prompt)
{
    T input;    // for storing user input
    
    // prompt user and get input
    std::cout << prompt;
    std::cin >> input;
    
    // validate input
    while (std::cin.get() != '\n')
    {
        // clear keyboard buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::cout << "ERROR: Invalid input. Try again.\n";
        std::cout << prompt;
        std::cin >> input;
    }
    
    return input;
}

// Gets a non-empty string.
std::string getString(std::string);

#endif  // end of UTILITY_HPP definition

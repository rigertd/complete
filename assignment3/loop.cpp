/**********************************************************
* Author:                 David Rigert
* Date Created:           10/18/2014
* Last Modification Date: 10/18/2014
* Assignment:             Assignment 3
* Filename:               loop.cpp
*
* Overview:
*     This program prompts the user for two strings
*     and determines if they are equal.
* 
* Input:
*     Two user-entered strings
*
* Output:
*     The output to the console will be in the form of:
*
*     This program tells you if two strings are the same.
*
*     Enter the first string: [Hello]
*     Enter the second string: [Hello]
*
*     "Hello" and "Hello" are the same.
**********************************************************/
#include <iostream> // For cin and cout
#include <string>   // For string class

int main()
{
    std::string str1;    // First string
    std::string str2;    // Second string
    bool isEqual = true; // Assume equal until proven otherwise
    
    std::cout << "This program tells you if two strings are the same."
              << std::endl << std::endl
              << "Enter the first string: ";
    std::getline(std::cin, str1);
    
    std::cout << "Enter the second string: ";
    std::getline(std::cin, str2);
    
    // First check if same length
    if (str1.length() != str2.length())
        isEqual = false;
    else
    {
        // Loop through string until inequality found
        for (int i = 0; i < str1.length(); i++)
        {
            if (str1.at(i) != str2.at(i))
            {
                isEqual = false;
                break; // No need to continue comparing
            }
        }
    }

    // Print result
    std::cout << std::endl << "\"" << str1 << "\" and \""
              << str2 << "\" are " << (isEqual ? "" : "not ") << "the same.";

    return 0;
}
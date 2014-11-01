/**********************************************************
* Author:                 David Rigert
* Date Created:           10/21/2014
* Last Modification Date: 10/31/2014
* Assignment:             Assignment 5
* Filename:               refFun.cpp
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

// Function prototype

// Determines whether two strings are equal.
bool IsEqual(std::string &, std::string &);

int main()
{
    std::string str1;    // First string
    std::string str2;    // Second string
    
    std::cout << "This program tells you if two strings are the same."
              << std::endl << std::endl
              << "Enter the first string: ";
    std::getline(std::cin, str1);
    
    std::cout << "Enter the second string: ";
    std::getline(std::cin, str2);
    
    // Print result
    std::cout << std::endl << "\"" << str1 << "\" and \""
              << str2 << "\" are " << (IsEqual(str1, str2) ? "" : "not ")
              << "the same." << std::endl;

    return 0;
}

/**********************************************************
 *  bool IsEqual(std::string &str1, std::string &str2)
 *
 *  Purpose: This function determines whether str1 and str2
 *           are equal. The comparison is case sensitive.
 *
 *  Preconditions: None.
 *   
 *  Postconditions: Returns true if str1 and str2 are equal.
 *********************************************************/
bool IsEqual(std::string &str1, std::string &str2)
{
    // First check if same length
    if (str1.length() != str2.length())
        return false;
    else
    {
        // Loop through string until inequality found
        for (int i = 0; i < str1.length(); i++)
        {
            if (str1.at(i) != str2.at(i))
            {
                return false; // No need to continue comparing
            }
        }
    }

    return true;
}

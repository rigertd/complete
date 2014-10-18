/**********************************************************
* Author:                 David Rigert
* Date Created:           10/17/2014
* Last Modification Date: 10/17/2014
* Assignment:             Assignment 3
* Filename:               strings.cpp
*
* Overview:
*     This program prompts the user for a string of characters,
*     prints the string forward and backward, 
*     and prints the number of letters.
* 
* Input:
*     String of characters
*
* Output:
*     The output to the console will be in the form of:
*
*     Enter a string of characters: [Word]
*
*     Word
*     droW
*     Letters: 4
**********************************************************/
#include <string>
#include <iostream>
#include <cctype>

int main()
{
	std::string my_str;  // String for storing user input
    int letterCount = 0; // Count letters in string
    
    std::cout << "Enter a string of characters: ";
    // Replace cin with getline to support multiple words
    // std::cin >> my_str;  // Get input from user
    std::getline(std::cin, my_str); // Get entire line of input
    std::cout << std::endl;

    // Print input forward
    for (int i = 0; i < my_str.length(); i++)
    {
        std::cout << my_str.at(i);
        
        // Count letters
        if (isalpha(my_str.at(i)))
        {
            letterCount++;
        }
    }
    std::cout << std::endl;
	
    // Print input backward
    for (int i = my_str.length() - 1; i >= 0; i--)
    {
        std::cout << my_str.at(i);
    }

    std::cout << std::endl << "Letters: " << letterCount << std::endl;
    
    return 0;
}
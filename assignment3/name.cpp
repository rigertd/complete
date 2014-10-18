/**********************************************************
* Author:                 David Rigert
* Date Created:           10/17/2014
* Last Modification Date: 10/17/2014
* Assignment:             Assignment 3
* Filename:               name.cpp
*
* Overview:
*     This program prompts the user a full name,
*     including first, middle (or just middle initial), and last
*     in that order, and then prints it as Last, First MI.
* 
* Input:
*     Full name, including middle or middle initial
*
* Output:
*     The output to the console will be in the form of:
*
*     Enter your first, middle, and last name in the following order.
*     First Middle Last: [John Henry Doe]
*
*     Doe, John H.
**********************************************************/
#include <string>
#include <iostream>
#include <cctype>

int main()
{
	std::string fullName;        // String for storing full name
    std::string firstName = "";  // String for storing first name
    std::string middleInit = ""; // String for storing middle initial
    std::string lastName = "";   // String for storing last name
    int wordCount;               // Count tokens in input
    bool spaceFlag;              // Tells if last char was whitespace
    
    // Start input validation loop
    do
    {
        wordCount = 0;    // Initialize word counter
        spaceFlag = true; // Initialize whitespace flag
        
        std::cout << "Enter your first, middle, and last name "
                  << "in the following order." << std::endl
                  << "First Middle Last: ";
              
        std::getline(std::cin, fullName); // Get entire line of input
        
        // Count number of words in string
        for (int i = 0; i < fullName.length(); i++)
        {
            // Increment wordCount if first character is not a space,
            // or if last character was a space and current one is not.
            if (spaceFlag && !std::isspace(fullName.at(i)))
            {
                wordCount++;
            }
            
            // Set space flag if current char is whitespace
            spaceFlag = std::isspace(fullName.at(i));
        }
        
        if (wordCount < 2)
        {
            std::cout << std::endl
                      << "You must enter at least your first and last name." 
                      << std::endl << std::endl;
        }
    }
    while (wordCount < 2);
    
    // Store first, middle initial, and last name in variables
    int currentWord = 0; // Keep track of current word number
    spaceFlag = true;   // Reuse spaceFlag variable
    for (int i = 0; i < fullName.length(); i++)
    {
        // Increment word number counter if new word
        if (spaceFlag && !std::isspace(fullName.at(i)))
        {
            currentWord++;
        }
        
        // Append current char to buffer depending on word number
        if (!std::isspace(fullName.at(i)))
        {
            if (currentWord == 1)
            { // First name
                firstName += fullName.at(i);
            }
            else if (wordCount > 2 && currentWord == 2 && spaceFlag)
            { // First letter of 2nd name, but only if more than 2 names
                middleInit = fullName.at(i);
            }
            else if (wordCount == currentWord)
            { // Last name
                lastName += fullName.at(i);
            }
        }
        
        // Set space flag if current char is whitespace
        spaceFlag = std::isspace(fullName.at(i));
    }
    
    std::cout << std::endl;

    // Print name as Last, First MI.
    std::cout << lastName << ", " << firstName 
              << (wordCount > 2 ? " " + middleInit + "." : "") << std::endl;

    return 0;
}
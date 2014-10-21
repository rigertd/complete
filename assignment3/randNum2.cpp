/**********************************************************
* Author:                 David Rigert
* Date Created:           10/18/2014
* Last Modification Date: 10/18/2014
* Assignment:             Assignment 3
* Filename:               randNum2.cpp
*
* Overview:
*     This program gets two numbers from the user,
*     generates a random number in between them,
*     and prints the result to the console.
* 
* Input:
*     Two integer values representing the minimum and maximum number
*
* Output:
*     The output to the console will be in the form of:
*
*     Enter the smallest desired whole number: [1]
*     Enter the largest desired whole number: [100]
*
*     Your randomly picked number is 49. (different every time)
**********************************************************/
#include <ctime>    // For time (as seed)
#include <iostream> // For cout
#include <cstdlib>  // For rand and srand
#include <string>   // For string class
#include <limits>   // For numeric_limits

// Function prototypes
int getValidatedInt(std::string);

int main()
{
    int maxNum;          // Maximum random number
    int minNum;          // Minimum random number
    int randNum;         // Generated random number
    unsigned int seed;   // seed for srand function

    seed = std::time(0); // Set seed value to seconds since epoch
    std::srand(seed);    // Set the seed value for rand
    
    // Get valid user input
    minNum = getValidatedInt("Enter the smallest desired whole number: ");
    maxNum = getValidatedInt("Enter the largest desired whole number: ");

    // Make sure max >= min
    while (maxNum < minNum)
    {
        std::cout << "The largest number must be at least as large "
                  << "as the smallest." << std::endl;
                  
        // Get new maxNum
        maxNum = getValidatedInt("Enter the largest desired whole number: ");
    }
    
    randNum = rand() % (maxNum + 1 - minNum) + minNum;
    
    // Print the randomly generated number
    std::cout << std::endl << "Your randomly picked number is "
              << randNum << "." << std::endl;

    return 0;
}

/*  Gets an integer value from the console.
 *  Prints the specified prompt before accepting input.
 *  Performs basic input validation to ensure a valid integer.
 */
int getValidatedInt(std::string prompt)
{
    int input = 0; // for storing user input
    
    std::cout << prompt;
    std::cin >> input;
    
    /* Implements validation by detecting the next char in the cin buffer.
     * Unless the next char is a newline, 
     * it means there is unused data still in the buffer.
     */
    while (std::cin.get() != '\n') // true if next char not newline
    {
        std::cout << std::endl << "That is not a valid number. " << std::endl
                  << "Please enter a whole number between 1 and "
                  << std::numeric_limits<int>::max()  // Max int
                  << std::endl << "without any commas: ";
        std::cin.clear();     // Clear any errors.
        // Ignore any remaining input in buffer, up to the max buffer size.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> input;    // Store console input in 'input' variable.
    }
    
    return input; // Send the input value back to the caller.
}

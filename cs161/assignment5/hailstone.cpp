/**********************************************************
 * Author:                 David Rigert
 * Date Created:           11/1/2014
 * Last Modification Date: 11/1/2014
 * Assignment:             Assignment 5
 * Filename:               hailstone.cpp
 *
 * Overview:
 *     This program gets an integer from the user and 
 *     calculates the number of steps required to reach 1
 *     in the hailstone sequence.
 * 
 * Input:
 *     Number to start from
 *
 * Output:
 *     The output to the console will be in the form of:
 *
 *     Enter the starting number of the hailstone sequence: [3]
 *
 *     It takes 7 steps to reach 1.
 *********************************************************/
#include <iostream> // For cout
#include <string>   // For string class
#include <limits>   // For numeric_limits
#include <sstream>  // For stringstream class

// Function prototypes

// Recursively count the steps in the hailstone sequence from n.
void GetHailstone(int, int &);

// Prompts the user for an integer in the specified range and returns it.
int GetValidatedInt(const std::string &, 
                    int = std::numeric_limits<int>::min(), 
                    int = std::numeric_limits<int>::max());

int main()
{
    // Set the maximum start value to a number that won't overflow an int
    const int MAX_START = std::numeric_limits<int>::max() / 3 - 1;
    
    int start,     // Starting number for hailstone sequence
        count = 0; // Number of steps to get to 1
    
    // Get the starting number of the hailstone sequence
    start = GetValidatedInt(
        "Enter the starting number of the hailstone sequence: ", 1, MAX_START);
    
    // Get the number of steps to reach 1
    GetHailstone(start, count);
    
    // Display the number of steps
    std::cout << "\nIt takes " << count << " steps to reach 1.\n";

    return 0;
}

/**********************************************************
 *  void GetHailstone(int n, int &counter)
 *
 *  Purpose: This function recursively counts the number of steps
 *           to reach 1 from n in the hailstone sequence.
 *
 *  Preconditions: n >= 1, counter initialized to 0
 *   
 *  Postconditions: Counter contains the number of steps
 *********************************************************/
void GetHailstone(int n, int &counter)
{
    if (n == 1)           // Base case
    {
        return;
    }
    else if (n % 2 == 0) // Even recursive case
    {
        GetHailstone(n / 2, ++counter);
    }
    else                 // Odd recursive case
    {
        GetHailstone(n * 3 + 1, ++counter);
    }
}

/**********************************************************
 *  int GetValidatedInt(std::string prompt,
 *                      int min = std::numeric_limits<int>::min(),
 *                      int max = std::numeric_limits<int>::max())
 *
 *  Purpose: This function displays the specified prompt
 *           and gets a valid integer from the user
 *           within the specified range.
 *           The range defaults to the minimum and maximum int
 *           values if not specified.
 *
 *  Preconditions: None.
 *   
 *  Postconditions: Leaves the input buffer empty 
 *                  and returns an integer value.
 *********************************************************/
int GetValidatedInt(const std::string &prompt, int min, int max)
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
        std::cout << std::endl << "That is not a valid number." << std::endl
                  << "Please enter a whole number between "
                  << min << " and " // Min value
                  << max            // Max value
                  << std::endl << "without any commas: ";
        std::cin.clear();           // Clear any errors.
        // Ignore any remaining input in buffer, up to the max buffer size.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> input;    // Store console input in 'input' variable.
    }
    
    // Recursively call until input is between min and max.
    if (input < min || input > max)
    {
        // Build new prompt
        std::stringstream ss;
        ss << "Your number must be between " << min
           << " and " << max << ": ";
        input = GetValidatedInt(ss.str(), min, max);
    }
    
    return input; // Send the input value back to the caller.
}

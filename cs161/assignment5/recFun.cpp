/**********************************************************
 * Author:                 David Rigert
 * Date Created:           11/1/2014
 * Last Modification Date: 11/1/2014
 * Assignment:             Assignment 5
 * Filename:               recFun.cpp
 *
 * Overview:
 *     This program gets a number from the user and displays the 
 *     number at that position in the Fibonacci sequence.
 * 
 * Input:
 *     Index of the Fibonacci sequence to display
 *
 * Output:
 *     The output to the console will be in the form of:
 *
 *     Enter the index of the Fibonacci number to display: [4]
 *
 *     The 4th Fibonacci number is 5.
 *********************************************************/
#include <iostream> // For cout
#include <string>   // For string class
#include <limits>   // For numeric_limits
#include <sstream>  // For stringstream class

// Function prototypes

// Recursively gets the nth Fibonacci number.
long GetFib(int);

// Recursively gets the nth Fibonacci number efficiently.
long GetFib(int, long, long);

// Prompts the user for an integer in the specified range and returns it.
int GetValidatedInt(const std::string &, 
                    int = std::numeric_limits<int>::min(), 
                    int = std::numeric_limits<int>::max());

// Converts an integer to an ordinal string (1st, 2nd, etc.).
std::string IntToOrdinal(int);

int main()
{
    int index;   // Index of the Fibonacci number to get
    long result, // Fibonacci number at index
         result2;
    
    // Get index of Fibonacci number, up to 45 
    // (higher than this causes overflow)
    index = GetValidatedInt(
        "Enter the index of the Fibonacci number to display [0-45]: ", 0, 45);
    
    // Get the Fibonacci number at index
    result = GetFib(index);

    // Display the specified Fibonacci number
    std::cout << "\nThe " << IntToOrdinal(index) 
              << " Fibonacci number is " << result << ".\n";
    
    // Get the Fibonacci number efficiently.
    result2 = GetFib(index, 1, 1);
    
    // Display the specified Fibonacci number
    std::cout << "\nUsing efficient recursion, the " << IntToOrdinal(index)
              << " Fibonacci number is " << result2 << ".\n";

    return 0;
}

/**********************************************************
 *  long GetFib(int n)
 *
 *  Purpose: This function recursively gets the nth
 *           Fibonacci number.
 *
 *  Preconditions: n >= 0
 *   
 *  Postconditions: Returns the nth Fibonacci number
 *********************************************************/
long GetFib(int n)
{
    if (n < 2) // Base case
    {
        return 1;
    }
    else        // Recursive case
    {
        return GetFib(n - 2) + GetFib(n - 1);
    }
}

long GetFib(int n, long current, long previous)
{
    if (n == 0) // Base case
    {
        return previous;
    }
    else if (n == 1)
    {
        return current;
    }
    else       // Recursive case
    {
        return GetFib(n - 1, current + previous, current);
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

/**********************************************************
 *  int IntToOrdinal(int num)
 *
 *  Purpose: This function converts an integer to an ordinal
 *           string (e.g., 1st, 2nd, 3rd, 4th, etc.)
 *
 *  Preconditions: None.
 *   
 *  Postconditions: Returns num as an ordinal number.
 *********************************************************/
std::string IntToOrdinal(int num)
{
    int lastDigit = num % 10; // The last digit determines the ordinal
    std::stringstream ss;     // stringstream object for conversion
    
    // Create an ordinal number based on num
    switch(lastDigit)
    {
        case 1:
            ss << num << "st";
            break;
        case 2:
            ss << num << "nd";
            break;
        case 3:
            ss << num << "rd";
            break;
        default:
            ss << num << "th";
    }
    
    return ss.str();
}
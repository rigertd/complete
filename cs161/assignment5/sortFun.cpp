/**********************************************************
 * Author:                 David Rigert
 * Date Created:           10/31/2014
 * Last Modification Date: 10/31/2014
 * Assignment:             Assignment 5
 * Filename:               sortFun.cpp
 *
 * Overview:
 *     This program gets three numbers from the user,
 *     displays the order as entered, sorts them from
 *     smallest to largest, and displays them in that order.
 * 
 * Input:
 *     Three integer values
 *
 * Output:
 *     The output to the console will be in the form of:
 *
 *     This program sorts three integers from smallest to largest.
 *     Enter the first integer: [1]
 *     Enter the second integer: [3]
 *     Enter the third integer: [2]
 *
 *     You entered 1, 3, and 2.
 *     After sorting, the numbers are 1, 2, and 3.
 *********************************************************/
#include <iostream> // For cout
#include <cstdlib>  // For min function
#include <string>   // For string class
#include <limits>   // For numeric_limits

// Function prototypes

// Sorts the values in the specified variables from smallest to largest.
void SortNums(int &, int &, int &);

// Prompts the user for a valid integer and returns it.
int GetValidatedInt(std::string);

int main()
{
    int num1,          // First number
        num2,          // Second number
        num3;          // Third number

    std::cout << "This program sorts three integers "
              << "from smallest to largest.\n";
    
    // Get valid user input for each number number
    num1 = GetValidatedInt("Enter the first number: ");
    num2 = GetValidatedInt("Enter the second number: ");
    num3 = GetValidatedInt("Enter the third number: ");
    
    // Display numbers in original order
    std::cout << "\nYou entered " << num1 << ", " << num2 
              << ", and " << num3 << ".\n";
    
    // Sort numbers into smallest to largest
    SortNums(num1, num2, num3);

    // Display numbers from smallest to largest.
    std::cout << "After sorting, the numbers are " << num1 
              << ", " << num2 << ", and " << num3 << ".\n";

    return 0;
}

/**********************************************************
 *  void SortNums(int &num1, int &num2, int &num3)
 *
 *  Purpose: Sorts the values in the specified variables 
 *           from smallest to largest.
 *
 *  Preconditions: None
 *   
 *  Postconditions: The values in num1, num2, and num3 are
 *                  sorted from smallest to largest.
 *********************************************************/
void SortNums(int &num1, int &num2, int &num3)
{
    int temp; // For swapping
    
    // Swap num1 and num2 if num1 is larger
    if (num1 > num2)
    {
        temp = num1;
        num1 = num2;
        num2 = temp;
    }
    
    // Swap num2 and num3 if num2 is larger
    if (num2 > num3)
    {
        temp = num2;
        num2 = num3;
        num3 = temp;
    }
    
    // Swap num1 and num2 again if num1 is larger
    if (num1 > num2)
    {
        temp = num1;
        num1 = num2;
        num2 = temp;
    }
}

/**********************************************************
 *  int GetValidatedInt(std::string prompt)
 *
 *  Purpose: This function displays the specified prompt
 *           and gets a valid integer from the user.
 *
 *  Preconditions: None.
 *   
 *  Postconditions: Leaves the input buffer empty 
 *                  and returns an integer value.
 *********************************************************/
int GetValidatedInt(std::string prompt)
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
                  << std::numeric_limits<int>::min() << " and " // Min int
                  << std::numeric_limits<int>::max()  // Max int
                  << std::endl << "without any commas: ";
        std::cin.clear();     // Clear any errors.
        // Ignore any remaining input in buffer, up to the max buffer size.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> input;    // Store console input in 'input' variable.
    }
    
    return input; // Send the input value back to the caller.
}

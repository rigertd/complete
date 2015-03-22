/**********************************************************
 * Author:                 David Rigert
 * Date Created:           11/1/2014
 * Last Modification Date: 11/1/2014
 * Assignment:             Assignment 5
 * Filename:               recConvert.cpp
 *
 * Overview:
 *     This program displays a menu where the user can choose 
 *     to convert a binary number to decimal, convert a decimal
 *     number to binary, or exit the program.
 * 
 * Input:
 *     Menu: 1 for binary to decimal, 2 for decimal to binary, 3 to exit
 *     Binary: A valid positive binary number
 *     Decimal: A valid positive decimal number
 *
 * Output:
 *     The output to the console will be in the form of:
 *
 *     Menu
 *     1: Convert binary number to decimal
 *     2: Convert decimal number to binary
 *     3: Exit program
 *
 *     Enter your selection [1-3]: [1]
 *     Enter a positive binary number: [1010]
 *     
 *     1010 binary is 10 in decimal
 *
 *     Menu
 *     ... 
 *********************************************************/
#include <iostream> // For cout
#include <string>   // For string class
#include <sstream>  // For stringstream class

// Function prototypes

// Recursively convert a binary in a string to a decimal in an int.
int BinToDec(std::string);

// Recursively convert a decimal int to binary in a string.
std::string DecToBin(int);

// Determines whether a string is a valid binary number.
bool IsBinary(std::string);

// Clears the input buffer.
void ClearBuffer();

int main()
{
    short selection; // User selection on menu
    
    // Start of menu do-while loop
    do
    {
        std::cout << "\nMenu\n"
                  << "1: Convert binary number to decimal\n"
                  << "2: Convert decimal number to binary\n"
                  << "3: Exit program\n\n"
                  << "Enter your selection [1-3]: ";
        
        // Store selection in variable
        std::cin >> selection;
        
        // Parse input
        if (std::cin.get() != '\n' || selection < 1 || selection > 3)
        {
            std::cout << "Invalid selection. Try again\n";
            ClearBuffer();
            continue; // just in case input started with 3
        }
        else if (selection == 1)
        {
            std::string input;
            std::cout << "Enter a positive binary number: ";
            std::getline(std::cin, input);
            
            // Validate input. More than 31 bits causes int overflow.
            if (IsBinary(input) && input.length() < 32)
            {
                std::cout << std::endl << input << " binary is "
                          << BinToDec(input) << " in decimal\n";
            }
            else
            {
                std::cout << "Sorry, that is not a valid binary number.\n";
            }
        }
        else if (selection == 2)
        {
            int input;
            std::cout << "Enter a positive decimal number: ";
            std::cin >> input;
            
            // Validate input
            if (std::cin.get() == '\n' && input >= 0)
            {
                std::cout << std::endl << input << " decimal is "
                          << DecToBin(input) << " in binary\n";
            }
            else
            {
                std::cout << "Sorry, that is not a valid decimal number.\n";
                ClearBuffer(); // Clear input buffer for next iteration
            }
            
        }
    }
    while (selection != 3);

    return 0;
}

/**********************************************************
 *  int BinToDec(std::string num)
 *
 *  Purpose: This function recursively converts a binary 
 *           number in a string to a decimal in an int.
 *
 *  Preconditions: num is valid binary and >= 0
 *   
 *  Postconditions: Returns the decimal equivalent
 *********************************************************/
int BinToDec(std::string num)
{
    int len = num.length(); // Store length in advance
    
    if (len < 2)                     // Base case
    {
        // Return 1 if "1", and 0 if "0" or ""
        return num == "1" ? 1 : 0;
    }
    else if (num.at(len - 1) == '1') // Recursive case ending in "1"
    {
        return 2 * BinToDec(num.substr(0, len - 1)) + 1;
    }
    else                             // Recursive case ending in "0"
    {
        return 2 * BinToDec(num.substr(0, len - 1));
    }
}

/**********************************************************
 *  std::string DecToBin(int num)
 *
 *  Purpose: This function recursively converts a binary 
 *           number in a string to a decimal in an int.
 *
 *  Preconditions: num is valid binary and >= 0
 *   
 *  Postconditions: Returns the decimal equivalent
 *********************************************************/
std::string DecToBin(int num)
{
    if (num == 0) // Base cases
    {
        return "0";
    }
    else if (num == 1)
    {
        return "1";
    }
    else          // Recursive case
    {             // Append 1 if odd, 0 if even.
        return DecToBin(num / 2) + (num % 2 == 0 ? "0" : "1");
    }
}

/**********************************************************
 *  bool IsBinary(std::string num)
 *
 *  Purpose: This function determines whether a string is
 *           a valid binary number (contains only '1's and '0's).
 *
 *  Preconditions: None.
 *   
 *  Postconditions: Returns whether valid binary.
 *********************************************************/
bool IsBinary(std::string num)
{
    for (int i = 0; i < num.length(); i++)
    {
        // Return false if outside of 0-1 range
        if (num.at(i) < '0' || num.at(i) > '1')
            return false;
    }
    
    return true;
}

/**********************************************************
 *  void ClearBuffer()
 *
 *  Purpose: This function resets the error bit and 
 *           clears the input buffer.
 *
 *  Preconditions: None.
 *   
 *  Postconditions: The input buffer is empty.
 *********************************************************/
void ClearBuffer()
{
    if (!std::cin.eof())
    {
        std::cin.clear(); // Clear any errors.
        std::cin.sync();  // Clear the buffer.
    }
}

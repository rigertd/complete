/**********************************************************
 * Author:                 David Rigert
 * Date Created:           10/1/2014
 * Last Modification Date: 10/2/2014
 * Assignment:             Assignment 1
 * Filename:               echo.cpp
 *
 * Overview:
 *     This program prints a number entered by the user
 *     to the console. Performs basic input validation to ensure
 *     the input begins with a number.
 * 
 * Input:
 *     User enters a positive or negative integer.
 *
 * Output:
 *     The output to the console will be in the form of:
 *
 *     Enter an integer value for me to repeat: 100
 *
 *     You entered 100.
 *********************************************************/
#include <iostream> // required for cin and cout
#include <limits>   // required for numeric_limits

using namespace std;

int main()
{
    long input;    // for storing user input.
    
    cout << "Enter an integer value for me to repeat: ";
    cin >> input;  // Store console input in 'input' variable.
    
    /* Basic input validation.
     * The program will only accept input that includes all numerical values.
     * It does this by detecting the next character in the cin buffer.
     * Unless the next char is a newline, 
     * it means there is unused data still in the buffer.
     */
    while (cin.get() != '\n')   // true if next char is not newline
    {
        cout << endl << "That is not a valid integer. " << endl 
             << "Please enter a positive or negative whole number" << endl
             << " between " << numeric_limits<long>::min() // Smallest long val
             << " and " << numeric_limits<long>::max()    // Largest long val
             << "without any commas: ";
        cin.clear();     // Clear any errors.
        // Ignore any remaining input in buffer, up to the max buffer size.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> input;    // Store new console input in 'input' variable.
    }
    cout << endl << "You entered " << input << ".\n";
    
    return 0;
}


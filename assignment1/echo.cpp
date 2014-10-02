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
 *     You entered 100.
 *********************************************************/
#include <iostream>
#include <cstdio>

using namespace std;

int main()
{
    long input; // for storing user input
    
    cout << "Enter an integer value for me to repeat: ";
    cin >> input;
	
	/* Basic input validation.
	 * This does not catch instances where the input starts with a number
	 * and is followed by a non-number. For example, input of "123abc"
     * will print "123" and input of "456.7" will print "456"
     * I would need to use cin.getline for proper input validation.	 
	 */
    while (cin.fail())   // true if input starts with non-number.
    {
        cout << "That is not a valid integer. " << endl 
             << "Please enter a positive or negative whole number: ";
        cin.clear();     // Clear the error bit.
        // Ignore any remaining input in buffer, up to the max buffer size.
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> input;
    }
    cout << "You entered " << input << ".\n";
    
    return 0;
}
    
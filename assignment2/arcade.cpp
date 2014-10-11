/**********************************************************
* Author:                 David Rigert
* Date Created:           10/9/2014
* Last Modification Date: 10/11/2014
* Assignment:             Assignment 2
* Filename:               arcade.cpp
*
* Overview:
*     This program prompts the user for the number of coupons
*     and prints how many candy bars and gumballs the coupons
*     can be exchanged for. It also prints how many coupons will
*     be left over.
*     This version of the program uses division and remainders
*     to determine the number of each prize.
* 
* Input:
*     Number of coupons
*
* Output:
*     The output to the console will be in the form of:
*
*     Enter the number of coupons you have: 
*
*     Your # coupons can be exchanged for:
*         # candy bars
*         # gumballs
*     with # coupons left over.
**********************************************************/
#include <iostream>
#include <limits>
#include <string>
#include <cmath>
using namespace std;

// Function prototypes
short getValidatedShort(string);

int main()
{
    short coupons = 0,   // number of coupons
          candyBars = 0, // number of candy bars
          gumballs = 0,  // number of gumballs
          leftover = 0;  // number of leftover coupons
    
    // Get the number of coupons from the user.
    coupons = getValidatedShort("Enter the number of coupons you have: ");
    
    // Because this is more fun than reprompting
    if (coupons < 0)
    {
        cout << endl << "You are " << abs(static_cast<float>(coupons))
             << " coupon" << (coupons == -1 ? "" : "s") << " in debt." << endl
             << "You owe the arcade prizes!" << endl;
    }
    else if (coupons == 0)
    {
        cout << endl << "Sorry, but nothing in life is free!" << endl;
    }
    else if (coupons < 3)
    {
        cout << endl << "Sorry, you can't afford any of the prizes with your "
             << "measly " << coupons << " coupon" << (coupons == 1 ? "" : "s")
             << "!" << endl;
    }
    else // User has at least 3 coupons.
    {
        candyBars = coupons / 10;
        gumballs = (coupons % 10) / 3;
        leftover = (coupons % 10) % 3;
        
        cout << endl << "Your " << coupons 
             << " coupons can be exchanged for:" << endl;
        
        // Only print candy bars if > 0
        if (candyBars > 0)
        {
            cout << "    " << candyBars << " candy bar" 
            << (candyBars == 1 ? "" : "s") << endl;
        }

        // Only print gumballs if > 0
        if (gumballs > 0)
        {
            cout << "    " << gumballs << " gumball" 
                 << (gumballs == 1 ? "" : "s") << endl;
        }
        
        // Print leftover coupons
        cout << "with " << leftover << " coupon"
             << (leftover == 1 ? "" : "s") << " left over." << endl;
    }
    
    return 0;
}

/*  Gets an integer value from the console.
 *  Prints the specified prompt before accepting input.
 *  Performs basic input validation to ensure a valid integer.
 */
short getValidatedShort(string prompt)
{
    short input = 0; // for storing user input
    
    cout << prompt;
    cin >> input;
    
    /* Implements validation by detecting the next char in the cin buffer.
     * Unless the next char is a newline, 
     * it means there is unused data still in the buffer.
     */
    while (cin.get() != '\n') // true if next char not newline
    {
        cout << endl << "That is not a valid number. " << endl 
             << "Please enter a whole number between 1 and "
             << numeric_limits<short>::max()  // Max short
             << endl << "without any commas: ";
        cin.clear();     // Clear any errors.
        // Ignore any remaining input in buffer, up to the max buffer size.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> input;    // Store console input in 'input' variable.
    }
    
    return input; // Send the input value back to the caller.
}

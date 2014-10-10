/**********************************************************
* Author:                 David Rigert
* Date Created:           10/9/2014
* Last Modification Date: 10/9/2014
* Assignment:             Assignment 2
* Filename:               arcade2.cpp
*
* Overview:
*     This program prompts the user for the number of coupons
*     and prints how many candy bars and gumballs the coupons
*     can be exchanged for. It also prints how many coupons will
*     be left over.
*     This version of the program uses loops
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
*     Your # coupons can be exchanged for...
*     Candy bars:
*     CCCCC CCCCC
*     C   C C   C
*     CCCCC CCCCC
*
*     Gumballs:
*      GGG   GGG
*     G   G G   G
*      GGG   GGG
*
*     Coupons left over:
*      ____  ____
*     /   | /   |
*     ----- -----
**********************************************************/
#include <iostream>
#include <limits>
#include <string>
#include <cmath>
using namespace std;

// Function prototypes
int getValidatedInt(string);

int main()
{
    int coupons = 0,      // number of coupons
        candyBars = 0,    // number of candy bars
        gumballs = 0,     // number of gumballs
        leftover = 0;     // number of leftover coupons
    
    // Get the number of coupons from the user.
    coupons = getValidatedInt("Enter the number of coupons you have: ");
    
    // Because this is more fun than reprompting
    if (coupons < 0)
    {
        cout << endl << "You are " << abs(static_cast<float>(coupons)) 
             << " coupons in debt." << endl
             << "You owe the arcade prizes!" << endl;
    }
    else if (coupons == 0)
    {
        cout << endl << "Sorry, but nothing in life is free!" << endl;
    }
    else if (coupons < 3)
    {
        cout << endl << "Sorry, you can't afford any of the prizes "
             << "with your measly " << coupons;
        if (coupons < 2) // 1 coupon
        {
            cout << " coupon!" << endl;
        }
        else             // 2 coupons
        {
            cout << " coupons!" << endl;
        }
    }
    else // User has at least 3 coupons.
    {
        leftover = coupons; // Use leftover variable for counter 
                            // so we can keep track of original number
        
        while (leftover >= 10)
        {
            candyBars++;   // increment counter by 1 each iteration
            leftover -= 10; // decrement leftover by 10 each iteration
        }
        
        while (leftover >= 3)
        {
            gumballs++;   // increment counter by 1 each iteration
            leftover -= 3; // decrement leftover by 3 each iteration
        }

        cout << endl << "Your " << coupons 
             << " coupons can be exchanged for..." << endl;
        
        // Only print candy bars if > 0
        if (candyBars > 0)
        {
            cout << "Candy bars:" << endl;
        }
        while (candyBars > 0)
        {
            int currentRow = min(candyBars, 13);
            // top of candy bars
            for (int i = 0; i < currentRow; i++)
            {
                cout << "CCCCC ";
            }
            cout << endl;
            
            // middle of candy bars
            for (int i = 0; i < currentRow; i++)
            {
                cout << "C   C ";
            }
            cout << endl;
            
            // bottom of candy bars
            for (int i = 0; i < currentRow; i++)
            {
                cout << "CCCCC ";
            }
            cout << endl << endl;
            candyBars -= currentRow;
        }

        // Only print gumballs if > 0
        if (gumballs > 0) 
        {
            cout << "Gumballs:" << endl;
            // top of gumballs
            for (int i = 0; i < gumballs; i++)
            {
                cout << " GGG  ";
            }
            cout << endl;
            // middle of gumballs
            for (int i = 0; i < gumballs; i++)
            {
                cout << "G   G ";
            }
            cout << endl;
            // bottom of gumballs
            for (int i = 0; i < gumballs; i++)
            {
                cout << " GGG  ";
            }
            cout << endl << endl;
        }
        
        // Only print leftover coupons if > 0
        if (leftover > 0) 
        {
            cout << "Coupons left over:" << endl;
            // top of coupons
            for (int i = 0; i < leftover; i++)
            {
                cout << " ____ ";
            }
            cout << endl;
            // middle of coupons
            for (int i = 0; i < leftover; i++)
            {
                cout << "/   | ";
            }
            cout << endl;
            // bottom of coupons
            for (int i = 0; i < leftover; i++)
            {
                cout << "----- ";
            }
            cout << endl;
        }
    }
    
    return 0;
}

/*  Gets an integer value from the console.
 *  Prints the specified prompt before accepting input.
 *  Performs basic input validation to ensure a valid integer.
 */
int getValidatedInt(string prompt)
{
    int input = 0; // for storing user input
    
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
             << numeric_limits<int>::max()  // Max int
             << endl << "without any commas: ";
        cin.clear();        // Clear any errors.
        // Ignore any remaining input in buffer, up to the max buffer size.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> input;    // Store console input in 'input' variable.
    }
    
    return input; // Send the input value back to the caller.
}

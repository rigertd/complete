/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           11/17/2014
 * Last Modification Date: 11/17/2014
 * Assignment:             Assignment 8
 * Filename:               findMode.cpp
 *
 * Overview:
 *     This program prompts the user for integers until a sentinel 
 *     value is entered, finds the mode(s) of all values entered,
 *     and displays the results.
 * 
 * Input:
 *     integer values
 *     sentinel value (blank line)
 *
 * Output:
 *     The output to the console will be in the form of:
 *
 ************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <limits>

using namespace std;

// function prototypes
// Finds the mode(s) of the ints in the specified vector.
vector<int> findMode(const vector<int> &);
// Determines whether a string is a valid integer.
bool isInteger(const string &);

int main()
{
    // declare variables
    vector<int> input; // stores user input
    string buffer;     // stores unvalidated input
    
    cout << "This program finds the mode(s) of a list of integer values.\n"
         << "Press Enter without any value when you are done.\n\n";
         
    do
    {
        cout << "Enter a value: ";
        getline(cin, buffer);
        
        // check if valid input
        if (isInteger(buffer))
        {
            // add to vector
            input.push_back(atoi(buffer.c_str()));
        }
        else if (!buffer.empty())
        {
            // invalid input
            cout << "Invalid integer. Try again.\n";
        }
    } while (!buffer.empty());
    
    if (!input.empty()) // non-empty
    {
        // get the mode(s)
        vector<int> result = findMode(input);
        
        // display result(s)
        if (result.size() > 1)
        {
            // fencepost
            cout << "The modes are " << result[0];
            for (int i = 1; i < result.size(); i++)
            {
                if (i == result.size() - 1)
                    cout << ", and " << result[i] << ".\n";
                else
                    cout << ", " << result[i];
            }
        }
        else if (result.size() == 1)
        {
            cout << "The mode is " << result[0] << ".\n";
        }
    }
    else              // empty
    {
        cout << "You did not enter any integers.\n";
    }
    
    return 0;
}

/**
    bool isInteger(const string &str)
    
    Purpose:
        This function determines whether a string is a valid integer.
    
    Preconditions:
        none
    
    Postconditions:
        returns true if string only contains 0-9, false otherwise
 */
bool isInteger(const string &str)
{
    // return false if empty
    if (str.length() < 1)
        return false;
    
    // check each char
    for (int i = 0; i < str.length(); i++)
    {
        if (!isdigit(str[i]))
            return false;
    }
    
    // no non-digits found
    return true;
}

/**
    void printTotal(const Item cart[], const int count)
    
    Purpose:
        This function shows the total cost of all items
        in the specified array.
    
    Preconditions:
        cart is initialized and contains count Items
    
    Postconditions:
        total cost is displayed in console window
 */
vector<int> findMode(const vector<int> &vals)
{
    // keep track of highest and lowest values
    int high = numeric_limits<int>::min(),
        low = numeric_limits<int>::max(); 
    // keep track of highest count
    short highest = 0;
    
    // find the lowest and highest values entered
    for (int i = 0; i < vals.size(); i++)
    {
        high = max(high, vals[i])
    }
    // create arrays for counting all possible int values
    // (surely there must be a better way to do this without a dictionary)
    short *negCount = new short[-MAX_NEG_INT];
    short *posCount = new short[MAX_POS_INT];
    
    cout << "Initializing count array\n";
    // initialize counting arrays
    for (int i = 0; i < MAX_POS_INT; i++)
    {
        posCount[i] = 0;
        negCount[i] = 0;
    }
    cout << "End of count array init.\n";
    negCount[MAX_NEG_INT - 1] = 0;
    
    cout << "Incrementing count array\n";
    // increment array counters based on vector values
    // also find highest count.
    for (int i = 0; i < vals.size(); i++)
    {
        int index;
        if (vals[i] < 0)
        {
            index = -vals[i];
            negCount[index]++;
            highest = static_cast<short>(
                max(negCount[index], highest));
        }
        else
        {
            index = vals[i];
            posCount[index]++;
            highest = static_cast<short>(
                max(posCount[index], highest));
        }
    }
    
    // add most common vals to result vector
    cout << "Declaring result array\n";
    vector<int> results;
    cout << "Populating result array\n";
    for (int i = 0; i < MAX_POS_INT; i++)
    {
        if (posCount[i] == highest)
            results.push_back(i);
        if (negCount[i] == highest)
            results.push_back(-i);
    }
    if (negCount[MAX_NEG_INT - 1] == highest)
        results.push_back(MAX_NEG_INT - 1);
    
    // delete arrays
    delete [] posCount;
    delete [] negCount;
    
    return results;
}

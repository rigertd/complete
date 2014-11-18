/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           11/17/2014
 * Last Modification Date: 11/17/2014
 * Assignment:             Assignment 8
 * Filename:               carLot.cpp
 *
 * Overview:
 *     This program is a rudimentary menu-driven inventory management tool
 *     for a car dealer. The user can enter data on cars that are still on
 *     the lot and cars that have been sold. The user can also list unsold
 *     inventory and the profit for any given month.
 * 
 * Input:
 *     menu selection
 *     car and sale data
 *
 * Output:
 *     The output to the console will be in the form of:
 *
 *     This program finds the mode(s) of a list of integer values.
 *     Press Enter without any value when you are done.
 *
 *     Enter a value: [4]
 *     Enter a value: [2]
 *     Enter a value: [9]
 *     Enter a value: [2]
 *     Enter a value:
 *
 *     The mode is 2.
 ************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <limits>
#include <iomanip>
#include <cstdlib>

using namespace std;

// represents a date
struct Date
{
    int day,
        month,
        year;
};

// represents current or past inventory on a car dealer lot
struct Car
{
    string make,
           model;
    int year;
    Date datePurchased,
         dateSold;
    double purchasePrice,
           salePrice;
    bool isSold;
};

// function prototypes
// Prints the menu.
void printMenu();
// Adds a car entry to the system.
void addEntry(vector<Car> &);
// Lists all unsold cars in inventory.
void listInventory(const vector<Car> &);
// Prompts user for a month and year and prints the profit.
void printProfit(const vector<Car> &);
// Determines whether a string is a valid integer.
bool isInteger(const string &);
// Splits a string on the specified delimiter.
vector<string> split(const string &, char = ' ');

// global constants
const short MAKE_MAX_LEN = 20;
const short MODEL_MAX_LEN = 20;
const short DATE_LEN = 10;
const short PRICE_LEN = 10;
const double PRICE_MAX = 9999999.99;

int main()
{
    // declare variables
    vector<Car> ledger;   // inventory ledger
    string input;         // user input buffer
    short selection;      // holds menu selection
    
    // menu loop
    do
    {
        // show the menu
        printMenu();
        
        // get user selection
        cout << "Enter your selection: ";
        getline(cin, input);

        // parse selection
        if (!isInteger(input))
        {
            cout << "Invalid input. Please enter a number 1-4.\n";
        }
        else
        {
            // convert user input to short int
            selection = atoi(input.c_str());
            
            if (selection < 1 || selection > 4 )
            {
                // invalid range
                cout << "Invalid selection. Try again.\n";
                // reset menu selection
                selection = 0;
            }
            else if (selection == 1)
            {
                // add entry to ledger
                addEntry(ledger);
            }
            else if (selection == 2)
            {
                // list inventory on lot
                listInventory(ledger);
            }
            else if (selection == 3)
            {
                // show profit for specified month
                printProfit(ledger);
            }
        }
    } while (selection != 4 && !cin.fail());

    cout << "\nDestroying ledger. Thank you for wasting your time.\n";
    
    return 0;
}

/**
    void printMenu()
    
    Purpose:
        This function prints the menu to the console.
    
    Preconditions:
        None.
    
    Postconditions:
        Menu is printed to the console.
 */
void printMenu()
{
    cout << "\nChoose an item from the menu.\n\n"
         << "1) Add entry\n"
         << "2) List current inventory\n"
         << "3) Profit for a month\n"
         << "4) Quit\n\n";
}

// Adds a car entry to the system.
void addEntry(vector<Car> &ledger)
{

}

// Lists all unsold cars in inventory.
void listInventory(const vector<Car> &ledger)
{
    unsigned int unsold = 0;
    
    cout << "\nYou have the following unsold cars on the lot:\n\n"
         << left << setw(MAKE_MAX_LEN + 2) << "Make"
         << setw(MODEL_MAX_LEN + 2) << "Model"
         << setw(6) << "Year"
         << setw(DATE_LEN + 2) << "Purchased"
         << setw(PRICE_LEN + 3) << "Price Paid"
         << endl << string (75, '-') << endl;
    for (int i = 0; i < ledger.size(); i++)
    {
        // print only if not sold
        if (!ledger[i].isSold)
        {
            cout << left << setw(MAKE_MAX_LEN + 2) << ledger[i].make
                 << setw(MODEL_MAX_LEN + 2) << ledger[i].model
                 << setw(6) << ledger[i].year
                 << ledger[i].datePurchased.month << "/"
                 << ledger[i].datePurchased.day << "/"
                 << setw(6) << ledger[i].datePurchased.year
                 << "$" << right << setw(PRICE_LEN + 2) << ledger[i].purchasePrice
                 << endl;
            
            unsold++;
        }
    }
    cout << "\nYou have " << unsold << " unsold car"
         << (unsold == 1 ? " " : "s ") << "on the lot.\n\n";
}

// Prompts user for a month and year and prints the profit.
/**
    void printProfit(const vector<Car> &ledger)
    
    Purpose:
        This function prompts the user for a month and year and prints the
        total profit from all cars sold within the specified month and year.
    
    Preconditions:
        ledger is initialized
    
    Postconditions:
        total cost is displayed in console window
 */
void printProfit(const vector<Car> &ledger)
{
    double profit = 0.0;       // total profit for month
    string input;              // user input buffer
    vector<string> monthYear;  // split month and year
    short month = 0, year = 0; // numerical month and year
    
    do
    {
        cout << "Enter the month and year as MM/YYYY: ";
        getline(cin, input);
        
        // split on slash
        monthYear = split(input, '/');
        
        // validate input length
        if (monthYear.size() != 2)
            cout << "Invalid date format.\n";
        else
        {
            // validate month
            if (isInteger(monthYear[0]))
            {
                month = atoi(monthYear[0].c_str());
                if (month < 1 || month > 12)
                    cout << "Invalid month. Enter a number between 1 and 12.\n";
            }
            else
                cout << "Invalid month.\n";
            // validate year
            if (isInteger(monthYear[1]))
            {
                year = atoi(monthYear[1].c_str());
                if (year < 1 || year > 9999)
                    cout << "Invalid year. Enter a number between 1 and 9999.\n";
            }
            else
                cout << "Invalid year.\n";
        }
    } while (month < 1 || month > 12 || year < 1 || year > 9999);
    
    for (int i = 0; i < ledger.size(); i++)
    {
        // only count if correct month and year
        if (ledger[i].dateSold.month == month && 
            ledger[i].dateSold.year == year)
            profit += ledger[i].salePrice - ledger[i].purchasePrice;
    }
    
    cout << "\nYour total profit for " << month << "/" << year << " was $"
         << fixed << showpoint << setprecision(2) << profit << ".\n";
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
        if (!isdigit(str[i]) && (i != 0 || str[i] != '-'))
            return false;
    }
    
    // no non-digits found
    return true;
}

// Splits a string on the specified delimiter.
/**
    vector<string> split(const string &text, char delim = ' ')
    
    Purpose:
        This function splits a string into substrings based on 
        the specified delimiter. It defaults to a space if no
        delimiter is specified.
    
    Preconditions:
        none
    
    Postconditions:
        returns a vector containing the substrings
 */
vector<string> split(const string &text, char delim)
{
    vector<string> buffer;
    size_t pos = 0; // find position
    
    while (pos < text.length())
    {
        size_t next = text.find(delim, pos);
        if (next == string::npos)
            next = text.length();
        buffer.push_back(text.substr(pos, next - pos));
        pos = next + 1;
    }
    
    return buffer;
}

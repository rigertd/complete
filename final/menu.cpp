#include "Book.h"
#include "Patron.h"
#include "Library.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

/********************************************************************
 *  function prototypes
 *******************************************************************/
// Prints the menu to the console.
void printMenu();
// Prompts the user to press ENTER to continue.
void enterToContinue();
// Determines whether a string is a valid double.
bool isDouble(const string &);
// Determines whether a string is a valid integer.
bool isInteger(const string &);

int main()
{
    Library lib;          // instantiate the library
    string input;         // user input buffer
    int selection;        // menu selection
    string memID, bookID; // member ID and book ID input buffer
    string payment;       // payment amount input buffer
    
    // print welcome message
    cout << "Welcome to the library.\n";
    
    // start of menu loop
    do
    {
        // reset selection variable
        selection = 0;

        // print menu options
        printMenu();
        
        // prompt user for selection and store it in input
        cout << "Enter your selection: ";
        getline(cin, input);

        // set selection if entire input string is valid integer
        if (isInteger(input))
            selection = atoi(input.c_str());

        switch (selection)
        {
            case 1: // add book to catalog
                lib.addBook();
                break;
            case 2: // add member to roster
                lib.addMember();
                break;
            case 3: // check out a book
                cout << "Checking out a book.\n"
                     << "Enter the member ID: ";
                getline(cin, memID);
                cout << "Enter the book ID: ";
                getline(cin, bookID);
                lib.checkOutBook(memID, bookID);
                break;
            case 4: // return a book
                cout << "Returning a book.\n"
                     << "Enter the book ID: ";
                getline(cin, bookID);
                lib.returnBook(bookID);
                break;
            case 5: // request a book
                cout << "Requesting a book.\n"
                     << "Enter the member ID: ";
                getline(cin, memID);
                cout << "Enter the book ID: ";
                getline(cin, bookID);
                lib.requestBook(memID, bookID);
                break;
            case 6: // pay a fine
                cout << "Paying a fine.\n"
                     << "Enter the member ID: ";
                getline(cin, memID);
                // print member info, including fine amount owed
                lib.viewPatronInfo(memID);
                cout << "Enter the amount to pay: ";
                getline(cin, payment);
                if (isDouble(payment))
                {
                    double amount = atof(payment.c_str());
                    lib.payFine(memID, amount);
                }
                else
                    cout << "Invalid amount.\n";
                break;
            case 7: // increment day counter
                cout << "Updating date to next day.\n";
                lib.incrementCurrentDate();
                break;
            case 8: // view member info
                cout << "Looking up information on a member.\n"
                     << "Enter the member ID: ";
                getline(cin, memID);
                lib.viewPatronInfo(memID);
                break;
            case 9: // view book info
                cout << "Looking up information on a book.\n"
                     << "Enter the book ID: ";
                getline(cin, bookID);
                lib.viewBookInfo(bookID);
                break;
            case 10: // exit program
                cout << "Thank you for using your local library.\n";
                break;
            default: // invalid input
                cout << "Invalid selection. Try again.\n";
        }
        
        // make user press ENTER to continue.
        enterToContinue();
    } while (selection != 10);
}

/********************************************************************
 *  void printMenu()
 *
 *  Purpose: This function prints the menu to the console.
 *
 *  Preconditions: none
 *
 *  Postconditions: Menu is printed to the console.
 ********************************************************************/
void printMenu()
{
    cout << "\nChoose an item from the menu below.\n\n"
         << "  1) Add a book to the library\n"
         << "  2) Add a member to the library\n"
         << "  3) Check out a book\n"
         << "  4) Return a book\n"
         << "  5) Request a book\n"
         << "  6) Pay an outstanding fine\n"
         << "  7) Increment day counter (run once per day)\n"
         << "  8) Look up information on a member\n"
         << "  9) Look up information on a book\n"
         << " 10) Exit the program\n\n";
}

/********************************************************************
 *  void printMenu()
 *
 *  Purpose: This function prompts the user to press ENTER and waits
 *           for any input followed by ENTER.
 *
 *  Preconditions: none
 *
 *  Postconditions: Blocks until ENTER is pressed.
 ********************************************************************/
void enterToContinue()
{
    string input;
    cout << "\nPress ENTER to continue...";
    getline(cin, input);
}

/********************************************************************
 *  bool isDouble(const string &str)
 *
 *  Purpose: This function determines whether a string is a valid double.
 *           Note that a valid integer is considered to be a valid double.
 *
 *  Preconditions: none
 *
 *  Postconditions: Returns true if entire string is valid double.
 ********************************************************************/
bool isDouble(const string &str)
{
    // return false if empty
    if (str.length() < 1)
        return false;
    
    bool pointFound = false;
    
    // check each char
    for (int i = 0; i < str.length(); i++)
    {
        if ( !isdigit(str[i]) &&           // not a digit
            (str[i] != '-' || i != 0) &&   // '-' found not at beginning
            (str[i] != '.' || pointFound)  // '.' when one already found
           )
            return false;
        if (str[i] == '.')
            pointFound = true;
    }
    
    // no illegal chars found
    return true;
}

/********************************************************************
 *  bool isInteger(const string &str)
 *
 *  Purpose: This function determines whether a string is a valid integer.
 *
 *  Preconditions: none
 *
 *  Postconditions: Returns true if entire string is valid integer.
 ********************************************************************/
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

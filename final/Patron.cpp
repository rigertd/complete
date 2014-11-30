#include "Patron.h"
#include <iostream>
#include <string>
#include <stdexcept>

// define NULL if not already defined
#ifndef NULL
#define NULL \0
#endif

Patron::Patron()
{
    fineAmount = 0.0;   // no fine
    // checkedOutBooks empty by default
}

Patron::Patron(std::string idn, std::string n)
{
    idNum = idn;        // ID number of patron
    name = n;           // name of patron
    fineAmount = 0.0;   // no fine
    // checkedOutBooks empty by default
}

void Patron::addBook(Book* b)
{
    // throw exception if null pointer
    if (b = NULL)
        throw std::invalid_argument("ERROR: Book object not found.");
        
    // check if already checked out based on idCode
    for (int i = 0; i < checkedOutBooks.size(); i++)
    {
        if (b.getIdCode() == checkedOutBooks[i].getIdCode())
        {
            std::cout << "The patron already has that book checked out.\n";
            return;
        }
    }
    
    // book not checked out if execution reaches here
    // add to checkedOutBooks
    checkedOutBooks.push_back(b);
}

void Patron::removeBook(Book* b)
{
    // throw exception if null pointer
    if (b = NULL)
        throw std::invalid_argument("ERROR: Book object not found.");
    
    // remove if found in checkedOutBooks
    for (int i = 0; i < checkedOutBooks.size(); i++)
    {
        if (b.getIdCode() == checkedOutBooks[i].getIdCode())
        {
            checkedOutBooks.erase(checkedOutBooks.begin() + i);
            return;
        }
    }
    
    // book not checked out if execution reaches here
    // print error message
    std::cout << "The patron does not have that book checked out.\n";
}

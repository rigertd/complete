#include "Book.h"
#include "Patron.h"
#include "Library.h"
#include <iostream>
#include <string>
#include <iomanip>

// define NULL if not already defined
#ifndef NULL
#define NULL \0
#endif

Library::Library()
{
    currentDate = 0;
}

int Library::findBook(std::string bookId)
{
    // return first subscript of bookId found in holdings
    for (int i = 0; i < holdings.size(); i++)
    {
        if (holdings[i].getIdCode() == bookId)
            return i;
    }
    
    // return -1 to indicate not found
    return -1;
}

int Library::findMember(std::string memberId)
{
    // return first subscript of memberId found in holdings
    for (int i = 0; i < members.size(); i++)
    {
        if (members[i].getIdNum() == memberId)
            return i;
    }
    
    // return -1 to indicate not found
    return -1;
}

void Library::addBook()
{
    std::string idCode, title, author;  // user input
    
    // prompt user for book ID
    std::cout << "\nEnter the book ID: ";
    std::getline(std::cin, idCode);
    
    // check if ID is already in use
    if (findBook(idCode) >= 0)
    {
        std::cout << "\nThat ID is already in use.\n";
        return;
    }
    
    // prompt user for book title
    std::cout << "Enter the book title: ";
    std::getline(std::cin, title);
    
    // prompt user for book author
    std::cout << "Enter the book author: ";
    std::getline(std::cin, author);
    
    // add book object to holdings
    std::cout << "\nAdding \"" << title << "\" to records.\n";
    holdings.push_back(Book (idCode, title, author));
}

void Library::addMember()
{
    std::string idNum, name;    // user input
    
    // prompt user for patron ID
    std::cout << "\nEnter the member ID: ";
    std::getline(std::cin, idNum);
    
    // check if ID is already in use
    if (findMember(idNum) >= 0)
    {
        std::cout << "\nThat ID is already in use.\n";
        return;
    }
    
    // prompt user for patron name
    std::cout << "Enter the member name: ";
    std::getline(std::cin, name);
    
    // add patron object to members
    std::cout << "\nAdding " << name << " to records.\n";
    members.push_back(Patron (idNum, name));
}

void Library::checkOutBook(std::string patronID, std::string bookID)
{
    // declare and set to indices of book and patron
    int bIndex = findBook(bookID),
        mIndex = findMember(patronID);
    
    // validate patronID and bookID
    if (bIndex < 0)
    {
        std::cout << "\nThat book ID was not found in the library database.\n";
        return;
    }
    if (mIndex < 0)
    {
        std::cout << "\nThat member ID was not found in the library database."
                  << "\n";
        return;
    }
    
    // check current location
    switch (holdings[bIndex].getLocation())
    {
        case CHECKED_OUT:
            std::cout << "\nThat book is already checked out.\n";
            return;
        case ON_HOLD:
            // check if on hold by requester
            if (holdings[bIndex].getRequestedBy()->getIdNum() != patronID)
            {
                std::cout << "\nThat book is on hold by another member.\n";
                return;
            }
            // on hold by requester. reset requestedBy
            holdings[bIndex].setRequestedBy(NULL);
            // here we intentionally fall through
        case ON_SHELF:
            // update location to CHECKED_OUT
            holdings[bIndex].setLocation(CHECKED_OUT);
            // update checkedOutBy to requester
            holdings[bIndex].setCheckedOutBy(&(members[mIndex]));
            // update dateCheckedOut to currentDate
            holdings[bIndex].setDateCheckedOut(currentDate);
            // add pointer to patron checkedOutBooks
            members[mIndex].addBook(&(holdings[bIndex]));
            // print confirmation message
            std::cout << "\n\"" << holdings[bIndex].getTitle() << "\""
                      << " is now checked out to "
                      << members[mIndex].getName() << ".\n";
    }
}

void Library::returnBook(std::string bookID)
{
    // declare variable and set to book index
    int bIndex = findBook(bookID);
    
    // validate bookID
    if (bIndex < 0)
    {
        std::cout << "\nThat book ID was not found in the library database.\n";
        return;
    }
    
    // validate current location
    if (holdings[bIndex].getLocation() != CHECKED_OUT)
    {
        std::cout << "\nThat book is not checked out right now.\n";
        return;
    }
    
    // remove book from current patron checkedOutBooks
    holdings[bIndex].getCheckedOutBy()->removeBook(&(holdings[bIndex]));
    
    // check if requested by another patron
    if (holdings[bIndex].getRequestedBy() != NULL)
        holdings[bIndex].setLocation(ON_HOLD);
    else
        holdings[bIndex].setLocation(ON_SHELF);
    
    // print confirmation message
    std::cout << "\n\"" << holdings[bIndex].getTitle() << "\""
              << " has been returned to the library.\n";
}

void Library::requestBook(std::string patronID, std::string bookID)
{
    // declare and set to indices of book and patron
    int bIndex = findBook(bookID),
        mIndex = findMember(patronID);
        
    // validate patronID and bookID
    if (bIndex < 0)
    {
        std::cout << "\nThat book ID was not found in the library database.\n";
        return;
    }
    if (mIndex < 0)
    {
        std::cout << "\nThat member ID was not found in the library database."
                  << "\n";
        return;
    }

    // check if already requested
    if (holdings[bIndex].getRequestedBy() != NULL)
    {
        std::cout << "\nThat book is already requested by another member.\n";
        return;
    }
    else if (holdings[bIndex].getCheckedOutBy() != NULL
             && holdings[bIndex].getCheckedOutBy()->getIdNum() == patronID)
    {
        // cannot request book that is checked out by requester
        std::cout << "\nA member cannot request a book that he or she "
                  << "already has checked out.\n";
        return;
    }
    
    // set requestedBy of book to requester
    holdings[bIndex].setRequestedBy(&(members[mIndex]));
    
    // update location only if ON_SHELF
    if (holdings[bIndex].getLocation() == ON_SHELF)
        holdings[bIndex].setLocation(ON_HOLD);
    
    // print confirmation message
    std::cout << "\n\"" << holdings[bIndex].getTitle() << "\""
              << " is now on request by "
              << members[mIndex].getName() << ".\n";
}

void Library::incrementCurrentDate()
{
    currentDate++;
    for (int i = 0; i < members.size(); i++)
    {
        double fineAmt = 0.0,                           // new fines
               currentAmt = members[i].getFineAmount(); // current fines
        // store return value to minimize function calls
        std::vector<Book*> checkedOut = members[i].getCheckedOutBooks();
        
        // loop through all checked out books and check for overdue books
        for (int j = 0; j < checkedOut.size(); j++)
        {
            // add daily fine to total for each overdue book
            if (currentDate - (checkedOut[j])->getDateCheckedOut() > 
                Book::CHECK_OUT_LENGTH)
                fineAmt += DAILY_FINE;
        }
        // add new fine to current fine and amend patron total fine
        members[i].amendFine(currentAmt + fineAmt);
    }
}

void Library::payFine(std::string patronID, double payment)
{
    // declare variable and set to patron index
    int mIndex = findMember(patronID);
    
    // validate patronID
    if (mIndex < 0)
    {
        std::cout << "\nThat member ID was not found in the library database."
                  << "\n";
        return;
    }

    // get amount owed by patron
    double amountOwed = members[mIndex].getFineAmount();
    // apply payment to amountOwed
    members[mIndex].amendFine(amountOwed - payment);
    // print confirmation message
    std::cout << std::fixed << std::showpoint << std::setprecision(2)
              << "\nFines for " << members[mIndex].getName() << " are now $"
              << members[mIndex].getFineAmount() << ".\n";
}

void Library::viewPatronInfo(std::string patronID)
{
    // declare variable and set to patron index
    int mIndex = findMember(patronID);
    
    // validate patronID
    if (mIndex < 0)
    {
        std::cout << "\nThat member ID was not found in the library database."
                  << "\n";
        return;
    }

    // print patron ID and name
    std::cout << "\nInformation on member ID " << members[mIndex].getIdNum()
              << "\n  Name: " << members[mIndex].getName() 
              << "\n  Books checked out:\n";
    // store return value to minimize function calls
    std::vector<Book*> checkedOut = members[mIndex].getCheckedOutBooks();
    // print book info for each book, or message if none checked out
    if (checkedOut.size() > 0)
        for (int i = 0; i < checkedOut.size(); i++)
        {
            std::cout << "\n  Book ID:  " << checkedOut[i]->getIdCode()
                      << "\n  Title:    " << checkedOut[i]->getTitle()
                      << "\n  Author:   " << checkedOut[i]->getAuthor()
                      << "\n\n";
        }
    else
        std::cout << "  None\n";
    
    // print fine amount, if any
    double fine = members[mIndex].getFineAmount();
    if (fine >= 0.01)
        std::cout << std::fixed << std::showpoint << std::setprecision(2)
                  << "  This member owes $" << fine << " in fines.\n";
    else
        std::cout << "  This member does not owe any fines.\n";
}

void Library::viewBookInfo(std::string bookID)
{
    // declare variable and set to book index
    int bIndex = findBook(bookID);
    
    // validate bookID
    if (bIndex < 0)
    {
        std::cout << "\nThat book ID was not found in the library database.\n";
        return;
    }
    
    // print book ID, title and author
    std::cout << "\nInformation on book ID " << holdings[bIndex].getIdCode()
              << "\n  Title:    " << holdings[bIndex].getTitle()
              << "\n  Author:   " << holdings[bIndex].getAuthor();
    
    // print book location
    std::cout << "\n  Location: ";
    switch (holdings[bIndex].getLocation())
    {
        case CHECKED_OUT:
            std::cout << "Checked out\n";
            break;
        case ON_HOLD:
            std::cout << "On hold\n";
            break;
        case ON_SHELF:
            std::cout << "On the shelf\n";
    }
    
    std::cout << std::endl;
    
    // print requestedBy if book has been requested
    Patron *pPatron = holdings[bIndex].getRequestedBy();
    if (pPatron != NULL)
        std::cout << "  This book has been requested by "
                  << pPatron->getName() << ".\n";
    
    // print checkedOutBy and due date if book is checked out
    pPatron = holdings[bIndex].getCheckedOutBy();
    if (pPatron != NULL)
    {
        std::cout << "  This book is checked out by "
                  << pPatron->getName() << ".\n";
        
        // get due date and print appropriate message
        int due = holdings[bIndex].getDateCheckedOut() + Book::CHECK_OUT_LENGTH;
        if (due == currentDate)
            std::cout << "  It is due on day " << due << ", which is today.\n";
        else if (due > currentDate)
            std::cout << "  It is due on day " << due << ", "
                      << "which is in " << currentDate - due << " days.\n";
        else
            std::cout << "  It is due on day " << due << ", "
                      << "which was " << due - currentDate << " days ago.\n";
    }
}

/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           11/29/2014
 * Last Modification Date: 11/30/2014
 * Assignment:             Final Project
 * Filename:               Book.cpp
 *
 * Overview:
 *     The Book class represents a Book in the Library class.
 *     This file provides the implementation details and documentation for
 *     the functions defined in Book.h.
 ************************************************************************/
#include "Book.h"
#include <string>

// define NULL if not already defined
#ifndef NULL
#define NULL \0
#endif

/********************************************************************
 *  Default constructor. Sets pointers to NULL and location is undefined.
 *******************************************************************/
Book::Book()
{
    checkedOutBy = NULL;    // not checked out
    requestedBy = NULL;     // not requested
    dateCheckedOut = 0;     // not checked out
    // idCode, title, and author are empty strings
    // location is undefined
}

/********************************************************************
 *  Three argument constructor. Sets idCode, title, and author to the
 *  specified values, and sets location to ON_SHELF.
 *******************************************************************/
Book::Book(std::string idc, std::string t, std::string a)
{
    idCode = idc;           // ID code of book
    title = t;              // title of book
    author = a;             // author of book
    checkedOutBy = NULL;    // not checked out
    requestedBy = NULL;     // not requested
    dateCheckedOut = 0;     // not checked out
    location = ON_SHELF;    // on the shelf
}

// implementation file documentation

/********************************************************************
 *  std::string Book::getIdCode()
 *
 *  Purpose: This function gets the idCode of the Book object.
 *
 *  Preconditions: none
 *
 *  Postconditions: Returns idCode value.
 *******************************************************************/
    
/********************************************************************
 *  std::string Book::getTitle()
 *
 *  Purpose: This function gets the title of the Book object.
 *
 *  Preconditions: none
 *
 *  Postconditions: Returns title value.
 *******************************************************************/
    
/********************************************************************
 *  std::string Book::getAuthor()
 *
 *  Purpose: This function gets the author of the Book object.
 *
 *  Preconditions: none
 *
 *  Postconditions: Returns author value.
 *******************************************************************/

/********************************************************************
 *  Locale Book::getLocation()
 *
 *  Purpose: This function gets the location of the Book object.
 *
 *  Preconditions: none
 *
 *  Postconditions: Returns location value (ON_SHELF, ON_HOLD, CHECKED_OUT).
 *******************************************************************/

/********************************************************************
 *  void Book::setLocation(Locale lo)
 *
 *  Purpose: This function sets the title of the Book object.
 *           The supported enumerators are: ON_SHELF, ON_HOLD, CHECKED_OUT
 *
 *  Preconditions: none
 *
 *  Postconditions: The location value is set to lo.
 *******************************************************************/
    
/********************************************************************
 *  Patron* Book::getCheckedOutBy()
 *
 *  Purpose: This function gets a pointer to the Patron object that has
 *           the Book object checked out.
 *
 *  Preconditions: none
 *
 *  Postconditions: Returns a pointer to a Patron object.
 *******************************************************************/

/********************************************************************
 *  void Book::setCheckedOutBy(Patron* p)
 *
 *  Purpose: This function sets a pointer to the Patron object that has
 *           the Book object checked out.
 *
 *  Preconditions: none
 *
 *  Postconditions: The checkedOutBy value is set to p.
 *******************************************************************/
    
/********************************************************************
 *  Patron* Book::getRequestedBy()
 *
 *  Purpose: This function gets a pointer to the Patron object that has
 *           the Book object on request.
 *
 *  Preconditions: none
 *
 *  Postconditions: Returns a pointer to a Patron object.
 *******************************************************************/

/********************************************************************
 *  void Book::setRequestedBy(Patron* p)
 *
 *  Purpose: This function sets a pointer to the Patron object that has
 *           the Book object on request.
 *
 *  Preconditions: none
 *
 *  Postconditions: The requestedBy value is set to p.
 *******************************************************************/

/********************************************************************
 *  int Book::getDateCheckedOut()
 *
 *  Purpose: This function gets the day value on which the Book object
 *           was checked out.
 *
 *  Preconditions: none
 *
 *  Postconditions: Returns the checked out date value as an integer.
 *******************************************************************/

/********************************************************************
 *  void Book::setDateCheckedOut(int d)
 *
 *  Purpose: This function sets the day value on which the Book object
 *           was checked out.
 *
 *  Preconditions: none
 *
 *  Postconditions: The dateCheckedOut value is set to d.
 *******************************************************************/

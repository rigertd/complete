#include "Book.h"
#include <string>

// define NULL if not already defined
#ifndef NULL
#define NULL \0
#endif

Book::Book()
{
    checkedOutBy = NULL;    // not checked out
    requestedBy = NULL;     // not requested
    dateCheckedOut = 0;     // not checked out
    // idCode, title, and author are empty strings
    // location is undefined
}

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

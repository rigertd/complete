/*************************************************************************
 * Author:          David Rigert
 * Date Created:    1/10/2015
 * Last Modified:   1/13/2015
 * Assignment:      Assignment 1
 * Filename:        utility.hpp
 *
 * Description:     A collection of utility functions for common tasks. 
 ************************************************************************/
#ifndef UTILITY_HPP
#define UTILITY_HPP

/********************************************************************
 *  Function:       void clearInputBuffer()
 *  Description:    Clears the keyboard buffer in the console window.
 *  Parameters:     none
 *  Preconditions:  Keyboard buffer has input (or blocks until input received).
 *  Postconditions: Keyboard buffer and fail bit are cleared.
 *******************************************************************/
void clearInputBuffer();

/********************************************************************
 *  Function:       void clearWindow()
 *  Description:    Clears the console window.
 *  Parameters:     none
 *  Preconditions:  none
 *  Postconditions: Console window is cleared and cursor is at top-left.
 *******************************************************************/
void clearWindow();

/********************************************************************
 *  Function:       void moveCursorToTopLeft()
 *  Description:    Moves the cursor to the top-left corner of the console.
 *  Parameters:     none
 *  Preconditions:  none
 *  Postconditions: Cursor is at top-left corner of console window.
 *******************************************************************/
void moveCursorToTopLeft();

/********************************************************************
 *  Function:       void sleepMilliseconds(int milliseconds)
 *  Description:    Sleeps for the specified number of milliseconds.
 *  Parameters:     none
 *  Preconditions:  none
 *  Postconditions: none
 *******************************************************************/
void sleepMilliseconds(int);

#endif // end of define UTILITY_HPP
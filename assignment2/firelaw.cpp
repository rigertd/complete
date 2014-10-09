/**********************************************************
* Author:                 David Rigert
* Date Created:           10/8/2014
* Last Modification Date: 10/8/2014
* Assignment:             Assignment 2
* Filename:               firelaw.cpp
*
* Overview:
*     This program prompts the user for the room capacity
*     and the number of participants in a meeting.
*     Based on the input, it tells the user whether the meeting
*     Can be legally held and how many over or under the limit
*     the number of participants is.
* 
* Input:
*     Room capacity
*     Meeting attendees
*
* Output:
*     The output to the console will be in the form of:
*
*     Enter the maximum room capacity: 
*     Enter the number of attendees: 
*
*     If attendees > capacity,
*         Fire regulations prohibit holding the meeting.
*         You have # too many attendees.
*     If attendees <= capacity,
*         You can hold the meeting as planned.
*         There is still space for # more attendees.
**********************************************************/
#include <iostream>
#include <limits>
using namespace std;

int main()
{
    short capacity = 0,  // Maximum room capacity. Initialize to 0.
          attendees = 0; // Number of attendees. Initialize to 0.
    
    cout << "Enter the maximum room capacity: ";
    cin >> capacity;  // Store console input in 'capacity' variable.
    
    /* Basic input validation.
     * The program will only accept input that is an integer.
     * It does this by detecting the next character in the cin buffer.
     * Unless the next char is a newline, 
     * it means there is unused data still in the buffer.
     */
    while (cin.get() != '\n') // true if next char not newline
    {
        cout << endl << "That is not a valid number. " << endl 
             << "Please enter a whole number between 1 and "
             << numeric_limits<short>::max()  // Max short
             << endl << "without any commas: ";
        cin.clear();        // Clear any errors.
        // Ignore any remaining input in buffer, up to the max buffer size.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> capacity;    // Store console input in 'capacity' variable.
    }
    
    cout << endl << "Enter the number of attendees: ";
    cin >> attendees;  // Store console input in 'attendees' variable.
    
    /* Basic input validation.
     * Same as above.
     */
    while (cin.get() != '\n') // true if next char not newline
    {
        cout << endl << "That is not a valid number. " << endl 
             << "Please enter a whole number between 1 and "
             << numeric_limits<short>::max() // Max short
             << " without any commas: ";
        cin.clear();         // Clear any errors.
        // Ignore any remaining input in buffer, up to the max buffer size.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> attendees;    // Store console input in 'attendees' variable.
    }
    
    if (attendees == 0)
    {
        cout << endl << "You cannot have a meeting without any attendees!"
             << endl;
    }
    else if (capacity == 0)
    {
        cout << endl << "What kind of meeting room doesn't hold any people? "
             << "A closet?" << endl;
    }
    else if (attendees < 0 || capacity < 0)
    {
        cout << endl << "Sorry, fire regulations don't do negatives." << endl;
    }
    else if (attendees > capacity)
    {
        cout << endl << "Fire regulations prohibit holding the meeting."
             << endl << "You have " << (attendees - capacity) 
             << " too many attendees." << endl; 
    }
    else // attendees < capacity
    {
        cout << endl << "You can hold the meeting as planned."
             << endl << "There is still space for " << (capacity - attendees)
             << " more attendees." << endl;
    }
    
    return 0;
}


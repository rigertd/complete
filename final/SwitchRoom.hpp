/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           3/14/2015
 * Last Modification Date: 3/14/2015
 * Course:                 CS162_400
 * Assignment:             Final Project
 * Filename:               SwitchRoom.hpp
 *
 * Overview:
 *     Represents a room that can toggle the state of a different ConditionRoom
 *     when the correct item is used in it.
 ************************************************************************/
#ifndef SWITCHROOM_HPP
#define SWITCHROOM_HPP

#include <iostream>
#include "Room.hpp"

class SwitchRoom : public Room
{
    friend class UnitTest;  // for unit testing
public:
    // constructor
    SwitchRoom(World * = NULL);
    
    // destructor
    virtual ~SwitchRoom()
    {
        // memory leak check
        std::cout << "Room ID " << id << " is being destroyed.\n";
    }
};
#endif  // end of SWITCHROOM_HPP definition

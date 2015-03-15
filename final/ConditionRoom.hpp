/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           3/14/2015
 * Last Modification Date: 3/14/2015
 * Course:                 CS162_400
 * Assignment:             Final Project
 * Filename:               ConditionRoom.hpp
 *
 * Overview:
 *     Represents a conditional room or area in the world. This room can
 *     be toggled between two states by using an item in it, or by using
 *     an item in a SwitchRoom that is linked to it.
 ************************************************************************/
#ifndef CONDITIONROOM_HPP
#define CONDITIONROOM_HPP

#include <iostream>
#include "Room.hpp"

class ConditionRoom : public Room
{
    friend class UnitTest;  // for unit testing
public:
    // constructor
    ConditionRoom();
    
    // destructor
    virtual ~ConditionRoom()
    {
        // memory leak check
        std::cout << "Room ID " << id << " is being destroyed.\n";
    }
};
#endif  // end of CONDITIONROOM_HPP definition

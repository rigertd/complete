/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           3/14/2015
 * Last Modification Date: 3/14/2015
 * Course:                 CS162_400
 * Assignment:             Final Project
 * Filename:               SwitchRoom.hpp
 *
 * Overview:
 *     Implementation for SwitchRoom class.
 ************************************************************************/
#include "SwitchRoom.hpp"

// constructor
SwitchRoom::SwitchRoom(World *w) : Room(w)
{
    description = "A room that switches the state of a different room.\n";
}
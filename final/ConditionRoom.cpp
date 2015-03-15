/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           3/14/2015
 * Last Modification Date: 3/14/2015
 * Course:                 CS162_400
 * Assignment:             Final Project
 * Filename:               ConditionRoom.hpp
 *
 * Overview:
 *     Implementation for ConditionRoom class.
 ************************************************************************/
#include "ConditionRoom.hpp"

// constructor
ConditionRoom::ConditionRoom(World *w) : Room(w)
{
    description = "A new conditional room that switches between two states.\n";
}
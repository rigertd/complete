/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/7/2015
 * Last Modified:   2/27/2015
 * Assignment:      Assignment 4
 * Filename:        ReptilePeople.cpp
 *
 * Description:     Implementation of the ReptilePeople class.
 ************************************************************************/
#include "ReptilePeople.hpp"

/*============================ Constructors ============================*/
/*************************************************************************
 *  Function:       ReptilePeople::ReptilePeople(std::string name)
 *  Description:    Invokes the parent class constructor with values for
 *                  a reptile person.
 *  Parameters:     name    name of reptile person
 ************************************************************************/
ReptilePeople::ReptilePeople(std::string name)
    : Character(
                name + " the Reptile",
                6,  // attack die sides
                3,  // attack die rolls
                6,  // defense die sides
                1,  // defense die rolls
                7,  // armor
                18  // strengthPoints
               )
{ }

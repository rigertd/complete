/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/7/2015
 * Last Modified:   2/27/2015
 * Assignment:      Assignment 4
 * Filename:        Barbarian.cpp
 *
 * Description:     Implementation of the Barbarian class.
 ************************************************************************/
#include "Barbarian.hpp"

/*============================ Constructors ============================*/
/*************************************************************************
 *  Function:       Barbarian::Barbarian(std::string name)
 *  Description:    Invokes the parent class constructor with values for
 *                  a barbarian.
 *  Parameters:     name    name of barbarian
 ************************************************************************/
Barbarian::Barbarian(std::string name)
    : Character(
                name + " the Barbarian",
                6,  // attack die sides
                2,  // attack die rolls
                6,  // defense die sides
                2,  // defense die rolls
                0,  // armor
                12  // strengthPoints
               )
{ }

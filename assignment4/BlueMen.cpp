/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/7/2015
 * Last Modified:   2/27/2015
 * Assignment:      Assignment 4
 * Filename:        BlueMen.cpp
 *
 * Description:     Implementation of the BlueMen class.
 ************************************************************************/
#include "BlueMen.hpp"

/*============================ Constructors ============================*/
/*************************************************************************
 *  Function:       BlueMen::BlueMen(std::string name)
 *  Description:    Invokes the parent class constructor with values for
 *                  a Blue Men character.
 *  Parameters:     name    name of blue men
 ************************************************************************/
BlueMen::BlueMen(std::string name)
    : Character(
                name + " the Blue Men",
                10,  // attack die sides
                2,  // attack die rolls
                6,  // defense die sides
                3,  // defense die rolls
                3,  // armor
                12  // strengthPoints
               )
{ }

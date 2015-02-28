/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/7/2015
 * Last Modified:   2/27/2015
 * Assignment:      Assignment 4
 * Filename:        TheShadow.cpp
 *
 * Description:     Implementation of the TheShadow class.
 ************************************************************************/
#include <iostream>

#include "TheShadow.hpp"

/*============================ Constructors ============================*/
/*************************************************************************
 *  Function:       TheShadow::TheShadow(std::string name)
 *  Description:    Invokes the parent class constructor with values for
 *                  a TheShadow character.
 *  Parameters:     name    name of the Shadow
 ************************************************************************/
TheShadow::TheShadow(std::string name)
    : Character(
                name + " the Shadow",
                10,  // attack die sides
                2,  // attack die rolls
                10,  // defense die sides
                1,  // defense die rolls
                0,  // armor
                12  // strengthPoints
               )
{ }

/*======================= Public Member Functions ======================*/
/*************************************************************************
 *  Function:       void TheShadow::defense(int attackValue)
 *  Description:    Defends against an attack with a 90% chance of being
 *                  missed. Does not get a defense roll if hit.
 *  Parameters:     attackValue     attacker's attack roll
 *  Preconditions:  none
 *  Postconditions: strengthPoints is reduced if a 1 is rolled on 1d10
 ************************************************************************/
// opponent has 90% chance of missing
void TheShadow::defense(int attackValue)
{
    int defenseValue = getDefenseRoll();
    if (defenseValue == 1)
    {
        std::cout << "and lands a blow!\n";
        this->strengthPoints -= attackValue;
        
    }
    else
    {
        std::cout << "but the Shadow wasn't there!\n";
    }
}

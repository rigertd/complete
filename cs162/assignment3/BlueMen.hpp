/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/7/2015
 * Last Modified:   2/14/2015
 * Assignment:      Assignment 3
 * Filename:        BlueMen.hpp
 *
 * Description:     Represents a Blue Men character in a fantasy combat game.
 *                  Uses the default attack and defense functions of the
 *                  Character class.
 ************************************************************************/
#ifndef BLUE_MEN_HPP
#define BLUE_MEN_HPP

#include <string>
#include "Character.hpp"

class BlueMen : public Character
{
public:
    BlueMen(std::string);
    
    // use parent class member function
    void attack(Character &c1)
    {
        Character::attack(c1);
    }

    // use parent class member function
    void defense(int attackValue)
    {
        Character::defense(attackValue);
    }
};

#endif  // end of BLUE_MEN_HPP definition

/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/7/2015
 * Last Modified:   2/14/2015
 * Assignment:      Assignment 3
 * Filename:        Barbarian.hpp
 *
 * Description:     Represents a Barbarian character in a fantasy combat game.
 *                  Uses the default attack and defense functions of the
 *                  Character class.
 ************************************************************************/
#ifndef BARBARIAN_HPP
#define BARBARIAN_HPP

#include <string>
#include "Character.hpp"

class Barbarian : public Character
{
public:
    Barbarian(std::string);
    
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

#endif  // end of BARBARIAN_HPP definition

/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/7/2015
 * Last Modified:   2/27/2015
 * Assignment:      Assignment 4
 * Filename:        ReptilePeople.hpp
 *
 * Description:     Represents a Reptile Person character in a fantasy combat
 *                  game. Uses the default attack and defense functions of the
 *                  Character class.
 ************************************************************************/
#ifndef REPTILE_PEOPLE_HPP
#define REPTILE_PEOPLE_HPP

#include <string>
#include "Character.hpp"

class ReptilePeople : public Character
{
public:
    ReptilePeople(std::string);
    
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

#endif  // end of REPTILE_PEOPLE_HPP definition

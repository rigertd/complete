/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/7/2015
 * Last Modified:   2/27/2015
 * Assignment:      Assignment 4
 * Filename:        Goblin.hpp
 *
 * Description:     Represents a Goblin character in a fantasy combat game.
 *                  Overrides the defense and attack functions to implement
 *                  a special Achilles attack that halves the opponent's
 *                  attack value.
 ************************************************************************/
#ifndef GOBLIN_HPP
#define GOBLIN_HPP

#include <string>
#include "Character.hpp"

class Goblin : public Character
{
private:
    bool achillesEnabled;   // whether Achilles attack is enabled

public:
    Goblin(std::string);
    
    virtual void defense(int);
    virtual void attack(Character &);
    virtual void regenerate();
};

#endif  // end of GOBLIN_HPP definition

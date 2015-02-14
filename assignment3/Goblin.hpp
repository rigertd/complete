/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/7/2015
 * Last Modified:   2/14/2015
 * Assignment:      Assignment 3
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
    unsigned specialCount;  // for debugging special move
    unsigned totalCount;    // for debugging special move

public:
    Goblin(std::string);
    
    virtual void defense(int);
    virtual void attack(Character &);
    
    // debug info accessors
    bool usedAchilles() { return achillesEnabled; }
    unsigned getSpecialCount()  { return specialCount; }
    unsigned getTotalCount()    { return totalCount; }
};

#endif  // end of GOBLIN_HPP definition

/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/7/2015
 * Last Modified:   2/14/2015
 * Assignment:      Assignment 3
 * Filename:        Character.hpp
 *
 * Description:     Represents a character in a fantasy combat game.
 *                  This is an abstract class. All derived classes must
 *                  define the attack() and defense() functions.
 ************************************************************************/
#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <vector>
#include "Dice.hpp"

class Character
{
private:
    std::string name;               // character name and class
    std::vector<Dice> attackDice;   // dice used for attack rolls
    std::vector<Dice> defenseDice;  // dice used for defense rolls

protected:
    int armor;          // reduces damage amount
    int strengthPoints; // amount of damage the character can take
    
    int getAttackRoll();    // rolls the attack dice
    int getDefenseRoll();   // rolls the defense dice
    
public:
    Character(std::string, int, int, int, int, int, int);
    
    // Gets the name and class of the character.
    std::string getName()   { return this->name; }
    
    // Accessors for debugging special moves
    virtual unsigned getSpecialCount()  { return 0; }
    virtual unsigned getTotalCount()    { return 0; }

    // Determines whether the Character is still alive.
    bool isAlive()          { return strengthPoints > 0; }
    
    // pure virtual functions
    virtual void attack(Character &) = 0;   // attacks the specified Character
    virtual void defense(int) = 0;          // defends against an attack
};

#endif  // end of CHARACTER_HPP definition

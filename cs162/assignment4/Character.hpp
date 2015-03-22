/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/7/2015
 * Last Modified:   2/27/2015
 * Assignment:      Assignment 4
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
    int points;                     // number of battle points
    int startStrength;              // initial strength points
    std::vector<Dice> attackDice;   // dice used for attack rolls
    std::vector<Dice> defenseDice;  // dice used for defense rolls
    int armor;                      // reduces damage amount

protected:
    int strengthPoints; // amount of damage the character can take
    int getAttackRoll();    // rolls the attack dice
    int getDefenseRoll();   // rolls the defense dice
    
public:
    Character(std::string, int, int, int, int, int, int);
    // use pure virtual destructor to make abstract class
    virtual ~Character() = 0;
    
    // Gets the name and class of the character.
    std::string getName()   { return this->name; }
    
    // functions for determining rank
    int getPoints()         { return this->points; }
    int getStartStrength()  { return this->startStrength; }
    void addPoints(int p)   { this->points += p; }

    // Determines whether the Character is still alive.
    bool isAlive()          { return strengthPoints > 0; }
    
    // functions for character actions
    virtual void attack(Character &);   // attacks the specified Character
    virtual void defense(int);          // defends against an attack
    virtual void regenerate();          // regenerates a random percentage
                                        // of strength points lost
};

#endif  // end of CHARACTER_HPP definition

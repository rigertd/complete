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

    // Determines whether the Character is still alive.
    bool isAlive()          { return strengthPoints > 0; }
    
    // pure virtual functions
    virtual void attack(Character &) = 0;   // attacks the specified Character
    virtual void defense(int) = 0;          // defends against an attack
};

#endif  // end of CHARACTER_HPP definition

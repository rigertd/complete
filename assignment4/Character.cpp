/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/7/2015
 * Last Modified:   2/27/2015
 * Assignment:      Assignment 4
 * Filename:        Character.cpp
 *
 * Description:     Implementation of the abstract Character class. 
 ************************************************************************/
#include <iostream>
#include "Character.hpp"

/*============================ Constructors ============================*/
/*************************************************************************
 *  Function:       Character::Character(
 *                      std::string name, int attackSides, int attackRolls,
 *                      int defenseSides, int defenseRolls, int armor, int strengthPoints
 *  Description:    This constructor is called by derived classes.
 *  Parameters:     name            name of character
 *                  attackSides     sides of attack die
 *                  attackRolls     number of attack dice
 *                  defenseSides    sides of defense die
 *                  defenseRolls    number of defense dice
 *                  armor           armor value
 *                  strengthPoints  amount of life
 ************************************************************************/
Character::Character(
    std::string name, int attackSides, int attackRolls, 
    int defenseSides, int defenseRolls, int armor, int strengthPoints )
{
    this->name = name;
    this->armor = armor;
    this->strengthPoints = strengthPoints;
    this->startStrength = strengthPoints;
    this->points = 0;
    
    // create attack dice
    for (int i = 0; i < attackRolls; i++)
    {
        attackDice.push_back(Dice(attackSides));
    }
    
    // create defense dice
    for (int i = 0; i < defenseRolls; i++)
    {
        defenseDice.push_back(Dice(defenseSides));
    }
}

/*============================= Destructor ==============================*/
Character::~Character() { }

/*===================== Protected Member Functions ======================*/
/*************************************************************************
 *  Function:       int Character::getAttackRoll()
 *  Description:    Rolls the attack dice and returns the attack value.
 *  Parameters:     none
 *  Preconditions:  none
 *  Postconditions: Returns the sum of a roll of all dice in attackDice.
 ************************************************************************/
int Character::getAttackRoll()
{
    int attackValue = 0;    // buffer for storing roll
    
    // roll all of the attack dice and store the result
    for (unsigned i = 0; i < attackDice.size(); i++)
    {
        attackValue += attackDice[i].rollDice();
    }
    return attackValue;
}

/*************************************************************************
 *  Function:       int Character::getAttackRoll()
 *  Description:    Rolls the defense dice and returns the defense value.
 *  Parameters:     none
 *  Preconditions:  none
 *  Postconditions: Returns the sum of a roll of all dice in defenseDice.
 ************************************************************************/
int Character::getDefenseRoll()
{
    int defenseValue = 0;   // buffer for storing roll
    
    // roll all of the defense dice and store the result
    for (unsigned i = 0; i < defenseDice.size(); i++)
    {
        defenseValue += defenseDice[i].rollDice();
    }
    return defenseValue;
}

/*======================= Public Member Functions ======================*/
/*************************************************************************
 *  Function:       void Character::attack(Character &target)
 *  Description:    Attacks the specified Character object.
 *  Parameters:     none
 *  Preconditions:  none
 *  Postconditions: Target character has strengthPoints reduced if attack
 *                  was successful (attack score > defense score)
 ************************************************************************/
void Character::attack(Character &target)
{
    int attackValue = getAttackRoll();
    std::cout << name << " attacked " << target.getName() << " ";
    
    // call defense function of opponent
    target.defense(attackValue);
}

/*************************************************************************
 *  Function:       void Character::defense(int attackValue)
 *  Description:    Defends against an attack.
 *  Parameters:     attackValue     attacker's attack roll
 *  Preconditions:  none
 *  Postconditions: strengthPoints is reduced if defenseValue + armor < attackValue
 ************************************************************************/
void Character::defense(int attackValue)
{
    int defenseValue = getDefenseRoll();
    // damage = attack roll - (defense roll + armor)
    int damage = attackValue - (defenseValue + this->armor);
    
    if (attackValue <= defenseValue)
    {
        std::cout << "and misses!\n";
    }
    else if (damage <= 0)
    {
        std::cout << "but it bounces off the armor.\n";
    }
    else
    {
        std::cout << "and lands a blow!\n";
        this->strengthPoints -= damage;
    }
}

/*************************************************************************
 *  Function:       void Character::regenerate()
 *  Description:    Regenerates a random percentage of strength points.
 *  Parameters:     none
 *  Preconditions:  none
 *  Postconditions: Lost strengthPoints are regenerated by d10 * 10 percent.
 ************************************************************************/
void Character::regenerate()
{
    Dice d = Dice(10);
    int lostSP = startStrength - strengthPoints;
    if (lostSP > 0)
    {
        int recovered = static_cast<int>(0.1 * lostSP * d.rollDice());
        strengthPoints += recovered;
        std::cout << name << " recovered " << recovered << " strength points!\n";
    }
}

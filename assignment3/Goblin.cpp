/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/7/2015
 * Last Modified:   2/14/2015
 * Assignment:      Assignment 3
 * Filename:        Goblin.cpp
 *
 * Description:     Implementation of the Goblin class.
 ************************************************************************/
#include <iostream>
#include <typeinfo> // for typeid

#include "Goblin.hpp"

/*============================ Constructors ============================*/
/*************************************************************************
 *  Function:       Goblin::Goblin(std::string name)
 *  Description:    Invokes the parent class constructor with values for
 *                  a goblin.
 *  Parameters:     name    name of goblin
 ************************************************************************/
Goblin::Goblin(std::string name)
    : Character(
                name + " the Goblin",
                6,  // attack die sides
                2,  // attack die rolls
                6,  // defense die sides
                1,  // defense die rolls
                3,  // armor
                8   // strengthPoints
               )
{
    achillesEnabled = false;    // Achilles attack disabled by default
    specialCount = 0;
    totalCount = 0;
}

/*======================= Public Member Functions ======================*/
/*************************************************************************
 *  Function:       void Goblin::attack(Character &target)
 *  Description:    Attacks the specified Character object.
 *  Parameters:     none
 *  Preconditions:  none
 *  Postconditions: Target character has strengthPoints reduced if attack
 *                  was successful (attack score > defense score).
 *                  Achilles attack is enabled if a 12 is rolled.
 ************************************************************************/
void Goblin::attack(Character &target)
{
    totalCount++;   // increment total attack counter
    
    int attackValue = getAttackRoll();
    if (attackValue == 12 && typeid(*this) != typeid(target))
    {
        achillesEnabled = true;
        specialCount++; // increment special attack counter
        //std::cout << getName() << " hit " << target.getName() << "'s Achilles tendon!\n";
    }
    //std::cout << getName() << " attacked " << target.getName() << " ";
    target.defense(attackValue);
}

/*************************************************************************
 *  Function:       void Goblin::defense(int attackValue)
 *  Description:    Defends against an attack. Attack value is halved if an 
 *                  Achilles attack was successful during the battle.
 *  Parameters:     attackValue     attacker's attack roll
 *  Preconditions:  none
 *  Postconditions: strengthPoints is reduced if defenseValue + armor < attackValue
 *                  attackValue is halved if achillesEnabled is true
 ************************************************************************/
void Goblin::defense(int attackValue)
{
    int modifiedAttack = attackValue;
    if (achillesEnabled)
    {
        modifiedAttack = attackValue / 2;
        //std::cout << "with an injured heel ";
    }
    this->Character::defense(modifiedAttack);
}

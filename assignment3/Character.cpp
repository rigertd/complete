#include <iostream>
#include "Character.hpp"

Character::Character(
    std::string name, int attackSides, int attackRolls, 
    int defenseSides, int defenseRolls, int armor, int strengthPoints )
{
    this->name = name;
    this->armor = armor;
    this->strengthPoints = strengthPoints;
    
    // create attack die
    for (int i = 0; i < attackRolls; i++)
    {
        attackDice.push_back(Dice(attackSides));
    }
    
    // create defense die
    for (int i = 0; i < defenseRolls; i++)
    {
        defenseDice.push_back(Dice(defenseSides));
    }
}

// rolls the attack dice
int Character::getAttackRoll()
{
    int attackValue = 0;    // buffer for storing roll
    
    // roll all of the attack dice and store the result
    for (int i = 0; i < attackDice.size(); i++)
    {
        attackValue += attackDice[i].rollDice();
    }
    return attackValue;
}

// rolls the defense dice
int Character::getDefenseRoll()
{
    int defenseValue = 0;
    
    // roll all of the defense dice and store the result
    for (int i = 0; i < defenseDice.size(); i++)
    {
        defenseValue += defenseDice[i].rollDice();
    }
    return defenseValue;
}

// attack the specified target
void Character::attack(Character &target)
{
    int attackValue = getAttackRoll();
    std::cout << name << " attacked " << target.getName() << " ";
    
    // call defense function of opponent
    target.defense(attackValue);
}

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

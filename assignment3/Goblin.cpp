#include <iostream>
#include <typeinfo>

#include "Goblin.hpp"

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
}

// enable achillesEnabled if rolls a 12
void Goblin::attack(Character &target)
{
    int attackValue = getAttackRoll();
    if (attackValue == 12 && typeid(*this) != typeid(target))
    {
        achillesEnabled = true;
        std::cout << getName() << " hit " << target.getName() << "'s Achilles tendon!\n";
    }
    std::cout << getName() << " attacked " << target.getName() << " ";
    target.defense(attackValue);
}

// opponent attack halved if achillesEnabled
void Goblin::defense(int attackValue)
{
    int modifiedAttack = attackValue;
    if (achillesEnabled)
    {
        modifiedAttack = attackValue / 2;
        std::cout << "with an injured heel ";
    }
    this->Character::defense(modifiedAttack);
}

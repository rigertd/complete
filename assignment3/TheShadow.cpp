#include <iostream>
#include "TheShadow.hpp"

TheShadow::TheShadow(std::string name)
    : Character(
                name + " the Shadow",
                10,  // attack die sides
                2,  // attack die rolls
                10,  // defense die sides
                1,  // defense die rolls
                0,  // armor
                12  // strengthPoints
               )
{ }

// opponent has 90% chance of missing
void TheShadow::defense(int attackValue)
{
    int defenseValue = getDefenseRoll();
    if (defenseValue == 1)
    {
        //std::cout << "and lands a blow!\n";
        this->strengthPoints -= attackValue;
    }
    else
    {
        //std::cout << "but the Shadow wasn't there!\n";
    }
}

#include "Barbarian.hpp"

Barbarian::Barbarian(std::string name)
    : Character(
                name + " the Barbarian",
                6,  // attack die sides
                2,  // attack die rolls
                6,  // defense die sides
                2,  // defense die rolls
                0,  // armor
                12  // strengthPoints
               )
{ }

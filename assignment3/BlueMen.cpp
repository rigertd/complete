#include "BlueMen.hpp"

BlueMen::BlueMen(std::string name)
    : Character(
                name + " the Blue Men",
                10,  // attack die sides
                2,  // attack die rolls
                6,  // defense die sides
                3,  // defense die rolls
                3,  // armor
                12  // strengthPoints
               )
{ }

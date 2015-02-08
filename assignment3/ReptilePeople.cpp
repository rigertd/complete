#include "ReptilePeople.hpp"

ReptilePeople::ReptilePeople(std::string name)
    : Character(
                name + " the Reptile Person",
                6,  // attack die sides
                3,  // attack die rolls
                6,  // defense die sides
                1,  // defense die rolls
                7,  // armor
                18  // strengthPoints
               )
{ }

#ifndef BLUE_MEN_HPP
#define BLUE_MEN_HPP

#include <string>
#include "Character.hpp"

class BlueMen : public Character
{
public:
    BlueMen(std::string);
    
    // use parent class member function
    void attack(Character &c1)
    {
        Character::attack(c1);
    }

    // use parent class member function
    void defense(int attackValue)
    {
        Character::defense(attackValue);
    }
};

#endif  // end of BLUE_MEN_HPP definition

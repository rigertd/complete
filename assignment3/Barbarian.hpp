#ifndef BARBARIAN_HPP
#define BARBARIAN_HPP

#include <string>
#include "Character.hpp"

class Barbarian : public Character
{
public:
    Barbarian(std::string);
    
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

#endif  // end of BARBARIAN_HPP definition

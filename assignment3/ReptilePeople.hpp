#ifndef REPTILE_PEOPLE_HPP
#define REPTILE_PEOPLE_HPP

#include <string>
#include "Character.hpp"

class ReptilePeople : public Character
{
public:
    ReptilePeople(std::string);
    
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

#endif  // end of REPTILE_PEOPLE_HPP definition

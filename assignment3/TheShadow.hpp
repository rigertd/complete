#ifndef THE_SHADOW_HPP
#define THE_SHADOW_HPP

#include <string>
#include "Character.hpp"

class TheShadow : public Character
{
private:
    unsigned specialCount;  // for debugging special defense
    unsigned totalCount;  // for debugging special defense

public:
    TheShadow(std::string);
    
    virtual void defense(int);
    
    // use parent class member function
    void attack(Character &c1)
    {
        Character::attack(c1);
    }

    // debug info accessors
    unsigned getSpecialCount()  { return specialCount; }
    unsigned getTotalCount()    { return totalCount; }
};

#endif  // end of THE_SHADOW_HPP definition

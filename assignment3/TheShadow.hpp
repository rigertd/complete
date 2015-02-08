#ifndef THE_SHADOW_HPP
#define THE_SHADOW_HPP

#include <string>
#include "Character.hpp"

class TheShadow : public Character
{
public:
    TheShadow(std::string);
    virtual void defense(int);
};

#endif  // end of THE_SHADOW_HPP definition

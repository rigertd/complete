#ifndef GOBLIN_HPP
#define GOBLIN_HPP

#include <string>
#include "Character.hpp"

class Goblin : public Character
{
private:
    bool achillesEnabled;   // whether Achilles attack is enabled
public:
    Goblin(std::string);
    virtual void defense(int);
    virtual void attack(Character &);
};

#endif  // end of GOBLIN_HPP definition

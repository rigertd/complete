#ifndef GOBLIN_HPP
#define GOBLIN_HPP

#include <string>
#include "Character.hpp"

class Goblin : public Character
{
private:
    bool achillesEnabled;   // whether Achilles attack is enabled
    unsigned specialCount;  // for debugging special move
    unsigned totalCount;    // for debugging special move

public:
    Goblin(std::string);
    
    virtual void defense(int);
    virtual void attack(Character &);
    
    // debug info accessors
    bool usedAchilles() { return achillesEnabled; }
    unsigned getSpecialCount()  { return specialCount; }
    unsigned getTotalCount()    { return totalCount; }
};

#endif  // end of GOBLIN_HPP definition

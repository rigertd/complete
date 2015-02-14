#ifndef COMBAT_TEST_HPP
#define COMBAT_TEST_HPP

#include "Character.hpp"

class CombatTest
{
private:
    int characterType1; // first character type
    int characterType2; // second character type
    int rounds;         // number of rounds of combat
    int c1Wins;         // wins by character 1
    int c2Wins;         // wins by character 2
    int c1SpecialCount; // special moves by character 1
    int c1TotalCount;   // total moves by character 1
    int c2SpecialCount; // special moves by character 2
    int c2TotalCount;   // total moves by character 2
    
    Character *createCharacter(int);
    void deleteCharacter(Character *&);
    Character *fight(Character *, Character *);
    void printResults();
    
public:
    CombatTest(int, int, int);
    void run();
};

#endif  // end of COMBAT_TEST_HPP definition
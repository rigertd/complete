#include <iostream>
#include <iomanip>

#include "CombatTest.hpp"
#include "Goblin.hpp"
#include "Barbarian.hpp"
#include "ReptilePeople.hpp"
#include "BlueMen.hpp"
#include "TheShadow.hpp"

static std::string characterNames[] = { "",
                                   "Goblin",
                                   "Barbarian",
                                   "Reptile Person",
                                   "Blue Men",
                                   "The Shadow" };

CombatTest::CombatTest(int c1Type, int c2Type, int rounds)
{
    this->characterType1 = c1Type;
    this->characterType2 = c2Type;
    this->rounds = rounds;
    c1Wins = 0;
    c2Wins = 0;
    c1SpecialCount = 0;
    c1TotalCount = 0;
    c2SpecialCount = 0;
    c2TotalCount = 0;
}

Character *CombatTest::createCharacter(int characterType)
{
    Character *pCharacter;
    switch (characterType)
    {
    case 1:
        pCharacter = new Goblin("Gronk");
        break;
    case 2:
        pCharacter = new Barbarian("Andre");
        break;
    case 3:
        pCharacter = new ReptilePeople("Lizzy");
        break;
    case 4: 
        pCharacter = new BlueMen("Louis");
        break;
    case 5:
        pCharacter = new TheShadow("Shade");
        break;
    default:
        pCharacter = NULL;
    }
    return pCharacter;
}
void CombatTest::deleteCharacter(Character *&pCharacter)
{
    if (pCharacter != NULL)
    {
        delete pCharacter;
        pCharacter = NULL;
    }
}

Character *CombatTest::fight(Character *c1, Character *c2)
{
    do
    {
        c1->attack(*c2);
        if (c2->isAlive())
            c2->attack(*c1);
    } while (c1->isAlive() && c2->isAlive());
    
    // return pointer to winner or NULL if tie
    if (c1->isAlive())
        return c1;
    else
        return c2;
}

void CombatTest::printResults()
{
    // calculate percentages
    double c1WinPercent = 100.0 * static_cast<double>(c1Wins) / rounds;
    double c2WinPercent = 100.0 * static_cast<double>(c2Wins) / rounds;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Winner Statistics:" << std::endl;
    
    // results for character 1
    std::cout << "Character 1 (" << characterNames[characterType1] << "): " 
              << c1WinPercent << "% of " << rounds << " rounds" << std::endl;
    
    if (characterType1 == 1)        // Achilles statistics for Goblin
    {
        double specialPercent = 100.0 * static_cast<double>(c1SpecialCount) / c1TotalCount;
        std::cout << "Used Achilles attack " << specialPercent << "% of attacks.\n";
    }
    else if (characterType1 == 5)   // Special statistics for TheShadow
    {
        double specialPercent = 100.0 * static_cast<double>(c1SpecialCount) / c1TotalCount;
        std::cout << "Dodged " << specialPercent << "% of attacks.\n";
    }
    
    // results for character 2
    std::cout << "Character 2 (" << characterNames[characterType2] << "): " 
              << c2WinPercent << "% of " << rounds << " rounds" << std::endl;
    
    if (characterType2 == 1)        // Achilles statistics for Goblin
    {
        double specialPercent = 100.0 * static_cast<double>(c2SpecialCount) / c2TotalCount;
        std::cout << "Used Achilles attack " << specialPercent << "% of attacks.\n";
    }
    else if (characterType2 == 5)   // Special statistics for TheShadow
    {
        double specialPercent = 100.0 * static_cast<double>(c2SpecialCount) / c2TotalCount;
        std::cout << "Dodged " << specialPercent << "% of attacks.\n";
    }
    std::cout << std::endl;
}

void CombatTest::run()
{
    // display combat announcement
    std::cout << "Starting combat between "
              << characterNames[characterType1] << " and "
              << characterNames[characterType2] << "...\n";
    
    Character *c1;  // first character
    Character *c2;  // second character
    for (int i = 0; i < rounds; i++)
    {
        c1 = createCharacter(characterType1);
        c2 = createCharacter(characterType2);
        Character *winner = fight(c1, c2);
        if (winner == c1)
        {
            //std::cout << c1->getName() << " (Character 1) is victorious!\n";
            c1Wins++;
            
        }
        else if (winner == c2)
        {
            //std::cout << c2->getName() << " (Character 2) is victorious!\n";
            c2Wins++;
            
        }
        else
        {
            //std::cout << "Both combatants have fallen in battle!\n";
        }
        
        // track special moves vs total moves
        if (characterType1 == 1 || characterType1 == 5)
        {
            c1SpecialCount += c1->getSpecialCount();
            c1TotalCount += c1->getTotalCount();
        }
        if (characterType2 == 1 || characterType2 == 5)
        {
            c2SpecialCount += c2->getSpecialCount();
            c2TotalCount += c2->getTotalCount();
        }
        
        // delete characters from heap
        deleteCharacter(c1);
        deleteCharacter(c2);
    }
    std::cout << "All " << rounds << " rounds of combat are over.\n\n";
    printResults();
}

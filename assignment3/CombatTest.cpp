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
        c2->attack(*c1);
    } while (c1->isAlive() && c2->isAlive());
    
    // return pointer to winner or NULL if tie
    if (c1->isAlive())
        return c1;
    else if (c2->isAlive())
        return c2;
    else
        return NULL;
}

void CombatTest::printResults()
{
    double c1WinPercent = 100.0 * static_cast<double>(c1Wins) / rounds;
    double c2WinPercent = 100.0 * static_cast<double>(c2Wins) / rounds;
    double tiePercent = 100.0 - (c1WinPercent + c2WinPercent);
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Winner Statistics:" << std::endl;
    std::cout << "Character 1 (" << characterNames[characterType1] << "): " 
              << c1WinPercent << "% of " << rounds << " rounds" << std::endl;
    std::cout << "Character 2 (" << characterNames[characterType2] << "): " 
              << c2WinPercent << "% of " << rounds << " rounds" << std::endl;
    std::cout << "Both characters died " << tiePercent << "% of the time\n\n";
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
        
        // delete characters from heap
        deleteCharacter(c1);
        deleteCharacter(c2);
    }
    std::cout << "All " << rounds << " rounds of combat are over.\n\n";
    printResults();
}

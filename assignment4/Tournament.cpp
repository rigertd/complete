/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/25/2015
 * Last Modified:   2/25/2015
 * Assignment:      Assignment 4
 * Filename:        Tournament.hpp
 *
 * Description:     Implementation of the Tournament class. 
 ************************************************************************/
#include <iostream>
#include <iomanip>

#include "Tournament.hpp"
#include "Goblin.hpp"
#include "Barbarian.hpp"
#include "ReptilePeople.hpp"
#include "BlueMen.hpp"
#include "TheShadow.hpp"

/*========================== Static Constants ==========================*/
// used for converting characterType values to string
static const std::string characterNames[] = { "",
                                   "Goblin",
                                   "Barbarian",
                                   "Reptile Person",
                                   "Blue Men",
                                   "The Shadow" };

/*===================== Constructor and Destructor =====================*/
/*************************************************************************
 *  Function:       Tournament::Tournament(int teamSize)
 *  Description:    Instantiates a Tournament object designed for teamSize
 *                  fighters per team.
 *  Parameters:     teamSize    number of fighters on each team
 ************************************************************************/
Tournament::Tournament(int teamSize)
{
    this->teamSize = teamSize;
    this->coin = Dice(2);
}

/*************************************************************************
 *  Function:       Tournament::~Tournament()
 *  Description:    Deletes all dynamically allocated objects pointed to
 *                  by pointers in team1, team2 and standings containers.
 *  Preconditions:  none
 *  Postconditions: Dynamic memory pointed to by pointers in member containers
 *                  is deallocated.
 ************************************************************************/
Tournament::~Tournament()
{
    Character *ptr = NULL;  // points to object to delete
    
    // deallocate objects in team1
    while (team1.size() > 0)
    {
        ptr = team1.front();
        team1.pop();
        delete ptr;
        ptr = NULL;
    }
    // deallocate objects in team2
    while (team2.size() > 0)
    {
        ptr = team2.front();
        team2.pop();
        delete ptr;
        ptr = NULL;
    }
    // deallocate objects in standings
    while (standings.size() > 0)
    {
        ptr = standings.front().member;
        standings.pop_front();
        delete ptr;
        ptr = NULL;
    }
}

/*====================== Private Member Functions ======================*/
/*************************************************************************
 *  Function:       void Tournament::addToStandings(TeamMember tm)
 *  Description:    Adds the specified fighter to the standings based on
 *                  their total points.
 *  Parameters:     tm      Character and team number wrapped in TeamMember
 *  Preconditions:  none
 *  Postconditions: tm is positioned in standings based on points
 ************************************************************************/
void Tournament::addToStandings(TeamMember tm)
{
    auto current = standings.begin();
    while (current != standings.end())
    {
        // insert before current position if parameter has more points
        if (tm.member->getPoints() > current->member->getPoints())
        {
            standings.insert(current, tm);
            break;
        }
        // check tiebreaker if same number of points
        else if (tm.member->getPoints() == current->member->getPoints())
        {
            // insert before current position if less starting strength points
            if (tm.member->getStartingStrength() 
                < current->member->getStartingStrength())
            {
                standings.insert(current, tm);
                break;
            }
            // flip a coin if starting strength is the same
            else if (tm.member->getStartingStrength()
                == current->member->getStartingStrength())
            {
                if (coin.rollDice() == 1)
                {
                    standings.insert(current, tm);
                    break;
                }
            }
        }
        
        // increment iterator and try next position
        ++current;
    }
}

/*************************************************************************
 *  Function:       Character *Tournament::createCharacter(
 *                      int characterType, std::string name)
 *  Description:    Creates an object of the specified character type
 *                  with the specified name and returns a pointer.
 *  Parameters:     characterType   1=Goblin, 2=Barbarian, 3=ReptilePeople
 *                                  4=BlueMen, 5=TheShadow
 *                  name            name of character
 *  Preconditions:  none
 *  Postconditions: Returns a pointer to the instantiated object, 
 *                  or NULL if an invalid character type is specified.
 ************************************************************************/
Character *Tournament::createCharacter(int characterType, std::string name)
{
    Character *pCharacter;
    switch (characterType)
    {
    case 1:
        pCharacter = new Goblin(name);
        break;
    case 2:
        pCharacter = new Barbarian(name);
        break;
    case 3:
        pCharacter = new ReptilePeople(name);
        break;
    case 4: 
        pCharacter = new BlueMen(name);
        break;
    case 5:
        pCharacter = new TheShadow(name);
        break;
    default:
        pCharacter = NULL;
    }
    return pCharacter;
}

/*************************************************************************
 *  Function:       Character *Tournament::fight(Character *c1, Character *c2)
 *  Description:    Manages a fight between two characters.
 *                  The character to attack first is determined by a coin toss.
 *                  Characters do not get a chance to attack after they lose.
 *  Parameters:     c1  pointer to first Character object
 *                  c2  pointer to second Character object
 *  Preconditions:  c1 and c2 are not NULL
 *  Postconditions: Returns a pointer to the victor.
 ************************************************************************/
Character *Tournament::fight(Character *c1, Character *c2)
{
    do
    {
        if (coin.rollDice() == 1)
        {
            c1->attack(*c2);
            // character 2 only attacks if still alive
            if (c2->isAlive())
                c2->attack(*c1);
        }
        else
        {
            c2->attack(*c1);
            // character 1 only attacks if still alive
            if (c1->isAlive())
                c1->attack(*c2);
        }
    } while (c1->isAlive() && c2->isAlive());
    
    // return pointer to winner or NULL if tie
    if (c1->isAlive())
        return c1;
    else
        return c2;
}

/*************************************************************************
 *  Function:       void Tournament::printStandings()
 *  Description:    Prints the top 3 fighters after the tournament is over.
 *                  Rank is based on total points scored during battle.
 *  Parameters:     none
 *  Preconditions:  none
 *  Postconditions: Top 3 fighters are printed to terminal window
 ************************************************************************/
void Tournament::printStandings()
{
    // make sure there is at least one fighter in standings
    if (standings.size() == 0)
    {
        std::cout << "There are no fighters in this tournament!\n";
        return;
    }
    
    // print header row
    std::cout << std::setw(5) << "Rank"
              << std::setw(5) << "Team"
              << std::setw(50) << "Name"
              << std::setw(10) << "Points"
              << std::endl
              << string(70, '-') << std::endl;
    
    // print first 3, or everything in list if < 3
    for (unsigned i = 0; i < standings.size() && i < 3; ++i)
    {
        // get iterator to offset of i
        auto current = standings.begin() + i;
        std::cout << std::setw(5) << (i + 1)                // rank
                  << std::setw(5) << current->teamNumber    // team number
                  << std::setw(50) << current->member->getName()    // name
                  << std::setw(10) << current->member->getPoints()  // points
                  << std::endl;
    }
}

/*======================= Public Member Functions ======================*/
/*************************************************************************
 *  Function:       void CombatTest::run()
 *  Description:    Runs the configured simulation.
 *  Parameters:     none
 *  Preconditions:  none
 *  Postconditions: results of simulation are printed to the terminal
 ************************************************************************/
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

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
#include <stdexcept>

#include "utility.hpp"
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
    if (teamSize < 1)
        throw std::invalid_argument("The team size must be at least 1.");
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
    std::list<TeamMember>::iterator current = standings.begin();
    if (current == standings.end())
        standings.push_front(tm);
    else
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
                if (tm.member->getStartStrength() 
                    < current->member->getStartStrength())
                {
                    standings.insert(current, tm);
                    break;
                }
                // flip a coin if starting strength is the same
                else if (tm.member->getStartStrength()
                    == current->member->getStartStrength())
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
              << std::string(70, '-') << std::endl;
    
    // get iterator to beginning of list
    std::list<TeamMember>::iterator current = standings.begin();
    
    // print first 3, or everything in list if < 3
    for (unsigned i = 0; i < standings.size() && i < 3; ++i)
    {
        std::cout << std::setw(5) << (i + 1)                // rank
                  << std::setw(5) << current->teamNumber    // team number
                  << std::setw(50) << current->member->getName()    // name
                  << std::setw(10) << current->member->getPoints()  // points
                  << std::endl;
        ++current;
    }
}

/*************************************************************************
 *  Function:       void Tournament::printTeams()
 *  Description:    Prints the match-ups for the current round of the
 *                  tournament.
 *  Parameters:     none
 *  Preconditions:  none
 *  Postconditions: Both player's current lineups are displayed on screen
 ************************************************************************/
void Tournament::printTeams()
{
    unsigned maxSize = team1.size() > team2.size() ? team1.size() : team2.size();
    Character *t1Begin = team1.front();
    Character *c1 = t1Begin;
    Character *t2Begin = team2.front();
    Character *c2 = t2Begin;
    std::cout << "--------------------------Fighter Lineup------------------------------\n";
    std::cout << "     Player 1                     VS        Player 2\n";
    for (int i = 0; i < maxSize; ++i)
    {
        // Team 1 fighter
        if (i == 0 || t1Begin != c1)
        {
            team1.pop();
            std::cout << std::right << std::setw(3) << (i + 1) << ": "
                      << std::left << std::setw(34) << c1->getName();
            team1.push(c1);
            c1 = team1.front();
        }
        else
        {
            // display blank entry if not enough fighters
            std::cout << std::setw(39) << " ";
        }
        // Team 2 fighter
        if (i == 0 || t2Begin != c2)
        {
            team2.pop();
            std::cout << std::right << std::setw(3) << (i + 1) << ": "
                      << std::left << std::setw(34) << c2->getName();
            team2.push(c2);
            c2 = team2.front();
        }
        else
        {
            // display blank entry if not enough fighters
            std::cout << std::setw(39) << " ";
        }
        std::cout << std::endl;
    }
}

/*************************************************************************
 *  Function:       Character *Tournament::selectCharacter()
 *  Description:    Prompts the user for the type and name of the character
 *                  to create.
 *  Parameters:     none
 *  Preconditions:  none
 *  Postconditions: Returns a pointer to a new Character subclass object
 ************************************************************************/
Character *Tournament::selectCharacter()
{
    int selection;      // holds character type
    std::string name;   // holds character name
    
    // display selection menu
    for (int i = 1; i < 6; ++i)
    {
        std::cout << i << ": " << characterNames[i] << std::endl;
    }
    
    // prompt user for selection
    std::cout << "\nCharacter type: ";
    std::cin >> selection;
    
    // validate input
    while (std::cin.get() != '\n' || selection < 1 || selection > 5)
    {
        clearInputBuffer();
        std::cout << "Invalid selection. Try again: ";
        std::cin >> selection;
    }
    
    // prompt user for name
    std::cout << "Character name: ";
    std::getline(std::cin, name);
    
    // validate input
    while (name.empty())
    {
        std::cout << "Name cannot be blank. Try again: ";
        std::getline(std::cin, name);
    }
    
    // return pointer to object
    return createCharacter(selection, name);
}

/*************************************************************************
 *  Function:       void Tournament::selectTeam(std::queue<Character *> &team)
 *  Description:    Prompts the user to select all of the characters on their
 *                  team and adds them to the queue for that team.
 *  Parameters:     none
 *  Preconditions:  none
 *  Postconditions: Specified queue is filled with the selected characters
 ************************************************************************/
void Tournament::selectTeam(std::queue<Character *> &team)
{
    Character *pCharacter = NULL;
    for (int i = 0; i < teamSize; ++i)
    {
        std::cout << "\nFighter #" << (i + 1) << ":" << std::endl;
        pCharacter = selectCharacter();
        team.push(pCharacter);
    }
}

/*======================= Public Member Functions ======================*/
/*************************************************************************
 *  Function:       void Tournament::run()
 *  Description:    Runs the configured tournament.
 *  Parameters:     none
 *  Preconditions:  none
 *  Postconditions: results of simulation are printed to the terminal
 ************************************************************************/
void Tournament::run()
{
    std::string buffer; // for user input
    unsigned round = 0; // round number
    Character *c1;      // first character
    Character *c2;      // second character
    Character *winner;  // winner of battle
    short winTeam = 0;  // the winning team
    
    std::cout << "Welcome to the Fantasy Fighting League Tournament!\n";
    
    do
    {
        // increment round number
        ++round;
        int battles = team1.size() > team2.size() ? team1.size() : team2.size();
        
        
        // display round number and current lineup
        std::cout << "\nStarting Round " << round << "...\n";
        printTeams();
        
        // continue fighting until every character has fought, or one team wins
        for (int i = 0; i < battles && team1.size() > 0 && team2.size() > 0; ++i)
        {
            // get fighter from team1
            c1 = team1.front();
            team1.pop();
            
            // get fighter from team2
            c2 = team2.front();
            team2.pop();
            
            // display battle
            std::cout << "\nBattle #" << (i + 1) << ": "
                      << c1->getName() << " VS " << c2->getName() << "\n\n";
            std::cout << "Press Enter to begin battle...";
            std::getline(std::cin, buffer);
            std::cout << std::endl;
            
            // start fight
            winner = fight(c1, c2);
            
            // display victor
            std::cout << "\nAnd the winner is " << winner->getName() << "!\n";
            
            winner->regenerate();
            
            std::cout << "\nPress Enter to continue...";
            std::getline(std::cin, buffer);
            
            // add points to winner, put winner back in lineup, 
            // and put loser in standings
            if (winner == c1)
            {
                winner->addPoints(c2->getStartStrength());
                team1.push(winner);
                addToStandings(TeamMember(c2, 2));
            }
            else
            {
                winner->addPoints(c1->getStartStrength());
                team2.push(winner);
                addToStandings(TeamMember(c1, 1));
            }
            
            clearWindow();
        }
        
    } while (team1.size() > 0 && team2.size() > 0);
    
    // move remaining characters to standings
    while (team1.size() > 0)
    {
        winTeam = 1;
        c1 = team1.front();
        team1.pop();
        addToStandings(TeamMember(c1, 1));
    }
    while (team2.size() > 0)
    {
        winTeam = 2;
        c2 = team2.front();
        team2.pop();
        addToStandings(TeamMember(c2, 2));
    }
    
    // display winner and top 3 fighters
    std::cout << "\nThe Grand Tournament is Over!\n";
    std::cout << "Player " << winTeam << "'s team won. Congratulations!\n"; 
    std::cout << "Your team won in " << round << " rounds!\n\n";
    printStandings();
    std::cout << std::endl;
}

/*************************************************************************
 *  Function:       void Tournament::setup()
 *  Description:    Prompts each player to select their fighters.
 *  Parameters:     none
 *  Preconditions:  none
 *  Postconditions: teamSize is set and team1 and team2 are filled with
 *                  player-specified characters.
 ************************************************************************/
void Tournament::setup()
{
    // prompt player 1
    std::cout << "\nPlayer 1, choose your fighters." << std::endl;
    selectTeam(team1);
    clearWindow();
    // prompt player 2/25/2015
    std::cout << "Player 2, choose your fighters." << std::endl;
    selectTeam(team2);
    clearWindow();
}

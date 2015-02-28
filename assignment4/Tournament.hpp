/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/25/2015
 * Last Modified:   2/25/2015
 * Assignment:      Assignment 4
 * Filename:        Tournament.hpp
 *
 * Description:     Represents a 2-player tournament where the players pit
 *                  their teams of fantasy fighters against each other.
 *                  The winning team and final standings of individual fighters
 *                  are displayed at the end of the tournament.
 ************************************************************************/
#ifndef TOURNAMENT_HPP
#define TOURNAMENT_HPP

#include <queue>
#include <list>

#include "Character.hpp"
#include "Dice.hpp"

class Tournament
{
private:
    // keeps track of team number in standings
    struct TeamMember
    {
        Character *member;
        short teamNumber;
        
        TeamMember(Character *mem, short teamNum)
        {
            member = mem;
            teamNumber = teamNum;
        }
    };
    
    std::queue<Character *> team1;      // holds fighters of player 1
    std::queue<Character *> team2;      // holds fighters of player 2
    std::list<TeamMember> standings;   // holds sorted standings
    int teamSize;       // size of each team
    Dice coin;          // d2 for coin flips
    
public:
    Tournament(int);    // constructor
    ~Tournament();      // destructor
    
    // prompts user for team and stores in specified queue
    void selectTeam(std::queue<Character *> &);
    // sets up both teams for the tournament
    void setup();
    // runs the tournament
    void run();
    
private:
    // adds a Character wrapped in a TeamMember struct to the standings
    void addToStandings(TeamMember);
    // creates a Character and returns a pointer to it
    Character *createCharacter(int, std::string);
    // has two Characters fight and returns the winner
    Character *fight(Character *, Character *);
    // prints the top 3 of the standings after the tournament
    void printStandings();
    // prints the current lineups of both teams
    void printTeams();
    // prompts user to pick a character and name them
    Character *selectCharacter();
};

#endif  // end of TOURNAMENT_HPP definition
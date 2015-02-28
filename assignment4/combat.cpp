/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/25/2015
 * Last Modified:   2/25/2015
 * Assignment:      Assignment 4
 * Filename:        combat.hpp
 *
 * Description:     This simulates a 2-player tournament where the players
 *                  put together teams of fantasy fighters and have them battle
 *                  each other. 
 *                  The winning team and final standings of individual fighters
 *                  are displayed at the end of the tournament.
 * Input:           - Number of fighters on each team
 *                  - Player 1 selects fighters
 *                  - Player 2 selects fighters
 * Output:          - Results of each battle
 *                  - Winning team
 *                  - Top 3 fighters
 ************************************************************************/
#include <iostream>

#include "utility.hpp"
#include "Tournament.hpp"

int main()
{
    int count;      // stores number of fighters
    
    do  // start of menu loop
    {
        clearWindow();
        // get number of fighters
        std::cout << "Enter the number of fighters on each team: ";
        std::cin >> count;
        
        // validate input
        while (std::cin.get() != '\n' || count < 1)
        {
            clearInputBuffer();
            std::cout << "Invalid input. Try again: ";
            std::cin >> count;
        }
        
        // declare Tournament with teams of 'count' fighters
        Tournament t(count);
        // have players select their teams
        t.setup();
        // run the tournament
        t.run();
        
        // check if players want to play again
        std::cout << "Do you want to play again? (y/N): ";
    } while (getYesNo());
    
    std::cout << "\nThank you for playing!\n";
    
    return 0;
}
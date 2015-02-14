/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/8/2015
 * Last Modified:   2/14/2015
 * Assignment:      Assignment 3
 * Filename:        combat.cpp
 *
 * Description:     This program is a test driver for the character classes
 *                  of a fantasy combat game. It runs combat tests using all 
 *                  possible combinations of the Character class's subclasses.
 *
 * Input:           None
 *
 * Output:          - Displays the results of COMBAT_COUNT rounds of combat
 *                    between each pair of characters.
 *                  - If a character has a special ability,
 *                    displays the usage percentage.
 ************************************************************************/
#include <iostream>
#include <string>
#include <vector>

#include "CombatTest.hpp"

struct CharacterPair
{
    int c1;
    int c2;
};

const int COMBAT_COUNT = 100000;   // each pair fights for this many rounds

int main()
{
    std::vector<CharacterPair> pairs;
    std::string buffer;
    
    // add every 2-permutation of characters
    for (int i = 1; i <= 5; i++)
        for (int j = 1; j <= 5; j++)
        {
            CharacterPair pair = {i, j};
            pairs.push_back(pair);
        }
    
    std::cout << "Running " << COMBAT_COUNT << " rounds of combat per pair.\n\n";
    // calculate and display statistics for each pair
    for (unsigned i = 0; i < pairs.size(); i++)
    {
        CombatTest ct (pairs[i].c1, pairs[i].c2, COMBAT_COUNT);
        ct.run();
    }
    
    return 0;
}
/*************************************************************************
 * Author:          David Rigert
 * Date Created:    1/29/2015
 * Last Modified:   1/29/2015
 * Assignment:      Lab 3
 * Filename:        LoadedDice.cpp
 *
 * Description:     Implementation of the LoadedDice class.
 ************************************************************************/
#include <ctime>
#include <cstdlib>
#include "LoadedDice.hpp"

int LoadedDice::rollDice() const
{
    // Double the number of sides minus 1
    // For example, on a 6-sided die, a random number between 1 and 10 is generated.
    // With 10 possible numbers, there is a 50% chance of 1-5, and a 50% chance of 6-10.
    // Return 6 if the value is 6-10 to have a 50% chance of the highest value.
    int result = (rand() % ((numSides - 1) * 2)) + 1;
    if (result > numSides)
        result = numSides;
    return result;
}
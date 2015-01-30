/*************************************************************************
 * Author:          David Rigert
 * Date Created:    1/29/2015
 * Last Modified:   1/29/2015
 * Assignment:      Lab 3
 * Filename:        LoadedDice.hpp
 *
 * Description:     Represents a die with a 50% chance of getting 
 *                  the highest number.
 ************************************************************************/
#ifndef LOADEDDICE_HPP
#define LOADEDDICE_HPP

#include "Dice.hpp"

class LoadedDice : public Dice
{
private:

public:
    LoadedDice() {}
    LoadedDice(int sides) : Dice(sides) {}
    int rollDice() const;
};

#endif
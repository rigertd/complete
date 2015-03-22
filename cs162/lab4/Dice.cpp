// Dice.cpp created from Lab4.pdf for CS162 Winter 2015
// File generated by David Rigert

#include <cstdlib>
#include <ctime>
#include "Dice.hpp"

Dice::Dice()
{
    numSides = 6;
    srand(time(NULL)); // Seeds random number generator
}

Dice::Dice(int numSides)
{
    this->numSides = numSides;
    srand(time(NULL)); // Seeds random number generator
}

int Dice::rollDice() const
{
    return (rand() % numSides) + 1;
}

// Take two dice objects, roll them, and return the sum
int rollTwoDice(const Dice& die1, const Dice& die2)
{
    return die1.rollDice() + die2.rollDice();
}
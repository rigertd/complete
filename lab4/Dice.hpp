#ifndef DICE_HPP
#define DICE_HPP
class Dice
{
public:
    Dice();
    Dice( int numSides);
    virtual int rollDice() const;
protected:
    int numSides;
};

int rollTwoDice(const Dice &, const Dice &);
#endif

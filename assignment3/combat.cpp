#include <iostream>
#include <string>
#include <vector>

#include "CombatTest.hpp"

struct CharacterPair
{
    int c1;
    int c2;
};

const int COMBAT_COUNT = 10000;   // each pair fights for this many rounds

int main()
{
    std::vector<CharacterPair> pairs;
    std::string buffer;
    
    for (int i = 1; i <= 5; i++)
        for (int j = i; j <= 5; j++)
        {
            CharacterPair pair = {i, j};
            pairs.push_back(pair);
        }
    
    std::cout << "Running 10000 rounds of combat per pair.\n\n";
    // calculate and display statistics for each pair
    for (int i = 0; i < pairs.size(); i++)
    {
        CombatTest ct (pairs[i].c1, pairs[i].c2, COMBAT_COUNT);
        ct.run();
    }
    
    return 0;
}
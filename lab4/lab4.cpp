/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           1/29/2015
 * Last Modification Date: 1/29/2015
 * Course:                 CS162_400
 * Assignment:             Lab 4
 * Filename:               lab4.cpp
 *
 * Overview:
 *     This program uses the Dice and LoadedDice classes to generate sample
 *     data from rolling two dice and outputs the results to a CSV file.
 *
 * Requirements:
 *     - Write a function that creates two Dice objects and two LoadedDice objects.
 *     - Call the rollTwoDice function 10 times to verify that the LoadedDice work.
 *     - Generate results from 100 or more rolls of the dice 
 *       for both regular and loaded dice.
 ************************************************************************/
#include <iostream>
#include <fstream>
#include "Dice.hpp"
#include "LoadedDice.hpp"

int main()
{
    Dice d6_1;              // regular d6 x 2
    Dice d6_2;
    LoadedDice ld6_1;       // loaded d6 x 2
    LoadedDice ld6_2;
    Dice d20 (20);          // regular d20
    LoadedDice ld20 (20);   // loaded d20
    
    // Print 10 rolls from regular dice to console
    std::cout << "Two regular 6-sided dice:";
    for (int i = 0; i < 10; i++)
    {
        std::cout << " " << rollTwoDice(d6_1, d6_2);
    }
    std::cout << std::endl;
    
    // Print 10 rolls from loaded dice to console
    std::cout << "Two loaded 6-sided dice:";
    for (int i = 0; i < 10; i++)
    {
        std::cout << " " << rollTwoDice(ld6_1, ld6_2);
    }
    std::cout << std::endl;
    
    // Print 10 rolls from regular d20 to console
    std::cout << "Roll one regular 20-sided die twice:";
    for (int i = 0; i < 10; i++)
    {
        std::cout << " " << rollTwoDice(d20, d20);
    }
    std::cout << std::endl;
    
    // Print 10 rolls from loaded d20 to console
    std::cout << "Roll one loaded 20-sided die twice:";
    for (int i = 0; i < 10; i++)
    {
        std::cout << " " << rollTwoDice(ld20, ld20);
    }
    std::cout << std::endl;

    // Open a file for CSV output
    std::ofstream ofs ("results.csv");
    
    // Write CSV header row
    ofs << "\"Roll #\",\"Regular d6\",\"Loaded d6\",\"Regular d20\",\"Loaded d20\"" << std::endl;
    
    // Test if file is open and writeable
    if (ofs)
    {
        std::cout << "Saving 1000 rolls to CSV... ";
        // Roll one regular die and one loaded die 1000 times each 
        // for statistical analysis
        for (int i = 1; i <= 1000; i++)
        {
            ofs << i << "," 
                << d6_1.rollDice() << "," 
                << ld6_1.rollDice() << ","
                << d20.rollDice() << ","
                << ld20.rollDice() << std::endl;
        }
        std::cout << "Done!\n";
    }
    else
    {
        std::cout << "ERROR: Could not open output file.\n";
    }
    
    // Close filestream to flush buffer
    ofs.close();
    
    return 0;
}

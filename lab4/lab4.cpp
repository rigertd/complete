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
    // Declare 2 regular 6-sided dice and 2 loaded 6-sided dice
    Dice die1 (6);
    Dice die2 (6);
    LoadedDice ldie1 (6);
    LoadedDice ldie2 (6);
    
    // Print 10 rolls from regular dice to console
    std::cout << "Two regular 6-sided dice:";
    for (int i = 0; i < 10; i++)
    {
        std::cout << " " << rollTwoDice(die1, die2);
    }
    std::cout << std::endl;
    
    // Print 10 rolls from loaded dice to console
    std::cout << "Two loaded 6-sided dice:";
    for (int i = 0; i < 10; i++)
    {
        std::cout << " " << rollTwoDice(ldie1, ldie2);
    }
    std::cout << std::endl;
    
    // Open a file for CSV output
    std::ofstream ofs ("results.csv");
    
    // Write CSV header row
    ofs << "\"Roll #\",\"Regular Dice\",\"Loaded Dice\"" << std::endl;
    
    // Test if file is open and writeable
    if (ofs)
    {
        // Roll one regular die and one loaded die 1000 times each 
        // for statistical analysis
        for (int i = 1; i <= 1000; i++)
        {
            ofs << i << "," 
                << die1.rollDice() << "," 
                << ldie1.rollDice() << std::endl;
        }
    }
    else
    {
        std::cout< "ERROR: Could not open output file.\n";
    }
    
    // Close filestream to flush buffer
    ofs.close();
    
    return 0;
}

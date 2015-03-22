/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           2/16/2015
 * Last Modification Date: 2/16/2015
 * Course:                 CS162_400
 * Assignment:             Lab 7
 * Filename:               createFiles.cpp
 *
 * Overview:
 *     This program creates four files containing random numbers between 1 and 9
 *     with a 0 in the beginning, the middle, the end, or not at all. 
 *     The filenames are specified by the user.
 *
 * Input:
 *     - Filename to save a sequence of random numbers with a 0 in front
 *     - Filename to save a sequence of random numbers with a 0 in the middle
 *     - Filename to save a sequence of random numbers with a 0 in back
 *     - Filename to save a sequence of random numbers with no 0
 *
 * Output:
 *     - File with a sequence of random numbers with a 0 in front
 *     - File with a sequence of random numbers with a 0 in the middle
 *     - File with a sequence of random numbers with a 0 in back
 *     - File with a sequence of random numbers with no 0
 ************************************************************************/
#include <cstdlib>  // srand, rand
#include <ctime>    // time
#include <fstream>
#include <iostream>
#include <string>
#include <limits>

#include "globals.hpp"  // global constants
#include "utility.hpp"  // getFilename

// prototypes
template <typename T>
T getInput(std::string);

void writeRands(std::string &, int, int, int, int, int);

int main()
{
    std::srand(time(0));    // seed random number generator
    int numCount;           // number of numbers to generate
    std::string filename;   // stores filename to write to
    
    // get the number of numbers to generate
    numCount = getInput<int>("Enter the number of numbers to generate: ");
    
    std::cout << "Enter the filenames for each file, "
              << "or leave blank for the default.\n";

    // name of file with 0 in front
    filename = getFilename("Name of file with 0 in front: ", FRONT_DEFAULT);
    
    // fill with numCount - 1 random numbers 0 in front (position 0)
    writeRands(filename, numCount, 1, 9, 0, 0);
    
    // name of file with 0 in middle
    filename = getFilename("Name of file with 0 in middle: ", MIDDLE_DEFAULT);
    
    // fill with numCount - 1 random numbers 0 in middle (position numCount / 2)
    writeRands(filename, numCount, 1, 9, 0, numCount / 2);
    
    // name of file with 0 at end
    filename = getFilename("Name of file with 0 in back: ", BACK_DEFAULT);
    
    // fill with numCount - 1 random numbers 0 in back (position numCount - 1)
    writeRands(filename, numCount, 1, 9, 0, numCount - 1);

    // name of file without a 0
    filename = getFilename("Name of file without a 0: ", NO_ZERO_DEFAULT);
    
    // fill with numCount random numbers and no 0 (position -1)
    writeRands(filename, numCount, 1, 9, 0, -1);
}

// Gets valid user input of type T.
template <typename T>
T getInput(std::string prompt)
{
    T input;    // for storing user input
    
    // prompt user and get input
    std::cout << prompt;
    std::cin >> input;
    
    // validate input
    while (std::cin.get() != '\n')
    {
        // clear keyboard buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::cout << "ERROR: Invalid input. Try again.\n";
        std::cout << prompt;
        std::cin >> input;
    }
    
    return input;
}

// Writes 'count' numbers between 'min' and 'max' to 'fn' with 'target' at 'pos'
void writeRands(std::string &fn, int count, int min, int max, int target, int pos)
{
    std::ofstream out;
    
     // enable file exceptions on failbit and badbit
    out.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
        out.open(fn.c_str());
   
        // fencepost
        if (count > 0)
            if (pos == 0)
                out << target;
            else
                out << rand() % (max - min + 1) + min;

        for (int i = 1; i < count; i++)
        {
            if (pos == i)
                out << " " << target;
            else
                out << " " << rand() % (max - min + 1) + min;
        }
    }
    catch (std::ios_base::failure)
    {
        std::cout << "Error saving to " << fn << std::endl;
    }
    
    out.close();
}

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
 *     - Filename to save a sequence of random numbers with a 0 near the beginning
 *     - Filename to save a sequence of random numbers with a 0 near the middle
 *     - Filename to save a sequence of random numbers with a 0 near the end
 *     - Filename to save a sequence of random numbers with no 0
 *
 * Output:
 *     - File with a sequence of random numbers with a 0 near the beginning
 *     - File with a sequence of random numbers with a 0 near the middle
 *     - File with a sequence of random numbers with a 0 near the end
 *     - File with a sequence of random numbers with no 0
 ************************************************************************/
#include <cstdlib>  // srand, rand
#include <ctime>    // time
#include <fstream>
#include <iostream>

#include "utility.hpp"  // getInput<T>, getString

// prototype
void writeRands(std::ofstream &, int, int, int);

int main()
{
    std::srand(time(0));    // seed random number generator
    int numCount;           // number of numbers to generate
    std::string filename;   // stores filename to write to
    std::ofstream ofs;      // for writing to file
    
    // enable file exceptions on failbit and badbit
    ofs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    // get the number of numbers to generate
    numCount = getInput<int>("Enter the number of numbers to generate: ");
    
    // name of file with 0 in front
    filename = getString("Enter filename to have the 0 in front: ");
    
    try
    {
        // open the file for output
        ofs.open(filename.c_str());
        
        // write a 0
        ofs << "0 ";
        
        // fill the rest with (numCount - 1) random numbers
        writeRands(ofs, numCount - 1, 1, 9);
        
        // close the file
        ofs.close();
    }
    catch (std::ios_base::failure e)
    {
        std::cout << e.what() << std::endl;
    }
    
    // name of file with 0 in middle
    filename = getString("Enter filename to have the 0 in the middle: ");
    
    try
    {
        // open the file for output
        ofs.open(filename.c_str());
        
        // write rands up to half-way point
        writeRands(ofs, numCount / 2, 1, 9);
        
        // write a 0
        ofs << "0 ";
        
        // fill the rest with ((numCount / 2) - 1) random numbers
        writeRands(ofs, (numCount / 2) - 1, 1, 9);
        
        // close the file
        ofs.close();
    }
    catch (std::ios_base::failure e)
    {
        std::cout << e.what() << std::endl;
    }
    
    // name of file with 0 at end
    filename = getString("Enter filename to have the 0 in back: ");
    
    try
    {
        // open the file for output
        ofs.open(filename.c_str());
        
        // fill with (numCount - 1) random numbers
        writeRands(ofs, numCount - 1, 1, 9);
        
        // write a 0
        ofs << "0 ";
        
        // close the file
        ofs.close();
    }
    catch (std::ios_base::failure e)
    {
        std::cout << e.what() << std::endl;
    }

    // name of file without a 0
    filename = getString("Enter filename to have no zero: ");
    
    try
    {
        // open the file for output
        ofs.open(filename.c_str());
        
        // fill with numCount random numbers
        writeRands(ofs, numCount, 1, 9);
        
        // close the file
        ofs.close();
    }
    catch (std::ios_base::failure e)
    {
        std::cout << e.what() << std::endl;
    }
}


// Writes 'count' numbers between 'min' and 'max' to 'out'
void writeRands(std::ofstream &out, int count, int min, int max)
{
    for (int i = 0; i < count; i++)
    {
        out << rand() % (max - min + 1) + min << " ";
    }
}
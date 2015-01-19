/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           1/18/2015
 * Last Modification Date: 1/18/2015
 * Course:                 CS162_400
 * Assignment:             Lab 3
 * Filename:               lab3.cpp
 *
 * Overview:
 *     This program reads numeric values from two input files and merges them
 *     into a single output file in descending order. The program supports
 *     the following command-line syntax:
 *
 *         lab3.exe
 *         lab3.exe [input file 1] [input file 2]
 *         lab3.exe [input file 1] [input file 2] [output file]
 *
 *     If no arguments are specified, default values of 
 *     input1.txt, input2.txt, and output.txt are used.
 *
 * Requirements:
 *     - Check that files open correctly
 *     - Write a function that takes 2 input filestreams 
 *       and 1 output filestream by reference
 *     - Read files one at a time and decide which to write to output
 *     - The output should be in descending order 
 *       (as long as the values in the input files are)
 ************************************************************************/
#include <iostream>
#include <fstream>
#include <string>

// forward declaration
void mergeFiles(std::ifstream &, std::ifstream &, std::ofstream &);

int main(int argc, char **argv)
{
    // declare filename variables
    std::string inputFile1 = "input1.txt";
    std::string inputFile2 = "input2.txt";
    std::string outputFile = "output.txt";
    
    // check if filenames were provided as arguments
    switch (argc)
    {
        case 4:  // input1 input2 output
            outputFile = std::string(argv[3]);
            // FALLTHOUGH
        case 3:  // input1 input2
            inputFile1 = std::string(argv[1]);
            inputFile2 = std::string(argv[2]);
            break;
        case 1:  // no arguments
            std::cout << "No arguments specified.\n\n"
                      << "Using default filenames of input1.txt, "
                      << "input2.txt, and output.txt.\n\n";
            break;
        default: // invalid arguments
            std::cout << "Invalid command line arguments.\n"
                      << "Use the following syntax.\n"
                      << "  lab3.exe [input_file_1] [input_file_2] [output_file]\n\n";
    }
    
    // declare and open filestreams
    std::ifstream ifs1(inputFile1.c_str());
    std::ifstream ifs2(inputFile2.c_str());
    std::ofstream ofs(outputFile.c_str());
    
    // check if files exist and are read/writeable
    if (!ifs1 || !ifs2 || !ofs)
    {
        std::cout << "WARNING: Unable to open file. Please check your arguments.\n";
        return 0;
    }
    
    // call the sorting function
    sortInputFileNumbers(ifs1, ifs2, ofs);
    
    // close file handles
    ifs1.close();
    ifs2.close();
    ofs.close();
    
    std::cout << "Sorting complete. Your output file is " << outputFile << ".\n";
    
    return 0;
}

// Reads input files and outputs numbers in sequential order to output file.
void mergeFiles(std::ifstream &ifs1, std::ifstream &ifs2, std::ofstream &ofs)
{
    // declare input double variables
    double input1;
    double input2;
    
    /* Start by reading one value from each file. Keep reading from one file
     * until a value larger than the one from the other file is found.
     * Then start reading from the other file until a larger value is found.
     * Repeat until EOF or invalid data is encountered in both files.
     */
    ifs1 >> input1;
    ifs2 >> input2;
    do
    {
        while (ifs1.good() && (input1 <= input2 || !ifs2.good()))
        {
            ofs << input1 << std::endl;
            ifs1 >> input1;
        }
        while (ifs2.good() && (input2 <= input1 || !ifs1.good()))
        {
            ofs << input2 << std::endl;
            ifs2 >> input2;
        }
    } while (ifs1.good() || ifs2.good());
}

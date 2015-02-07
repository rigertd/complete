/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           2/4/2015
 * Last Modification Date: 2/6/2015
 * Course:                 CS162_400
 * Assignment:             Lab 5
 * Filename:               lab5.cpp
 *
 * Overview:
 *     This program opens an input file and uses the CopyFilter, UppercaseFilter
 *     EncryptionFilter, and CipherTextFilter classes to transform the data and
 *     write it to separate output files. This program uses a static value for the
 *     encryption key for testing purposes.
 *
 * Syntax:
 *     lab5.exe [input_filename]
 *
 * Output:
 *     CopyOutput.txt
 *     UpperOutput.txt
 *     EncryptionOutput.txt
 *     CiphertextOutput.txt
 ************************************************************************/
#include <iostream>
#include <fstream>

#include "Filter.hpp"
#include "CopyFilter.hpp"
#include "UppercaseFilter.hpp"
#include "EncryptionFilter.hpp"
#include "CipherTextFilter.hpp"

// global constant
int KEY = 2;    // encryption key

int main(int argc, char **argv)
{
    std::ifstream ifs;  // input file stream
    std::ofstream ofs;  // output file stream
    
    // parse command-line arguments (if any)
    if (argc == 1)
    {
        ifs.open("input.txt");
    }
    else if (argc == 2)
    {
        ifs.open(argv[1]);
    }
    else
    {
        // display error message if more than 1 argument
        std::cout << "Invalid arguments.\n"
                  << "Specify input file only or no argument to use \"input.txt\"\n";
        return 1;
    }
    
    // validate ifstream
    if (!ifs)
    {
        std::cout << "Cannot open input file.\n";
        ifs.close();
        
        return 1;
    }
    
    // create an unchanged copy
    CopyFilter cf;
    ofs.open("CopyOutput.txt");
    cf.doFilter(ifs, ofs);
    ofs.close();
    
    // create an uppercase copy
    UppercaseFilter uf;
    ifs.clear();
    ifs.seekg(0, ifs.beg);
    ofs.open("UpperOutput.txt");
    uf.doFilter(ifs, ofs);
    ofs.close();
    
    // create a Caesar cipher-encrypted copy
    EncryptionFilter ef (KEY);
    ifs.clear();
    ifs.seekg(0, ifs.beg);
    ofs.open("EncryptionOutput.txt");
    ef.doFilter(ifs, ofs);
    ofs.close();
    
    // create a ciphertext copy
    CipherTextFilter ctf (KEY);
    ifs.clear();
    ifs.seekg(0, ifs.beg);
    ofs.open("CiphertextOutput.txt");
    ctf.doFilter(ifs, ofs);
    ofs.close();
    
    // close input file stream
    ifs.close();

    return 0;
}

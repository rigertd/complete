#include <iostream>
#include <fstream>

#include "Filter.hpp"
#include "CopyFilter.hpp"
#include "UppercaseFilter.hpp"
#include "EncryptionFilter.hpp"
#include "CipherTextFilter.hpp"

int main(int argc, char **argv)
{
    std::ifstream ifs;
    std::ofstream ofs;
    
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
    
    CopyFilter cf;
    ofs.open("CopyOutput.txt");
    cf.doFilter(ifs, ofs);
    ofs.close();
    
    UppercaseFilter uf;
    ifs.clear();
    ifs.seekg(0, ifs.beg);
    ofs.open("UpperOutput.txt");
    uf.doFilter(ifs, ofs);
    ofs.close();
    
    EncryptionFilter ef (2);
    ifs.clear();
    ifs.seekg(0, ifs.beg);
    ofs.open("EncryptionOutput.txt");
    ef.doFilter(ifs, ofs);
    ofs.close();
    
    CipherTextFilter ctf (2);
    ifs.clear();
    ifs.seekg(0, ifs.beg);
    ofs.open("CipherTextOutput.txt");
    ctf.doFilter(ifs, ofs);
    ofs.close();
    
    
    ifs.close();

    return 0;
}

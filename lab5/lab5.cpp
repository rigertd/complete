#include <iostream>
#include <fstream>

#include "Filter.hpp"
#include "CopyFilter.hpp"
#include "UppercaseFilter.hpp"

int main(int argc, char **argv)
{
    std::ifstream ifs;
    std::ofstream ofs;
    
    // parse command-line arguments (if any)
    if (argc == 2)
    {
        ifs.open(argv[1]);
        ofs.open("output.txt");
    }
    else if (argc == 3)
    {
        ifs.open(argv[1]);
        ofs.open(argv[2]);
    }
    else
    {
        ifs.open("input.txt");
        ofs.open("output.txt");
    }
    
    // validate fstreams
    if (!ifs)
    {
        std::cout << "Cannot open input file.\n";
        ifs.close();
        ofs.close();
        
        return 1;
    }
    else if (!ofs)
    {
        std::cout << "Cannot open output file.\n";
        ifs.close();
        ofs.close();
        
        return 1;
    }
    
    CopyFilter cf;
    UppercaseFilter uf;
    
    uf.doFilter(ifs, ofs);
    
    ifs.close();
    ofs.close();

    return 0;
}
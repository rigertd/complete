/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/4/2015
 * Last Modified:   2/6/2015
 * Assignment:      Lab 5
 * Filename:        Filter.cpp
 *
 * Description:     Implementation of the abstract Filter class.
 *                  Applies the transform function to every character of
 *                  the input file.
 ************************************************************************/
#include "Filter.hpp"

void Filter::doFilter(std::ifstream &in, std::ofstream &out)
{
    char buffer;
    
    // only apply if both input and output file streams are good
    while (in && out)
    {
        // read 1 char
        buffer = in.get();
        
        // transform char and write to output if not EOF
        if (buffer != EOF)
        {
            buffer = this->transform(buffer);
            out.put(buffer);
        }
    }
}

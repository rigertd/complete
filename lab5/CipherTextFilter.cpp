/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/4/2015
 * Last Modified:   2/6/2015
 * Assignment:      Lab 5
 * Filename:        CipherTextFilter.cpp
 *
 * Description:     Implementation of the CipherTextFilter class.
 *                  This class is derived from the EncryptionFilter class
 *                  and is a friend of the UppercaseFilter class.
 ************************************************************************/
#include "CipherTextFilter.hpp"
#include "UppercaseFilter.hpp"

// Transforms the specified char c.
// Applies the UppercaseFilter transform function
// followed by the EncryptionFilter transform function.
char CipherTextFilter::transform(char c)
{
    UppercaseFilter uf;
    char buffer = uf.transform(c);
    buffer = this->EncryptionFilter::transform(buffer);
    return buffer;
}

// Applies the transform function to every alphabetical letter
// of the input file and outputs the result to the output file
// with a space every 5 characters.
void CipherTextFilter::doFilter(std::ifstream &in, std::ofstream &out)
{
    char buffer;
    int count = 0;
    while (in && out)
    {
        buffer = in.get();
        
        if (buffer != EOF)
        {
            if (
                (buffer >= 'a' && buffer <= 'z') ||
                (buffer >= 'A' && buffer <= 'Z')
                )
            {
                buffer = this->transform(buffer);
                out.put(buffer);
                if (++count == 5)
                {
                    out.put(' ');
                    count = 0;
                }
            }
        }
    }
}

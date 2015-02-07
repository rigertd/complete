/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/4/2015
 * Last Modified:   2/6/2015
 * Assignment:      Lab 5
 * Filename:        UppercaseFilter.cpp
 *
 * Description:     Implementation of the UppercaseFilter class.
 *                  Applies the transform function to every character of
 *                  the input file.
 ************************************************************************/
#include "UppercaseFilter.hpp"

// converts lowercase letters to uppercase.
// otherwise, returns the char unchanged.
char UppercaseFilter::transform(char c)
{
    if (c >= 'a' && c <= 'z')
        return c - 32;
    else
        return c;
}

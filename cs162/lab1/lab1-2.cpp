/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           1/4/2015
 * Last Modification Date: 1/4/2015
 * Course:                 CS162_400
 * Assignment:             Lab 1, program 1
 * Filename:               lab1-1.cpp
 *
 * Overview:
 *     This program includes two custom header files and calls a function
 *     from each file.
 *
 * Requirements:
 *     - Include required header files (f.h and g.h)
 *     - Call f function and g function
 ************************************************************************/
#include "f.h"
#include "g.h"

int main()
{
    // call 'f' function defined in f.h
    f();
    
    // call 'g' function defined in g.h
    g();
    
    return 0;
}

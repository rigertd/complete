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
 ************************************************************************/
#include "f.h"
#include "g.h"

int main()
{
    // call 'f' function in f.h
    f();
    
    // call 'g' function in g.h
    g();
    
    return 0;
}

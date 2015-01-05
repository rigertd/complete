/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           1/4/2015
 * Last Modification Date: 1/4/2015
 * Course:                 CS162_400
 * Assignment:             Lab 1, program 1
 * Filename:               lab1-1.cpp
 *
 * Overview:
 *     This program displays three integer values, sorts them into 
 *     ascending order, and displays the sorted values.
 *
 * Requirements:
 *     - Function takes 3 pointers to int as arguments
 *     - Put lowest first, second lowest in middle, and highest last
 *     - No return value (void)
 *     - Print values before and after calling function
 ************************************************************************/
#include <iostream>

// sorts three integer values into ascending order from left to right
void sort(int *, int *, int *);

// swaps two integer values using pass by reference
void swap(int &, int &);

int main()
{
    int x = 2, y = 5, z = 1; // initial values
    
    // display initial values
    std::cout << "x = " << x << "  y = " << y << "  z = " << z << std::endl;
    
    // sort values
    sort(&x, &y, &z);
    
    // display sorted values
    std::cout << "x = " << x << "  y = " << y << "  z = " << z << std::endl;
    
    return 0;
}

// sorts three integer values into ascending order from left to right
void sort(int *x, int *y, int *z)
{
    // manually implement bubble sort
    int tmp;
    
    if (*x > *y)
        swap(*x, *y);
    if (*y > *z)
        swap (*y, *z);
    if (*x > *y)
        swap (*x, *y);
}

// swaps two integer values using pass by reference
void swap(int &a, int &b)
{
    int tmp = a;
    a = b;
    b = tmp;
}
/**********************************************************
* Author:                 David Rigert
* Date Created:           10/18/2014
* Last Modification Date: 10/18/2014
* Assignment:             Assignment 3
* Filename:               randNum.cpp
*
* Overview:
*     This program prints out 10 pseudorandom numbers
*     between 1 and 100.
* 
* Input:
*     None
*
* Output:
*     The output to the console will be in the form of:
*
*     1  (different every time)
*     2  (different every time)
*     3  (different every time)
*     ...
*     10 (different every time)
**********************************************************/
#include <ctime>    // For time (as seed)
#include <iostream> // For cout
#include <cstdlib>  // For rand and srand

int main()
{
    const int MAX_NUM = 100;    // Maximum random number
    const int MIN_NUM = 1;      // Minimum random number
    const int NUM_COUNT = 10;   // Number of numbers to print
    unsigned int seed;          // seed for srand function

    seed = std::time(0); // Set seed value to seconds since epoch
    std::srand(seed);    // Set the seed value for rand
    
    // Print NUM_COUNT random numbers
    for (int i = 0; i < NUM_COUNT; i++)
    {
        // Print numbers in range of MIN_NUM to MAX_NUM
        std::cout << (rand() % (MAX_NUM + 1 - MIN_NUM) + MIN_NUM) << std::endl;
    }

    return 0;
}

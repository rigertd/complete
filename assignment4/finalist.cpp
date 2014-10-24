/**********************************************************
 * Author:                 David Rigert
 * Date Created:           10/23/2014
 * Last Modification Date: 10/23/2014
 * Assignment:             Assignment 4
 * Filename:               finalist.cpp
 *
 * Overview:
 *     This program randomly picks 4 unique numbers
 *     between 1 and 25, and prints the results to the console.
 *     Each number represents a winning finalist to receive a prize.
 * 
 * Input:
 *     None
 *
 * Output:
 *     The output to the console will be in the form of:
 *
 *     And the winners are...
 *     Finalist #23!
 *     Finalist #16!
 *     Finalist #4!
 *     and the last winner is... Finalist #6!
 *
 *     Congratulations to our winners!
 *********************************************************/
#include <ctime>     // For time (as seed)
#include <iostream>  // For cout
#include <cstdlib>   // For rand and srand
#include <string>    // For string class

// Function prototypes

// Generates and returns a random int between min and max.
int rand_int(int, int);
// Prints dots to the console with a delay between each.
void PrintSlowDots(unsigned int, unsigned int);
// Sleeps for the specified number of seconds.
void sleep(unsigned int);

int main()
{
    const int MAX = 25;  // Maximum finalist number
    const int MIN = 1;   // Minimum finalist number
    unsigned int seed;   // seed for srand function
    int winner1, winner2, winner3, winner4; // Winning finalists

    seed = std::time(0); // Set seed value to seconds since epoch
    std::srand(seed);    // Set the seed value for rand
    
    // Get first winner
    winner1 = rand_int(MIN, MAX);
    
    // Get second winner until unique one is found
    do
    {
        winner2 = rand_int(MIN, MAX);
    }
    while (winner2 == winner1);
    
    // Get third winner until unique one is found
    do
    {
        winner3 = rand_int(MIN, MAX);
    }
    while (winner3 == winner1 || winner3 == winner2);
    
    // Get fourth winner until unique one is found
    do
    {
        winner4 = rand_int(MIN, MAX);
    }
    while (winner4 == winner1 || winner4 == winner2
           || winner4 == winner3);

    // Present the winners in a suspenseful way
    std::cout << "And the winners are";
    PrintSlowDots(1, 3);
    
    // I wish we could use arrays for this...
    // Print finalists
    std::cout << "\nFinalist #" << winner1 << "!";
    sleep(1);
    std::cout << "\nFinalist #" << winner2 << "!";
    sleep(1);
    std::cout << "\nFinalist #" << winner3 << "!";
    sleep(1);
    std::cout << "\nAnd the last winner is";
    // More suspense for last finalist
    PrintSlowDots(1, 3);
    std::cout << " Finalist #" << winner4 << "!\n\n"
              << "Congratulations to our winners!\n";
    
    return 0;
}

/**********************************************************
 *  int rand_int(int min, int max)
 *
 *  Purpose: This function generates a random number between
 *           the specified min and max values.
 *
 *  Preconditions: Seed is set for rand() and min <= max.
 *   
 *  Postconditions: Returns a random number between min and max.
 *********************************************************/
int rand_int(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}

/**********************************************************
 *  void PrintSlowDots(unsigned int delay, int dots)
 *
 *  Purpose: This function prints 'dots' periods to the console
 *           with a delay of 'delay' seconds between each period.
 *
 *  Preconditions: 'delay' and 'dots' are positive integers
 *   
 *  Postconditions: Prints 'dots' dots to the console
 *********************************************************/
void PrintSlowDots(unsigned int delay, unsigned int dots)
{
    for (int i = 0; i < dots; i++)
    {
        sleep(delay);
        std::cout << ".";
    }
}

/**********************************************************
 *  void PrintSlowDots(unsigned int sec)
 *
 *  Purpose: This function sleeps for 'sec' seconds
 *           and then returns.
 *
 *  Preconditions: 'sec' is a positive integer
 *   
 *  Postconditions: Sleeps for 'sec' seconds
 *********************************************************/
void sleep(unsigned int sec)
{
    unsigned int start = std::time(0);
    // Stay in loop until 'sec' seconds have elapsed.
    while ((std::time(0) - start) < sec);
}
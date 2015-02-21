/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           2/13/2015
 * Last Modification Date: 2/18/2015
 * Course:                 CS162_400
 * Assignment:             Lab 7
 * Filename:               lab7.cpp
 *
 * Overview:
 *     This program runs various searching and sorting algorithms on 
 *     different input files. It also calculates the number of ticks required
 *     to run each searching and sorting algorithm.
 *
 * Input:
 *     1 - Load files with 0 in front, in middle, at end, and none at all
 *     2 - Test the linear searching algorithm
 *     3 - Test the binary searching algorithm
 *     4 - Test the selection sorting algorithm
 *     5 - Exit the program
 *
 * Output:
 *     Ticks and milliseconds required for specified search/sort algorithm
 ************************************************************************/
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>

#include "utility.hpp"

// prints the activity menu
void printMenu();

// runs a binary search on a sorted vector and returns the index
int binaryFind(std::vector<int> &, int);

// runs a linear search on a file and returns the index
int linearFind(std::vector<int> &, int);

// loads the values in the specified file into a vector
void loadFile(const char *, std::vector<int> &);

// sorts an array into ascending order using the selection sort algorithm
void selectionSort(std::vector<int> &);

// gets a string from the user
std::string getString(std::string);

int main()
{
    unsigned selection = 0;                 // stores user input for menu

    std::string earlyZero;  // file with zero in front
    std::string middleZero; // file with zero in middle
    std::string lateZero;   // file with zero at end
    std::string noZero;     // file without a zero
    
    // keeps track of whether values have been sorted
    bool sorted = false;

    // vectors for use in sorting algorithm
    std::vector<int> earlyZeroVals;
    std::vector<int> middleZeroVals;
    std::vector<int> lateZeroVals;
    std::vector<int> noZeroVals;
    
    // menu loop
    do
    {
        printMenu();
        
        // get user selection and clear input buffer
        std::cout << "Selection: ";
        std::cin >> selection;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << std::endl;

        switch (selection)
        {
        case 1: // load the input files
            earlyZero = getString("Enter the file with the 0 in front: ");
            loadFile(earlyZero.c_str(), earlyZeroVals);
            middleZero = getString("Enter the file with the 0 in the middle: ");
            loadFile(middleZero.c_str(), middleZeroVals);
            lateZero = getString("Enter the file with the 0 in the back: ");
            loadFile(lateZero.c_str(), lateZeroVals);
            noZero = getString("Enter the file without a 0 in it: ");
            loadFile(noZero.c_str(), noZeroVals);
            std::cout << std::endl;
            sorted = false; // clear sorted flag
            break;
        case 2: // linear search
            std::cout << "Finding 0 in file with 0 in front...\n";
            linearFind(earlyZeroVals, 0);
            std::cout << std::endl;
            std::cout << "Finding 0 in file with 0 in middle...\n";
            linearFind(middleZeroVals, 0);
            std::cout << std::endl;
            std::cout << "Finding 0 in file with 0 in back...\n";
            linearFind(lateZeroVals, 0);
            std::cout << std::endl;
            std::cout << "Finding 0 in file without a 0...\n";
            linearFind(noZeroVals, 0);
            std::cout << std::endl;
            break;
        case 3: // binary search
            if (sorted)
            {
                std::cout << "Finding 0 in file with 0 in front...\n";
                binaryFind(earlyZeroVals, 0);
                std::cout << std::endl;
                std::cout << "Finding 0 in file with 0 in middle...\n";
                binaryFind(middleZeroVals, 0);
                std::cout << std::endl;
                std::cout << "Finding 0 in file with 0 in back...\n";
                binaryFind(lateZeroVals, 0);
                std::cout << std::endl;
                std::cout << "Finding 0 in file without a 0...\n";
                binaryFind(noZeroVals, 0);
                std::cout << std::endl;
            }
            else
            {
                std::cout << "The files must be sorted first.\n";
            }
            std::cout << std::endl;
            break;
        case 4: // selection sort
            std::cout << "Running selection sort on file with 0 in front...\n";
            selectionSort(earlyZeroVals);
            std::cout << std::endl;
            std::cout << "Running selection sort on file with 0 in middle...\n";
            selectionSort(middleZeroVals);
            std::cout << std::endl;
            std::cout << "Running selection sort on file with 0 in back...\n";
            selectionSort(lateZeroVals);
            std::cout << std::endl;
            std::cout << "Running selection sort on file without a 0...\n";
            selectionSort(noZeroVals);
            std::cout << std::endl;
            sorted = true;  // set sorted flag
            break;
        case 5: // exit the program
            std::cout << "Goodbye!" << std::endl;
            break;
        default:
            std::cout << "Invalid selection.\n";
        }
    } while (selection != 5);
    
    return 0;
}

// Prints the menu of activities.
void printMenu()
{
    std::cout << "\nSelect an activity.\n\n"
              << " 1: Load the input files\n"
              << " 2: Run the linear search algorithm\n"
              << " 3: Run the binary search algorithm\n"
              << " 4: Run the selection sort algorithm\n"
              << " 5: Exit the program\n\n";
}

// Finds the index of a value using a binary search
// Returns -1 if value is not found.
int binaryFind(std::vector<int> &vals, int value)
{
    clock_t t;
    t = clock();    // save current tick value
    
    int startIndex = 0;                // start of search range
    int endIndex = vals.size() - 1;    // end of search range
    int index = -1;                         // stores index if value is found
    unsigned iterCount = 0;                 // number of iterations required
    
    while (startIndex <= endIndex && index < 0) // stop when index found or start == end
    {
        // set midpoint based on current start and end
        int midIndex = (startIndex + endIndex) / 2;
        
        if (value > vals[midIndex])         // midpoint too low
            startIndex = midIndex + 1;      // set start of range to midpoint + 1
        else if (value < vals[midIndex])    // midpoint too high
            endIndex = midIndex - 1;        // set end of range to midpoint - 1
        else                                // found value; set index
            index = midIndex;
        
        iterCount++;
    }
    
    t = clock() - t;    // get difference in tick value
    
    std::cout << "binaryFind found " << value << " at index " << index << ".\n";
    std::cout << "binaryFind ran for " << iterCount << " iterations.\n";
    std::cout << "This took " << t << " ticks (" 
              << static_cast<float>(t)/CLOCKS_PER_SEC << " seconds).\n";
    
    return index;
}

// Finds the index of the first instance of value in the file.
// Stops searching once it finds the value.
// Returns -1 if value is not found.
int linearFind(std::vector<int> &vals, int value)
{
    clock_t t;
    t = clock();    // save current tick value

    unsigned count = 0;         // counts the number of numbers read
    int index = -1;             // holds the index of the number
    
    while (count < vals.size())
    {
        if (vals[count++] == value)
        {
            index = count;
            break;
        }
    }

    t = clock() - t;    // get difference in tick value
    
    std::cout << "linearFind found " << value 
              << " at index " << index << ".\n";
    std::cout << "linearFind loop ran for " << count << " iterations.\n";
    std::cout << "This took " << t << " ticks (" 
              << static_cast<float>(t)/CLOCKS_PER_SEC << " seconds).\n";
    
    return index;
}

// Loads the values in the specified file into a vector.
void loadFile(const char *filename, std::vector<int> &vals)
{
    int buffer;                 // holds the number read from the file

    vals.clear();               // delete any existing data
    std::ifstream in(filename); // open the file
    
    // enable file exceptions on failbit and badbit
    in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    std::cout << "Loading numbers from " << filename << "... ";
    try
    {
        while (in >> buffer)
        {
            vals.push_back(buffer); // add value to vector
        }
        in.close();                 // close the file
        std::cout << "Done.\n";
    }
    catch (std::ios_base::failure e)
    {
        std::cout << e.what() << std::endl;
    }
}

// Sorts an array into ascending order using the selection sort algorithm.
void selectionSort(std::vector<int> &vals)
{
    clock_t t;
    t = clock();    // save current tick value

    int startIndex;// starting point for current iteration
    int minIndex;  // index of the minimum value
    int minValue;       // minimum value (so far)
    unsigned iterCount = 0; // counts the iterations required for sort
    
    // start from first element, find smallest value in vector
    // and swap with the current start element
    for (startIndex = 0; startIndex < vals.size() - 1; startIndex++)
    {
        minIndex = startIndex;
        minValue = vals[startIndex]; // start with current value as min
        
        // check all elements starting from the next one
        for (int i = startIndex + 1; i < vals.size(); i++)
        {
            // update minValue and minIndex if smaller value is found
            if (vals[i] < minValue)
            {
                minValue = vals[i];
                minIndex = i;
            }
            
            iterCount++;    // increment the iteration counter
        }
        
        // minValue and minIndex now have the minimum value and its index
        // swap current index with minimum
        vals[minIndex] = vals[startIndex];
        vals[startIndex] = minValue;
    }

    t = clock() - t;    // get difference in tick value

    std::cout << "Sorted array in " << iterCount << " iterations.\n";
    std::cout << "This took " << t << " ticks (" 
              << static_cast<float>(t)/CLOCKS_PER_SEC << " seconds).\n";
}

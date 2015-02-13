/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           2/13/2015
 * Last Modification Date: 2/13/2015
 * Course:                 CS162_400
 * Assignment:             Lab 6
 * Filename:               lab6.cpp
 *
 * Overview:
 *     This program runs various searching and sorting algorithms on four
 *     different input files. It also saves the results of the sorting algorithm
 *     to a user-specified output file.
 *
 * Input:
 *     nozero.txt - A sequence of random numbers with no 0 in it
 *     early.txt  - A sequence of random numbers with a 0 near the beginning
 *     middle.txt - A sequence of random numbers with a 0 near the middle
 *     late.txt   - A sequence of random numbers with a 0 near the end
 *
 * Output:
 *     Activity 1 - Index of '0' in each file
 *     Activity 2 - Outputs values sorted into ascending order to user-specified file
 *     Activity 3 - Index of '5' in each file
 ************************************************************************/
#include <fstream>
#include <iostream>
#include <vector>

// prints the activity menu
void printMenu();

// runs a binary search on a sorted vector and returns the index
int binaryFind(std::vector<int> &, int);

// runs a linear search on a file and returns the index
int linearFind(const char *, int);

// loads the values in the specified file into a vector
void loadFile(const char *, std::vector<int> &);

// prompts user for filename and saves vector to it
void saveFile(const std::vector<int> &); 

// sorts an array into ascending order using the selection sort algorithm
void selectionSort(std::vector<int> &);

int main()
{
    const char* noZero = "nozero.txt";      // random with no '0'
    const char* earlyZero = "early.txt";    // random with a '0' early
    const char* middleZero = "middle.txt";  // random with a '0' in middle
    const char* lateZero = "late.txt";      // random numbers with a '0' late
    unsigned selection = 0;                 // stores user input for menu

    // vectors for use in sorting algorithm
    std::vector<int> noZeroVals;
    std::vector<int> earlyZeroVals;
    std::vector<int> middleZeroVals;
    std::vector<int> lateZeroVals;
    
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
        case 1: // linear search in random numbers
            linearFind(noZero, 0);
            std::cout << std::endl;
            linearFind(earlyZero, 0);
            std::cout << std::endl;
            linearFind(middleZero, 0);
            std::cout << std::endl;
            linearFind(lateZero, 0);
            break;
        case 2: // sort input files and write them to an output file
            
            // input file with no 0 in it
            loadFile(noZero, noZeroVals);
            std::cout << "Sorting " << noZero << "...\n";
            selectionSort(noZeroVals);
            std::cout << "Saving " << noZero << " values...\n";
            saveFile(noZeroVals);
            
            // input file with 0 near beginning
            loadFile(earlyZero, earlyZeroVals);
            std::cout << "Sorting " << earlyZero << "...\n";
            selectionSort(earlyZeroVals);
            std::cout << "Saving " << earlyZero << " values...\n";
            saveFile(earlyZeroVals);
            
            // input file with 0 near middle
            loadFile(middleZero, middleZeroVals);
            std::cout << "Sorting " << middleZero << "...\n";
            selectionSort(middleZeroVals);
            std::cout << "Saving " << middleZero << " values...\n";
            saveFile(middleZeroVals);
            
            // input file with 0 near end
            loadFile(lateZero, lateZeroVals);
            std::cout << "Sorting " << lateZero << "...\n";
            selectionSort(lateZeroVals);
            std::cout << "Saving " << lateZero << " values...\n";
            saveFile(lateZeroVals);
            break;
        case 3: // sort input files and run binary search

            // input file with no 0 in it
            loadFile(noZero, noZeroVals);
            std::cout << "Sorting " << noZero << "...\n";
            selectionSort(noZeroVals);
            binaryFind(noZeroVals, 5);
            std::cout << std::endl;

            // input file with 0 near beginning
            loadFile(earlyZero, earlyZeroVals);
            std::cout << "Sorting " << earlyZero << "...\n";
            selectionSort(earlyZeroVals);
            binaryFind(earlyZeroVals, 5);
            std::cout << std::endl;

            // input file with 0 near middle
            loadFile(middleZero, middleZeroVals);
            std::cout << "Sorting " << middleZero << "...\n";
            selectionSort(middleZeroVals);
            binaryFind(middleZeroVals, 5);
            std::cout << std::endl;

            // input file with 0 near end and no 5s
            loadFile(lateZero, lateZeroVals);
            std::cout << "Sorting " << lateZero << "...\n";
            selectionSort(lateZeroVals);
            binaryFind(lateZeroVals, 5);
            break;
        case 4: // exit the program
            std::cout << "\nGoodbye!" << std::endl;
            break;
        default:
            std::cout << "\nInvalid selection.\n";
        }
    } while (selection != 4);
    
    return 0;
}

// Prints the menu of activities.
void printMenu()
{
    std::cout << "\nSelect an activity.\n\n"
              << " 1: Find the index of '0' with a linear search\n"
              << " 2: Sort all 4 sets of numbers and write to a file\n"
              << " 3: Find the index of '5' with a binary search\n"
              << " 4: Exit the program\n\n";
}

// Finds the index of a value using a binary search
int binaryFind(std::vector<int> &vals, int value)
{
    unsigned startIndex = 0;                // start of search range
    unsigned endIndex = vals.size() - 1;    // end of search range
    int index = -1;                         // stores index if value is found
    unsigned iterCount = 0;                 // number of iterations required
    
    while (startIndex <= endIndex && index < 0) // stop when index found or start == end
    {
        // set midpoint based on current start and end
        int midIndex = (startIndex + endIndex) / 2;
        
        if (value == vals[midIndex])        // found value; set index
            index = midIndex;
        else if (value < vals[midIndex])    // too high, set end of range to midpoint - 1
            endIndex = midIndex - 1;
        else                                // too low, set start of range to midpoint + 1
            startIndex = midIndex + 1;
        
        iterCount++;
    }
    
    std::cout << "binaryFind found " << value << " at index " << index << ".\n";
    std::cout << "binaryFind ran for " << iterCount << " iterations.\n";
    
    return index;
}

// Finds the index of the first instance of value in the file.
// Stops searching once it finds the value.
// Returns -1 if value is not found.
int linearFind(const char *filename, int value)
{
    int buffer;                 // holds the number read from the file
    unsigned count = 0;         // counts the number of numbers read
    int index = -1;             // holds the index of the number
    std::ifstream in(filename); // open the file
    
    while (in >> buffer)
    {
        if (buffer == value)
        {
            index = count;
            break;
        }
        count++;
    }
    
    in.close();                 // close the file
    
    std::cout << "linearFind found " << value 
              << " at index " << index << " in " << filename << "\n";
    std::cout << "linearFind loop ran for " << count << " iterations.\n";
    
    return index;
}

// Loads the values in the specified file into a vector.
void loadFile(const char *filename, std::vector<int> &vals)
{
    int buffer;                 // holds the number read from the file

    vals.clear();               // delete any existing data
    std::ifstream in(filename); // open the file
    
    std::cout << "Loading numbers from " << filename << "... ";
    while (in >> buffer)
    {
        vals.push_back(buffer); // add value to vector
    }
    in.close();                 // close the file
    std::cout << "Done.\n";
}

// Prompts user for a filename and saves the vector values to it.
void saveFile(const std::vector<int> &vals)
{
    std::string outFile;    // stores the output filename

    // prompt user for output filename
    std::cout << "Enter save filename: ";
    std::getline(std::cin, outFile);
            
    // open file
    std::ofstream out(outFile.c_str());
    if (out)
    {
        for (int i = 0; i < vals.size(); i++)
        {
            out << vals[i] << " ";
        }
        out << std::endl;
    }
    else
    {
        std::cout << "ERROR: Cannot write to file.\n";
    }
    
    // close file
    out.close();
}

// Sorts an array into ascending order using the selection sort algorithm.
void selectionSort(std::vector<int> &vals)
{
    unsigned startIndex;// starting point for current iteration
    unsigned minIndex;  // index of the minimum value
    int minValue;       // minimum value (so far)
    unsigned iterCount = 0; // counts the iterations required for sort
    
    // display the starting order
    std::cout << "Starting order: ";
    for (int i = 0; i < vals.size(); i++)
    {
        std::cout << vals[i] << " ";
    }
    std::cout << std::endl;
    
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
    
    // display the sorted order
    std::cout << "After sorting: ";
    for (int i = 0; i < vals.size(); i++)
    {
        std::cout << vals[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Sorted array in " << iterCount << " iterations.\n";
}

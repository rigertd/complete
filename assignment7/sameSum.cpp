/**********************************************************
 * Author:                 David Rigert
 * Date Created:           11/7/2014
 * Last Modification Date: 11/7/2014
 * Assignment:             Assignment 6
 * Filename:               wordShift.cpp
 *
 * Overview:
 *     This program determines whether sums of the numbers in
 *     the rows, columns, and major diagonals of a square array
 *     are equal.
 *     It prompts the user for the number of rows in the array
 *     and then for the values in each row.
 * 
 * Input:
 *     number of rows, values for each row
 *
 * Output:
 *     The output to the console will be in the form of:
 *
 *     This program determines whether sums of the numbers in
 *     the rows, columns, and major diagonals of a square array
 *     are equal.
 *  
 *     Enter the number of rows in your square array, up to 10: [3]
 *     Enter 3 values for row 1, separated by spaces: [1 1 1]
 *     Enter 3 values for row 2, separated by spaces: [1 1 1]
 *     Enter 3 values for row 3, separated by spaces: [1 1 1]
 *
 *     All sums in your square array ARE equal.
 *********************************************************/
#include <iostream>

// Global constants
const int MAX_COLUMNS = 10;
const int MAX_ROWS = 10;

// Type definitions
typedef int squareArray[][MAX_COLUMNS];

// Function prototypes
// Determines whether all rows, columns, and major diagonals have same sum.
bool hasSameSum(const squareArray, int);

int main()
{
    int arrSize = 0;
    int squareArr[MAX_ROWS][MAX_COLUMNS];
    std::cout << "This program determines whether sums of the numbers in\n"
              << "the rows, columns, and major diagonals of a square array\n"
              << "are equal.\n\n"
              << "Enter the number of rows in your square array, up to "
              << MAX_ROWS << ": ";
    std::cin >> arrSize;

    // Validate input.
    while (std::cin.fail() || arrSize < 1 || arrSize > MAX_ROWS)
    {
        std::cin.clear(); // remove error bit
        std::cin.sync();  // clear input buffer
        std::cout << "The number of rows must be between 1 and "
                  << MAX_ROWS << ": ";
        std::cin >> arrSize;
    }


    // get values for each row
    for (int i = 0; i < arrSize; i++)
    {
        // clear input buffer
        std::cin.clear();
        std::cin.sync();

        int j = 0;           // counts valid nums
        std::string invalid; // invalid input

        std::cout << "Enter "
                  << arrSize
                  << " values for row "
                  << i + 1 
                  << ", separated by spaces: ";
        
        while (j < arrSize)
        {
            std::cin >> squareArr[i][j];
            // input validation
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin >> invalid;
                std::cout << invalid << " is invalid input. Skipping.\n";
            }
            else
            {
                j++;
            }
        }
        
    }
    
    std::cout << "\nAll sums in your square array ARE "
              << (hasSameSum(squareArr, arrSize) ? "" : "NOT ")
              << "equal.\n";

    return 0;
}

/**********************************************************
 *  bool hasSameSum(const squareArray arr, int rows)
 *
 *  Purpose: This function determines whether all rows, columns,
 *           and major diagonals have the same sum.
 *
 *  Preconditions: arr is a square 2D array with up to 
 *                 MAX_COLUMNS columns and MAX_ROWS rows
 *   
 *  Postconditions: Returns whether the sums are equal
 *********************************************************/
bool hasSameSum(const squareArray arr, int rows)
{
    int *rowSums = new int[rows]; // holds sums of rows
    int *colSums = new int[rows]; // holds sums of columns
    int topLeftDiag = 0;          // holds top-right to bottom-left diagonal
    int topRightDiag = 0;         // holds top-left to bottom-right diagonal
    bool result = true;           // holds whether the sums are equal
    
    // initialize colSums array
    for (int i = 0; i < rows; i++)
        colSums[i] = 0;
    
    for (int i = 0; i < rows; i++)
    {
        // initialize current row to 0
        rowSums[i] = 0;
        
        for (int j = 0; j < rows; j++)
        {
            // count nums in row
            rowSums[i] += arr[i][j];
            
            // count nums in col
            colSums[j] += arr[i][j];
            
            // count top-left diagonal
            if (i == j)
                topLeftDiag += arr[i][j];
            
            // count top-right diagonal
            if (i == (rows - j - 1))
                topRightDiag += arr[i][j];
        }
    }
    
    // check if diagonals are equal
    result = topLeftDiag == topRightDiag;
    
    // check if all sums are equal
    for (int i = 0; i < rows && result; i++)
    {
        // check rows and columns
        result = topLeftDiag == rowSums[i] && rowSums[i] == colSums[i];
    }
    
    // free dynamically allocated arrays
    delete [] rowSums;
    delete [] colSums;
    rowSums = colSums = 0;
    
    return result;
}
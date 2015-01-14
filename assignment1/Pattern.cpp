/*************************************************************************
 * Author:          David Rigert
 * Date Created:    1/10/2015
 * Last Modified:   1/13/2015
 * Assignment:      Assignment 1
 * Filename:        Pattern.cpp
 *
 * Description:     Implementation of the Pattern class.
 ************************************************************************/
#include <stdexcept>
#include <algorithm>
#include "Pattern.hpp"

// Constructs a Pattern object with the specified name and pattern
Pattern::Pattern(std::string name, vgrid &sourceGrid)
{
    this->name = name;
    
    // Copy source grid to member variable
    std::copy(sourceGrid.begin(), sourceGrid.end(), std::back_inserter(grid));
}

// Gets whether the specified cell is alive
bool Pattern::getCellState(int x, int y)
{
    // Validate arguments
    if (x < 0 || y < 0 || x >= getWidth() || y >= getHeight())
    {
        throw std::invalid_argument("ERROR: Specified coordinates out of bounds.");
    }
    
    return grid[y][x];
}
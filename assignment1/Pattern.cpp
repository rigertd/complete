#include <stdexcept>
#include <algorithm>
#include "Pattern.hpp"

Pattern::Pattern(std::string name, std::vector<std::vector<bool> > &sourceGrid)
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
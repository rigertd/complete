#include <algorithm>
#include <iostream>
#include <stdexcept>
#include "Simulation.hpp"
#include "Pattern.hpp"

// Define NULL if not already defined
#ifndef NULL
#define NULL 0
#endif

Simulation::Simulation(int width, int height, int bufferCellCount)
{
    this->bufferCellCount = bufferCellCount;
    this->width = width + (bufferCellCount * 2);
    this->height = height + (bufferCellCount * 2);
    generationCount = 0;
    
    // Initialize grids
    initializeGrid(currentGrid);
    initializeGrid(nextGrid);
}

// Returns whether the specified cell is alive in next generation
bool Simulation::getCellNextState(int x, int y)
{
    // Validate argument values
    if ((x < 1 || x > width - 2) || (y < 1 || y > width - 2))
    {
        throw std::invalid_argument("ERROR: Invalid cell coordinates specified.");
    }
    
    int liveNeighborCount = 0;  // number of live neighbor cells
    bool currentState = false;  // stores state of cell in current generation
    bool nextState = false;        // stores state of cell in next generation
    
    // Check number of live neighbors
    for (int i = y - 1; i <= y + 1; i++)
    {
        for (int j = x - 1; j <= x + 1; j++)
        {
            if (y == i && x == j)         // specified cell
            {
                currentState = currentGrid[i][j];
            }
            else if (currentGrid[i][j]) // neighbor is alive
            {
                liveNeighborCount++;
            }
        }
    }
    
    // Determine next state
    if (currentState)   // currently alive
    {
        // stays alive if 2 or 3 live neighbors
        nextState = liveNeighborCount > 1 && liveNeighborCount < 4;
    }
    else                // currently dead
    {
        // becomes alive if 3 live neighbors
        nextState = liveNeighborCount == 3;
    }
    return nextState;
}

void Simulation::initializeGrid(vgrid &grid)
{
    // Clear any existing data
    grid.clear();
    
    // Initialize rows and set all values to false
    for (int i = 0; i < height; i++)
    {
        std::vector<bool> row;
        for (int j = 0; j < width; j++)
        {
            row.push_back(false);
        }
        grid.push_back(row);
    }
}

// Sets the entire existing grid to false
void Simulation::resetGrid(vgrid &grid)
{
    // Set entire grid to false
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            grid[i][j] = false;
        }
    }
}

// Adds a pattern to the current grid starting from x,y
void Simulation::addPattern(Pattern p, int x, int y)
{
    // Convert visible x and y to actual x and y
    int realX = x + bufferCellCount;
    int realY = y + bufferCellCount;
    int patternX = p.getWidth();
    int patternY = p.getHeight();
    
    // Validate argument values
    if ((realX + patternX > width)
          || (realY + patternY > height))
    {
        throw std::invalid_argument("ERROR: Pattern too large for coordinates.");
    }
    
    // Update currentGrid with specified Pattern
    for (int i = 0; i < patternY; i++)
    {
        for (int j = 0; j < patternX; j++)
        {
            currentGrid[i + realY][j + realX] = p.getCellState(j, i);
        }
    }
}

// Advances the simulation to the next generation
void Simulation::advanceGeneration()
{
    bool nextState = false; // stores the next state
    
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            nextState = getCellNextState(j, i);
            nextGrid[i][j] = nextState;
        }
    }
    
    // Copy nextGrid to currentGrid
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            currentGrid[i][j] = nextGrid[i][j];
        }
    }
    // Set nextGrid to all false
    resetGrid(nextGrid);
    // Increment generation counter
    generationCount++;
}

// Prints the current state of the visible grid to the console
void Simulation::printCurrent()
{
    // Calculate visible range of coordinates
    int startX = bufferCellCount;
    int startY = bufferCellCount;
    int endX = width - bufferCellCount - 1;
    int endY = height - bufferCellCount - 1;
    
    for (int i = startY; i < endY; i++)
    {
        for (int j = startX; j < endX; j++)
        {
            if (currentGrid[i][j])  // alive
            {
                std::cout << '#';
            }
            else                    // dead
            {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Current generation number: " << generationCount << std::endl;
}
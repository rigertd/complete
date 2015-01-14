/*************************************************************************
 * Author:          David Rigert
 * Date Created:    1/10/2015
 * Last Modified:   1/13/2015
 * Assignment:      Assignment 1
 * Filename:        Simulation.hpp
 *
 * Description:     Represents a Game of Life simulator.
 ************************************************************************/
#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>

// Define 2D bool vector as vgrid
typedef std::vector<std::vector<bool> > vgrid;

// forward declaration
class Pattern;

class Simulation
{
// member variables
private:
    vgrid currentGrid;          // grid in current generation
    vgrid nextGrid;             // grid in next generation
    unsigned generationCount;   // number of generations that have passed
    int width;                  // width of visible grid
    int height;                 // height of visible grid
    int bufCellCount;           // hidden buffer cells on all sides of grid

public:
    static const int SLEEP_MS = 50; // Sleep time between frames
    
// member functions    
private:

    /********************************************************************
     *  Function:       bool getCellNextState(int x, int y)
     *  Description:    Gets whether the specified cell will be alive in
     *                  the next generation. Live cells will be alive if
     *                  there are 2-3 live neighbors. Dead cells will
     *                  come to life if there are 3 live neighbors.
     *  Parameters:     x   X coordinate of cell to check
     *                  y   Y coordinate of cell to check
     *  Preconditions:  Grids are initialized
     *                  Specified coords are at least 1 cell away from real edge
     *  Postconditions: Returns true if cell is alive in next generation.
     *******************************************************************/
    bool getCellNextState(int, int);
    
    /********************************************************************
     *  Function:       void initializeGrid(vgrid &grid)
     *  Description:    Creates the specified vgrid based on the width and
     *                  height member variables and sets all values to false.
     *  Parameters:     &grid   Grid to initialize
     *  Preconditions:  width and height are set
     *  Postconditions: Specified grid is initialized as 2D vector with
     *                  height rows, width columns, and all elements set to false.
     *******************************************************************/
    void initializeGrid(vgrid &);

    /********************************************************************
     *  Function:       void resetGrid(vgrid &grid)
     *  Description:    Sets all values of the specified vgrid to false.
     *  Parameters:     &grid   Grid to reset
     *  Preconditions:  none
     *  Postconditions: Specified grid has all elements set to false.
     *******************************************************************/
    void resetGrid(vgrid &);
    
public:

    /********************************************************************
     *  Function:       Simulation(int width, int height, int bufCellCount)
     *  Description:    The constructor takes the width and height of the
     *                  visible grid and creates a vgrid with bufCellCount 
     *                  cells added to all sides.
     *  Parameters:     width           Width of the visible grid
     *                  height          Height of the visible grid
     *                  bufCellCount    Number of padding cells added 
     *                                  to all sides
     *******************************************************************/
    Simulation(int, int, int);
    
    /********************************************************************
     *  Function:       void addPattern(Pattern p, int x, int y)
     *  Description:    Adds a pattern to the current grid with the top-left
     *                  corner at the specified x, y coordinates.
     *  Parameters:     p   Pattern to add
     *                  x   X coordinate of top-left corner of pattern
     *                  y   Y coordinate of top-left corner of pattern
     *  Preconditions:  Current grid is initialized, x,y is on visible grid
     *  Postconditions: Specified pattern is on current grid
     *******************************************************************/
    void addPattern(Pattern, int, int);
    
    /********************************************************************
     *  Function:       void advanceGeneration()
     *  Description:    Makes the simulation transition to the next generation
     *                  and updates all cells according to the rules of
     *                  Conway's Game of Life.
     *  Parameters:     none
     *  Preconditions:  Current grid is initialized
     *  Postconditions: Current grid contains next generation and
     *                  generation count is incremented by 1
     *******************************************************************/
    void advanceGeneration();

    /********************************************************************
     *  Function:       void printCurrent()
     *  Description:    Prints the current generation to the console window.
     *  Parameters:     none
     *  Preconditions:  none
     *  Postconditions: Grid of current generation is displayed in console
     *******************************************************************/
    void printCurrent();
    
    /********************************************************************
     *  Function:       void runInConsole(int generations)
     *  Description:    Runs the simulation for the specified number of
     *                  generations and displays the output in the console.
     *  Parameters:     none
     *  Preconditions:  none
     *  Postconditions: Grid of the last generation is displayed in console
     *******************************************************************/
    void runInConsole(int);
};
#endif // end of define SIMULATION_HPP

#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>

// Define 2D vector as vgrid
typedef std::vector<std::vector<bool> > vgrid;

// forward declaration
class Pattern;

class Simulation
{
// member variables
private:
    vgrid currentGrid;      // grid in current generation
    vgrid nextGrid;         // grid in next generation
    long generationCount;   // number of generations that have passed
    int width;              // width of visible grid
    int height;             // height of visible grid
    int bufferCellCount;    // hidden buffer cells on all sides of grid

// member functions    
private:
    // Gets whether a cell will be alive or dead next generation.
    bool getCellNextState(int, int);
    // Sets the specified vgrid to all false.
    void initializeGrid(vgrid &);
    void resetGrid(vgrid &);
public:
    Simulation(int, int, int);
    // Adds a pattern to the grid.
    void addPattern(Pattern, int, int);
    // Advances the simulation to the next generation.
    void advanceGeneration();
    // Prints the current state of the simulation to the console.
    void printCurrent();
};
#endif // end of define SIMULATION_HPP

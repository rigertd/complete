#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include "Simulation.hpp"
#include "Pattern.hpp"
#include "utility.hpp"

// Function prototypes
// Gets a set of coordinates from the user
void getCoordinates(int &, int &);
// Populates a vector with a set of hard-coded patterns
void loadPatterns(std::vector<Pattern> &);
// Prints the initial setup menu to the console
void printMenu();
// Prints a list of available patterns to the user
void printPatternMenu(std::vector<Pattern> &);
// Runs the simulation
void runSimulation(Simulation &);
// Gets the pattern index of the user selection
int selectPattern(std::vector<Pattern> &);

// Constants
static const int SLEEP_MS = 50;
static const int GRID_WIDTH = 80;
static const int GRID_HEIGHT = 22;
static const int BUFFER_CELLS = 41;

int main()
{
    Simulation sim = Simulation(GRID_WIDTH, GRID_HEIGHT, BUFFER_CELLS);
    int selection = 0;              // for menu selection
    int index;                      // for pattern index
    int x, y;                       // for pattern coordinates
    std::vector<Pattern> patterns;  // for predefined patterns
    
    // Load patterns from file
    loadPatterns(patterns);
    
    // Display welcome message
    std::cout << "\nWelcome to David Rigert's Life Simulator.\n";
    std::cout << "This program is for CS162_400 in Winter 2015.\n";

    // Menu loop
    do
    {
        std::cout << std::endl;
        printMenu();
        std::cout << "Enter your selection: ";
        std::cin >> selection;

        // Clear input buffer
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        
        switch(selection)
        {
            case 1: 
                sim.printCurrent();
                std::cout << "Press Enter to continue...";
                std::cin.ignore(1000, '\n');
                break;
            case 2:
                std::cout << std::endl;
                index = selectPattern(patterns);
                std::cout << std::endl;
                getCoordinates(x, y);
                
                // Validate coordinates
                if (x < 0 || x > GRID_WIDTH || y < 0 || y > GRID_HEIGHT)
                {
                    std::cout << "\nInvalid coordinates.\n\n";
                    continue;
                }
                
                // Add pattern to simulation
                sim.addPattern(patterns[index], x, y);
                break;
            case 3:
                runSimulation(sim);
                // Intentional fall-through
            case 4:
                std::cout << "\nThank you for playing.\n";
                break;
            default:
                std::cout << "\nInvalid selection.\n\n"; 
        }
    } while (selection < 3 || selection > 4);
    
    return 0;
}

void getCoordinates(int &x, int &y)
{
    std::cout << "Enter the starting coordinates of the top-left corner.\n"
              << "X: ";
    std::cin >> x;
    std::cout << "Y: ";
    std::cin >> y;
    
    // clear input buffer
    std::cin.clear();
    std::cin.ignore(1000, '\n');
}

void loadPatterns(std::vector<Pattern> &patterns)
{
    // Patterns taken from http://en.wikipedia.org/wiki/Conway's_Game_of_Life
    std::ifstream dataIn;
    dataIn.open("patterns.txt");
    if (dataIn.fail())
    {
        std::cout << "Warning: pattern.txt file not found.\n";
        return;
    }
    
    std::string name;
    std::string vals;
    std::istringstream iss;
    std::vector<std::vector<bool> > grid;
    int width = 0;
    int height = 0;
    int val = 0;
    
    // Load patterns from file
    while (!dataIn.eof())
    {
        // Load name and first line of values
        std::getline(dataIn, name);
        std::getline(dataIn, vals);

        // Create vector rows and store pattern
        while (vals != "#" && !dataIn.eof())
        {
            iss.clear();
            iss.str(vals);
            std::vector<bool> row;
            while (iss >> val)
            {
                row.push_back(static_cast<bool>(val));
            }
            grid.push_back(row);
            std::getline(dataIn, vals);
        }

        // Store pattern in vector unless name is blank
        if (!name.empty())
        {
            patterns.push_back(Pattern(name, grid));
        }
        grid.clear();
    }
    
    dataIn.close();
}

// Prints the initial setup menu
void printMenu()
{
    std::cout << " 1: Show current configuration\n"
              << " 2: Add a pattern\n"
              << " 3: Run the simulation\n"
              << " 4: Quit the program\n\n";
}

// Prints a list of available patterns
void printPatternMenu(std::vector<Pattern> &patterns)
{
    for (int i = 0; i < patterns.size(); i++)
    {
        std::cout << std::setw(2) << std::right << (i + 1) << ": "
                  << patterns[i].getName() << " (Size: " 
                  << patterns[i].getWidth() << " x " 
                  << patterns[i].getHeight() << " cells)" << std::endl;
    }
    std::cout << std::endl;
}

// Runs the simulation based on the current setup configuration
void runSimulation(Simulation &sim)
{
    int runForGens = 0; // stores user input for number of generations to run
    
    // Clear console
    clearWindow();
    // Print current state
    sim.printCurrent();
    
    std::cout << "Enter the number of generations to run or any letter to quit: ";

    while (std::cin >> runForGens)
    {
        for (int i = 0; i < runForGens; i++)
        {
            sleepMilliseconds(SLEEP_MS);
            sim.advanceGeneration();
            moveCursorToTopLeft();
            sim.printCurrent();
        }
        clearWindow();
        sim.printCurrent();
        std::cout << "Enter another number to advance or any letter to quit: ";
    }
}

int selectPattern(std::vector<Pattern> &patterns)
{
    int selection = 0;
    printPatternMenu(patterns);
    std::cout << "Select a pattern to add: ";
    std::cin >> selection;
    
    // Validate selection
    while (selection > patterns.size() || selection < 1)
    {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Invalid selection. Try again: ";
    }
    
    // Return index value
    return selection - 1;
}

// Define NULL if not already defined
#ifndef NULL
#define NULL 0
#endif

#ifndef PATTERN_HPP
#define PATTERN_HPP

#include <string>
#include <vector>

class Pattern
{
// member variables
private:
    std::vector<std::vector<bool> > grid;   // holds the pattern
    std::string name;                       // name of pattern

// member functions    
public:
    // Default constructor
    Pattern()               { name = ""; }
    Pattern(std::string, std::vector<std::vector<bool> > &);
    int getWidth()          { return grid.size() > 0 ? grid[0].size() : 0; }
    int getHeight()         { return grid.size(); }
    std::string getName()   { return name; }
    // Get whether specified cell is alive
    bool getCellState(int, int);
};

#endif // end of define PATTERN_HPP

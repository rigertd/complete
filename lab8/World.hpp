/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           2/28/2015
 * Last Modification Date: 2/28/2015
 * Course:                 CS162_400
 * Assignment:             Lab 8
 * Filename:               World.hpp
 *
 * Overview:
 *     Represents the overall world or map. The map consists of a series
 *     of areas or rooms. Each room has one or more of 4 possible directions:
 *     east, west, north, or south.
 ************************************************************************/
#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
#include <map>

// enumerates the available commands in each room
enum Command
{
    MOVE_NORTH,     // go north
    MOVE_EAST,      // go east
    MOVE_SOUTH,     // go south
    MOVE_WEST,      // go west
    TOGGLE_LABEL,   // toggle label visibility
    TOGGLE_EDIT,    // toggle edit mode
    EDIT_DESC,      // edit description
    MAKE_NORTH,     // make room to north
    MAKE_EAST,      // make room to east
    MAKE_SOUTH,     // make room to south
    MAKE_WEST,      // make room to west
    DELETE_NORTH,   // delete room to north
    DELETE_EAST,    // delete room to east
    DELETE_SOUTH,   // delete room to south
    DELETE_WEST,    // delete room to west
    SET_START,      // set room as start point
    SET_END,        // set room as end point
    EXIT,           // exit the program
    HELP            // show available commands
};

// enumerates available directions
enum Direction
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

class World
{
private:
    /******************************************************
    *             Private Member Variables                *
    ******************************************************/
    // represents a room or area in the world
    struct Room
    {
        int roomId;                 // holds a unique room ID
        std::string description;    // description of the room
        Room *north;                // pointer to north
        Room *south;                // pointer to south
        Room *east;                 // pointer to east
        Room *west;                 // pointer to west
        
        // constructor
        Room(int id)
        {
            roomId = id;
            description = "A new room";
            north = NULL;
            south = NULL;
            east = NULL;
            west = NULL;
        }
    };
    
    Room *start;        // starting point
    Room *end;          // exit
    Room *current;      // current location
    int nextRoomId;     // holds the next room ID
    bool editMode;      // edit mode flag
    bool labelVis;      // label visibility flag
    
    // maps string input to the corresponding command
    static std::map<std::string, Command> commands;
    
    /******************************************************
    *             Private Member Functions                *
    ******************************************************/
    // loads the world from a file
    void load(std::ifstream &);
    
    // saves the world to a file
    void save(std::ofstream &);
    
    // builds the command map
    void buildCommandMap();
    
    // deletes the room in the specified direction
    // and any orphaned rooms
    void deleteRoom(Direction);
    
    // deletes the entire tree connected to the specified room
    void deleteTree(Room *);
    
    // edits the description of the current room
    void editRoom();
    
    // checks if room has path to starting point
    bool isConnectedToStart(Room *) const;
    
    // creates a new room in the specified direction
    void makeRoom(Direction);
    
    // moves in the specified direction
    void move(Direction);
    
    // displays the help menu
    void printHelp() const;
    
    // displays the current room
    void printRoom() const;
    
    // runs the specified command
    void runCommand(Command);
    
public:
    /******************************************************
    *            Constructors and Destructor              *
    ******************************************************/
    World();
    ~World();

    /******************************************************
    *              Public Member Functions                *
    ******************************************************/
    // parses user input and runs the specified command
    void command(std::string &);
};

#endif  // end of WORLD_HPP definition
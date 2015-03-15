/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           3/13/2015
 * Last Modification Date: 3/14/2015
 * Course:                 CS162_400
 * Assignment:             Final Project
 * Filename:               World.hpp
 *
 * Overview:
 *     Represents the overall game world. Manages all Rooms and Items used
 *     in the world and handles command parsing, loading, and saving.
 ************************************************************************/
#ifndef WORLD_HPP
#define WORLD_HPP

#include <ctime>
#include <string>
#include <map>
#include <iostream>
#include <fstream>

#include "Command.hpp"
#include "Direction.hpp"
#include "Item.hpp"
#include "Player.hpp"
#include "Room.hpp"
#include "Result.hpp"

class World
{
private:
    /******************************************************
    *             Private Member Variables                *
    ******************************************************/
    // represents an exit direction and the room ID it links to
    struct Link
    {
        Direction dir;      // direction of exit
        unsigned rm;        // room ID that exit leads to
    };
    
    Room::RoomMap rooms;    // all rooms in the game world
    Item::ItemMap items;    // all items in the game world
    time_t startTime;       // time that gameplay started
    time_t timeLimit;       // gameplay time limit in seconds
    Player user;            // player info
    bool editMode;          // edit mode flag
    Room *start;            // starting point
    Room *end;              // exit
    
    /******************************************************
    *             Private Member Functions                *
    ******************************************************/
    // adds an exit to the specified direction based on the arg value
    Result addExit(Direction, std::string);
    
    // prompts the user for Item info and adds an item to the current room
    Result addItem();
    
    // Permanently removes all orphaned items and rooms.
    Result cleanUpOrphans();
    
    // Permanently removes the item with the specified ID from the world
    Result deleteItem(unsigned);
    
    // Permanently removes the room with the specified ID from the world
    Result deleteRoom(unsigned);
    
    // finds the ID of the first item that matches the specified name or 0
    unsigned findItemId(std::string);

    // lists all of the items in the game world with IDs
    void listItems();
    
    // lists all of the rooms in the game world with IDs
    void listRooms();
    
    // sets time limit to the specified number of seconds and resets the timer
    void setTimeLimit(time_t);
    
public:
    /******************************************************
    *            Constructors and Destructor              *
    ******************************************************/
    World();
    ~World();

    /******************************************************
    *              Public Member Functions                *
    ******************************************************/
    // loads the game data from the specified fstream
    Result load(std::ifstream &);
    
    // Parses and runs the specified Command object
    Result parse(Command);
    
    // starts the game loop
    void run();
    
    // saves the current game data to the specified fstream
    Result save(std::ofstream &);
};

#endif  // end of WORLD_HPP definition

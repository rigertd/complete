/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           3/13/2015
 * Last Modification Date: 3/14/2015
 * Course:                 CS162_400
 * Assignment:             Final Project
 * Filename:               Room.hpp
 *
 * Overview:
 *     Represents a room or area in the world. 
 ************************************************************************/
#ifndef ROOM_HPP
#define ROOM_HPP

#include <string>
#include <map>

#include "Item.hpp"
#include "Result.hpp"
#include "Direction.hpp"

class Room
{
    friend class UnitTest;      // for unit testing
public:    
    // define type for mapping ID to Room pointer
    typedef std::map<unsigned, Room *> RoomMap;
protected:
    int id;                     // holds a unique room ID
    std::string description;    // description of the room
    Item::ItemMap items;        // list of items in room
    Room *north;                // pointer to north
    Room *south;                // pointer to south
    Room *east;                 // pointer to east
    Room *west;                 // pointer to west
    
    static int nextId;          // ID of next Room to be created
    
public:
    // constructor
    Room();
    // destructor
    virtual ~Room() = 0;
    
    // adds an item to the room
    Result addItem(Item *);
    
    // removes the exit in the specified direction
    virtual Result clearExit(Direction);
    
    // finds an item with the specified id in the room or returns NULL
    Item *findItem(unsigned);
    
    // gets a pointer to the specified direction
    virtual Room *getExit(Direction d) const;
    
    // gets the ItemMap of the room
    Item::ItemMap &getItems()   { return items; }
    
    // gets the room ID
    unsigned getRoomId() const  { return id; }
    
    // removes an item with the specified id from the room
    Result removeItem(unsigned);
    
    // prompts the user to set the room description
    virtual Result setDescription(std::string = "");
    
    // sets the exit in the specified direction to the specified room
    virtual Result setExit(Direction, Room *);
    
    // sets the item required to use the room
    virtual Result setRequired(Item *);
    
    // sets the target room affected by a SwitchRoom
    virtual Result setTarget(Room *);
    
    // toggles the state of a room with multiple states
    virtual Result toggle();
    
    // attempts to use the specified Item in this room
    virtual Result useItem(Item *);
    virtual Result useItem(unsigned);
    
    // displays the current room info in the terminal window
    virtual void view(bool);
};
#endif  // end of ROOM_HPP definition

/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           3/13/2015
 * Last Modification Date: 3/14/2015
 * Course:                 CS162_400
 * Assignment:             Final Project
 * Filename:               Room.hpp
 *
 * Overview:
 *     Implementation for the abstract Room class. 
 ************************************************************************/
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "Room.hpp"
#include "Item.hpp"
#include "World.hpp"

// static member variable for unique ID
int Room::nextId = 1;

// constructors
Room::Room(World *w)
{
    id = nextId++;
    north = NULL;
    south = NULL;
    east = NULL;
    west = NULL;
    global = w;
}

// destructor
Room::~Room()
{
}

// adds an item to the room
Result Room::addItem(Item *itm)
{
    Result res(Result::SUCCESS);
    // test for NULL pointer
    if (!itm)
    {
        res.type = Result::FAILURE;
        res.message = "Item not found.";
        return res;
    }
    // test if Item already exists in Room
    else if (findItem(itm->getId()))
    {
        res.type = Result::FAILURE;
        res.message = "That item is already in the room.";
        return res;
    }
    
    // add item to Room if execution reaches here
    items[itm->getId()] = itm;
    return res;
}

// removes the exit in the specified direction
Result Room::clearExit(Direction d)
{
    Result res(Result::SUCCESS);
    switch (d)
    {
    case NORTH:
        if (north)
        {
            // remove return exit
            north->south = NULL;
            // remove exit
            north = NULL;
            res.message = "North exit removed.";
        }
        else
        {
            res.type = Result::FAILURE;
            res.message = "Exit not found.";
        }
        break;
    case EAST:
        if (east)
        {
            // remove return exit
            east->west = NULL;
            // remove exit
            east = NULL;
            res.message = "East exit removed.";
        }
        else
        {
            res.type = Result::FAILURE;
            res.message = "Exit not found.";
        }
        break;
    case SOUTH:
        if (south)
        {
            // remove return exit
            south->north = NULL;
            // remove exit
            south = NULL;
            res.message = "South exit removed.";
        }
        else
        {
            res.type = Result::FAILURE;
            res.message = "Exit not found.";
        }
        break;
    case WEST:
        if (west)
        {
            // remove return exit
            west->east = NULL;
            // remove exit
            west = NULL;
            res.message = "West exit removed.";
        }
        else
        {
            res.type = Result::FAILURE;
            res.message = "Exit not found.";
        }
        break;
    default:
        res.type = Result::FAILURE;
        res.message = "Invalid direction.";
    }
    return res;
}
    
// clears the item required to use the room
Result Room::clearRequired()
{
    Result res(Result::FAILURE);
    res.message = "Items can only be used in switch and condition rooms.";
    return res;
}

// clears the target room of a SwitchRoom
Result Room::clearTarget()
{
    Result res(Result::FAILURE);
    res.message = "Only switch rooms have a target.";
    return res;
}

// finds an item with the specified id in the room or returns NULL
Item *Room::findItem(unsigned id)
{
    Item *pItem = NULL; // stores return value
    std::map<unsigned, Item *>::iterator it;
    it = items.find(id);
    // assign pointer if item is found
    if (it != items.end())
        pItem = items[id];
    return pItem;
}

// gets a pointer to the specified direction
Room *Room::getExit(Direction d) const
{
    switch (d)
    {
    case NORTH:
        return north;
    case EAST:
        return east;
    case SOUTH:
        return south;
    case WEST:
        return west;
    }
    return NULL;
}

// removes an item with the specified id from the room
Result Room::removeItem(unsigned id)
{
    Result res(Result::SUCCESS);
    std::map<unsigned, Item *>::iterator it;
    it = items.find(id);
    if (it != items.end())
    {
        items.erase(it);
    }
    else
    {
        res.type = Result::FAILURE;
        res.message = "Item not found.";
    }
    return res;
}

// prompts the user to set the room description
Result Room::setDescription(std::string desc)
{
    Result res(Result::SUCCESS);
    std::string input;      // user input buffer
    std::ostringstream oss; // description string builder
    
    // update and exit without prompt if argument was passed
    if (!desc.empty())
    {
        description = desc;
        res.message = "Description updated.";
        return res;
    }
    
    std::cout << "Enter a new description one line at a time.\n"
              << "Press Enter with no input to keep the old one.\n"
              << "When you are done entering the description, press Enter twice.\n";
    
    // get first line of input
    std::getline(std::cin, input);
    
    // keep getting input until blank line entered
    while (!input.empty())
    {
        oss << input << std::endl;
        std::getline(std::cin, input);
    }
    
    // check if anything was entered
    if (oss.str().empty())
    {
        res.type = Result::ABORT;
        res.message = "Keeping old description.";
    }
    else
    {
        description = oss.str();
        res.message = "Description updated.";
    }
    return res;
}

// sets the exit in the specified direction to the specified room
Result Room::setExit(Direction d, Room *rm)
{
    Result res(Result::SUCCESS);
    
    // null pointer check
    if (!rm)
    {
        res.type = Result::FAILURE;
        res.message = "Pointer not an instance of an object.";
        return res;
    }

    switch (d)
    {
    case NORTH:
        if (!north)
        {
            // check if return exit is available
            if (rm->south)
            {
                res.type = Result::FAILURE;
                res.message = "Target room already has a room in the return direction.";
            }
            else
            {
                // set return exit
                rm->south = this;
                // set target exit
                north = rm;
            }
        }
        else
        {
            res.type = Result::FAILURE;
            res.message = "An exit already exists in that direction.";
        }
        break;
    case EAST:
        if (!east)
        {
            // check if return exit is available
            if (rm->west)
            {
                res.type = Result::FAILURE;
                res.message = "Target room already has a room in the return direction.";
            }
            else
            {
                // set return exit
                rm->west = this;
                // set target exit
                east = rm;
            }
        }
        else
        {
            res.type = Result::FAILURE;
            res.message = "An exit already exists in that direction.";
        }
        break;
    case SOUTH:
        if (!south)
        {
            // check if return exit is available
            if (rm->north)
            {
                res.type = Result::FAILURE;
                res.message = "Target room already has a room in the return direction.";
            }
            else
            {
                // set return exit
                rm->north = this;
                // set target exit
                south = rm;
            }
        }
        else
        {
            res.type = Result::FAILURE;
            res.message = "An exit already exists in that direction.";
        }
        break;
    case WEST:
        if (!west)
        {
            // check if return exit is available
            if (rm->east)
            {
                res.type = Result::FAILURE;
                res.message = "Target room already has a room in the return direction.";
            }
            else
            {
                // set return exit
                rm->east = this;
                // set target exit
                west = rm;
            }
        }
        else
        {
            res.type = Result::FAILURE;
            res.message = "An exit already exists in that direction.";
        }
    }
    return res;
}

// sets the item required to use the room
Result Room::setRequired(Item *itm)
{
    Result res(Result::FAILURE);
    res.message = "You cannot use items in this room type.";
    return res;
}

// sets the target room affected by a SwitchRoom
Result Room::setTarget(Room *rm)
{
    Result res(Result::FAILURE);
    res.message = "You cannot set a target room in this room type.";
    return res;
}

// toggles the state of a room with multiple states
Result Room::toggle()
{
    Result res(Result::FAILURE);
    res.message = "This room type only has one state.";
    return res;
}

// enables toggling in the specified direction
Result Room::toggleExit(Direction d)
{
    Result res(Result::FAILURE);
    res.message = "This command only works on a condition room.";
    return res;
}

// attempts to use the specified Item in this room
Result Room::useItem(Item *itm)
{
    Result res(Result::FAILURE);
    res.message = "You cannot use that here.";
    return res;
}

Result Room::useItem(unsigned id)
{
    Result res(Result::FAILURE);
    res.message = "You cannot use that here.";
    return res;
}

// displays the current room info in the terminal window
void Room::view(bool editMode)
{
    // display extra info in edit mode
    if (editMode)
    {
        std::cout << "Room ID: " << id << std::endl
                  << "Exits: "
                  << (north ? "North " : "")
                  << (east ? "East " : "")
                  << (south ? "South " : "")
                  << (west ? "West" : "")
                  << std::endl << std::endl;
    }
    
    // show room description
    std::cout << description;
    
    // show item descriptions
    std::map<unsigned, Item *>::iterator it = items.begin();
    while (it != items.end())
    {
        std::cout << it->second->getDesc() << std::endl;
        it++;
    }
}

// deserializes the room data and configures the Room object
void Room::deserialize(std::istream &in)
{
    std::string input;      // input buffer
    std::ostringstream oss; // description builder
    
    // get room ID
    std::getline(in, input);
    id = std::atoi(input.c_str());
    nextId = (nextId > id ? nextId : id + 1);
    
    // get room description
    std::getline(in, input);  // heading
    std::getline(in, input);  // first line
    while (input != "##ENDROOMDESCRIPTION##")
    {
        oss << input << std::endl;
        std::getline(in, input);
    }
    description = oss.str();
    
    // get room items
    std::getline(in, input);
    std::istringstream iss(input);
    unsigned itemId = 0;
    while (iss >> itemId)
    {
        Item *itm = global->findItem(itemId);
        if (itm)
            items[itemId] = itm;
    }
}

// deserializes the exits of the room
void Room::deserializeExits(std::istream &in)
{
    unsigned val;
    Room *target = NULL;
    
    in >> val;             // north
    target = global->findRoom(val);
    setExit(NORTH, target);
    in >> val;             // east
    target = global->findRoom(val);
    setExit(EAST, target);
    in >> val;             // south
    target = global->findRoom(val);
    setExit(SOUTH, target);
    in >> val;             // west
    target = global->findRoom(val);
    setExit(WEST, target);
}

// serializes the exits of the room
void Room::serializeExits(std::ostream &out)
{
    // output room ID
    out << id << " ";
    
    // output north exit room ID or 0 for NULL
    out << (north ? north->id : 0) << " ";
    
    // output east exit room ID or 0 for NULL
    out << (east ? east->id : 0) << " ";
    
    // output south exit room ID or 0 for NULL
    out << (south ? south->id : 0) << " ";
    
    // output west exit room ID or 0 for NULL
    out << (west ? west->id : 0) << std::endl;
}

// converts Room data to save data
void Room::serialize(std::ostream &out)
{
    // output room type
    out << "basic" << std::endl;
    
    // output room ID
    out << id << std::endl;
    
    // output description
    out << "##ROOMDESCRIPTION##" << std::endl;
    out << description;
    out << "##ENDROOMDESCRIPTION##" << std::endl;
    
    // output item IDs on one line
    std::map<unsigned, Item *>::iterator it = items.begin();
    while (it != items.end())
    {
        out << it->first << ' ';
        ++it;
    }
    out << std::endl;
}

// cannot make this virtual so call a member function
std::ostream &operator<<(std::ostream &out, Room &rm)
{
    rm.serialize(out);
    return out;
}

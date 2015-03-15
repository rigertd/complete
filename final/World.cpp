/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           3/14/2015
 * Last Modification Date: 3/14/2015
 * Course:                 CS162_400
 * Assignment:             Final Project
 * Filename:               World.cpp
 *
 * Overview:
 *     Implementation for World class.
 ************************************************************************/
#include <cstdlib>  // atoi
#include <queue>
#include <set>
#include <typeinfo> // typeid
#include <sstream>  // osringstream

#include "World.hpp"
#include "BasicRoom.hpp"
#include "ConditionRoom.hpp"
#include "SwitchRoom.hpp"

const char* DEFAULT_FILENAME = "game.dat";

/******************************************************
*             Private Member Functions                *
******************************************************/
// adds an exit to the specified direction based on the arg value
Result World::addExit(Direction d, std::string arg)
{
    Result res(Result::SUCCESS);
    Room *rm;
    std::ostringstream oss; // message builder
    
    // make sure exit is available before creating any objects
    if (user.getCurrentRoom()->getExit(d))
    {
        res.type = Result::FAILURE;
        res.message = "A room already exists in that direction.";
        return res;
    }
    
    // create BasicRoom if arg is blank
    if (arg.empty())
    {
        rm = new BasicRoom();
        res = user.getCurrentRoom()->setExit(d, rm);
        res.message = "Created new basic room.";
    }
    else
    {
        // try to convert argument to room ID
        unsigned id = std::atoi(arg.c_str());
        
        // check for room type if not an ID
        if (id == 0)
        {
            // check for ConditionRoom argument
            if (arg == "cond")
            {
                rm = new ConditionRoom();
                res = user.getCurrentRoom()->setExit(d, rm);
                res.message = "Created new condition room.";
            }
            // check for SwitchRoom argument
            else if (arg == "switch")
            {
                rm = new SwitchRoom();
                res = user.getCurrentRoom()->setExit(d, rm);
                res.message = "Created new switch room.";
            }
            else
            {
                res.type = Result::FAILURE;
                oss << "\"" << arg << "\" is not a valid room type or ID\n"
                    << "Use an argument of \"cond\" for a new condition "
                    << "room or \"switch\" for a new switch room.";
                res.message = oss.str();
            }
        }
        else
        {
            // see if id is a valid room ID
            Room::RoomMap::iterator it = rooms.find(id);
            if (it == rooms.end())
            {
                res.type = Result::FAILURE;
                oss << "Cannot find a room with an ID of " << id << ".";
                res.message = oss.str();
            }
            else
            {
                rm = it->second;
                res = user.getCurrentRoom()->setExit(d, rm);
                // only update message if operation was successful
                if (res.type == Result::SUCCESS)
                {
                    oss << "Linked to room ID " << rm->getRoomId() << ".";
                    res.message = oss.str();
                }
            }
        }
        
    }
    // add to master room list if successful
    if (res.type == Result::SUCCESS)
        rooms[rm->getRoomId()] = rm;
    
    return res;
}

// prompts the user for Item info and adds an item to the current room
Result World::addItem()
{
    Result res(Result::SUCCESS);
    
    std::string input;              // user input
    int size;                       // size of item
    int weight;                     // weight of item
    std::queue<std::string> names;  // stores name and aliases
    Item *pItem;                    // stores the new Item object
    
    // get names
    std::cout << "Enter the name of the item, followed by aliases.\n"
              << "Press Enter on a blank line when you are done.\n";
    std::getline(std::cin, input);
    
    // abort if blank input
    if (input.empty())
    {
        res.type = Result::ABORT;
        res.message = "Canceling creation of new item.";
        return res;
    }
    while (!input.empty())
    {
        names.push(input);
        std::getline(std::cin, input);
    }
    
    // get description
    std::cout << "Enter how you want the item to be described in the room.\n"
              << "The item will be invisible if you do not enter any text.\n";
    std::getline(std::cin, input);
    std::cout << std::endl;
    
    // get size
    std::cout << "Enter the size of the item. \nUse a size larger than "
              << user.getMaxSize() << " if you don't want the player "
              << "to pick it up.\n";
    std::cin >> size;
    while (std::cin.get() != '\n' || size < 0)
    {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Invalid size. Enter an integer greater than 0: ";
        std::cin >> size;
    }
    std::cout << std::endl;

    // get weight
    std::cout << "Enter the weight of the item. The player can currently hold "
              << "up to " << user.getMaxWeight() << " pounds.\n";
    std::cin >> weight;
    while (std::cin.get() != '\n' || weight < 0)
    {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Invalid weight. Enter an integer greater than 0: ";
        std::cin >> weight;
    }
    std::cout << std::endl;
    
    // create new Item object
    pItem = new Item(names.front(), static_cast<unsigned>(size), 
                     static_cast<unsigned>(weight));
    names.pop();
    // add any aliases
    while (names.size() > 0)
    {
        pItem->addName(names.front());
        names.pop();
    }
    
    // add description
    pItem->setDesc(input);
    
    // add to master items list
    items[pItem->getId()] = pItem;
    
    // add to current room
    res = user.getCurrentRoom()->addItem(pItem);
    res.message = "Created a new '" + pItem->getName() + "' item.";

    return res;
}

Result World::cleanUpOrphans()
{
    Result res(Result::SUCCESS);
    std::ostringstream oss;         // message builder
    std::set<Item *> itemLinks;     // stores all pointers to all items
    std::set<Room *> roomLinks;     // stores all pointers to all rooms
    
    // go through all rooms and save all pointers to rooms
    Room::RoomMap::iterator roomIt = rooms.begin();
    while (roomIt != rooms.end())
    {
        // save all room links
        roomLinks.insert(roomIt->second->getExit(NORTH));
        roomLinks.insert(roomIt->second->getExit(EAST));
        roomLinks.insert(roomIt->second->getExit(SOUTH));
        roomLinks.insert(roomIt->second->getExit(WEST));
        ++roomIt;
    }
    
    // add player location and start/end points
    roomLinks.insert(user.getCurrentRoom());
    roomLinks.insert(start);
    roomLinks.insert(endpoint);
    
    
    // now go through unorphaned rooms and save all links to items
    Item::ItemMap::iterator itemIt;
    std::set<Room *>::iterator keepRoomIt = roomLinks.begin();
    while (keepRoomIt != roomLinks.end())
    {
        // save all item links
        Room *pRoom = *keepRoomIt;
        // test for NULL pointer
        if (pRoom) 
        {
            itemIt = pRoom->getItems().begin();
            while (itemIt != pRoom->getItems().end())
            {
                itemLinks.insert(itemIt->second);
                ++itemIt;
            }
        }
        ++keepRoomIt;
    }
    
    // add player inventory
    itemIt = user.getItems().begin();
    while (itemIt != user.getItems().end())
    {
        itemLinks.insert(itemIt->second);
        itemIt++;
    }
    
    // remove any unlinked items from master list
    itemIt = items.begin();
    std::cout << "Removing item IDs: ";
    while (itemIt != items.end())
    {
        if (itemLinks.count(itemIt->second) == 0)
        {
            std::cout << itemIt->second->getId() << " ";
            delete itemIt->second;
            items.erase(itemIt++);
        }
        else
        {
            ++itemIt;
        }
    }
    std::cout << std::endl;

    // remove any unlinked rooms from master list
    roomIt = rooms.begin();
    std::cout << "Removing room IDs: ";
    while (roomIt != rooms.end())
    {
        if (roomLinks.count(roomIt->second) == 0)
        {
            std::cout << roomIt->second->getRoomId() << " ";
            delete roomIt->second;
            rooms.erase(roomIt++);
        }
        else
        {
            ++roomIt;
        }
    }
    std::cout << std::endl;
    
    return res;
}

// Permanently removes the item with the specified ID from the world
Result World::deleteItem(unsigned id)
{
    Result res(Result::SUCCESS);
    std::string name;
    std::ostringstream oss;         // message builder
    
    // check if item exists
    Item::ItemMap::iterator itemIt = items.find(id);
    if (itemIt == items.end())
    {
        res.type = Result::FAILURE;
        res.message = "Item not found.";
        return res;
    }
    
    // remove any link to item in player inventory
    user.dropItem(itemIt->second->getId());
    
    // remove any links to item in rooms
    Room::RoomMap::iterator roomIt = rooms.begin();
    while (roomIt != rooms.end())
    {
        roomIt->second->removeItem(itemIt->second->getId());
        roomIt++;
    }
    
    // delete Item object and remove from items
    name = itemIt->second->getName();
    delete itemIt->second;
    items.erase(itemIt);
    
    oss << "Permanently deleted item ID " << id << " (" << name << ").";
    res.message = oss.str();
    return res;
}

// Permanently removes the room with the specified ID from the world
Result World::deleteRoom(unsigned id)
{
    Result res(Result::SUCCESS);
    std::ostringstream oss;         // message builder
    
    // check if room ID exists
    Room::RoomMap::iterator it = rooms.find(id);
    if (it == rooms.end())
    {
        res.type = Result::FAILURE;
        res.message = "Room not found.";
        return res;
    }
    
    // remove any exits to room from other rooms
    Room::RoomMap::iterator it2 = rooms.begin();
    while (it2 != rooms.end())
    {
        // we could just remove exits from the room being deleted,
        // but I'll do it this way just in case I add one-way links
        if (it2->second->getExit(NORTH) == it->second)
            it2->second->clearExit(NORTH);
        if (it2->second->getExit(EAST) == it->second)
            it2->second->clearExit(EAST);
        if (it2->second->getExit(SOUTH) == it->second)
            it2->second->clearExit(SOUTH);
        if (it2->second->getExit(WEST) == it->second)
            it2->second->clearExit(WEST);
        it2++;
    }
    
    // delete Room object and remove from rooms
    delete it->second;
    rooms.erase(it);
    oss << "Permanently deleted room ID " << id << ".";
    res.message = oss.str();
    
    return res;
}

// finds the ID of the first item that matches the specified name or 0
unsigned World::findItemId(std::string name)
{
    Item::ItemMap::iterator it = items.begin();

    // check every Item in items
    while (it != items.end())
    {
        if (it->second->hasName(name))
            return it->second->getId();
        it++;
    }
    
    return 0;
}

// lists all of the items in the game world with IDs
void World::listItems()
{
    Item::ItemMap::iterator it = items.begin();

    if (items.size() > 0)
        std::cout << "The world currently contains the following items:" << std::endl;
    else
        std::cout << "The world does not have any items yet." << std::endl;
    
    // list all items and the ID number
    while (it != items.end())
    {
        std::cout << "  " << it->second->getName() 
                  << " (ID: " << it->second->getId() << ", " 
                  << "weight: " << it->second->getWeight() << ")" << std::endl;
        it++;
    }
}

// lists all of the rooms in the game world with IDs
void World::listRooms()
{
    Room::RoomMap::iterator it = rooms.begin();

    if (rooms.size() > 0)
        std::cout << "The world currently contains the following rooms:" << std::endl;
    else
        std::cout << "The world does not have any rooms yet." << std::endl;
    
    // list the ID and type of all rooms
    while (it != rooms.end())
    {
        std::cout << "  " << it->second->getRoomId() << " (";
        if (typeid(*it->second) == typeid(BasicRoom))
            std::cout << "basic)" << std::endl;
        else if (typeid(*it->second) == typeid(ConditionRoom))
            std::cout << "condition)" << std::endl;
        else if (typeid(*it->second) == typeid(SwitchRoom))
            std::cout << "switch)" << std::endl;
        it++;
    }
}

// sets time limit to the specified number of seconds and resets the timer
void World::setTimeLimit(time_t seconds)
{
    // Reset startTime to now
    startTime = std::time(0);
    
    timeLimit = seconds;
}

/******************************************************
*            Constructors and Destructor              *
******************************************************/
World::World()
{
    startTime = std::time(0);   // set start time to now
    timeLimit = 600;            // default time limit of 10 minutes
    editMode = false;           // edit mode off by default
    start = NULL;
    endpoint = NULL;
}

World::~World()
{
    // delete all rooms
    Room::RoomMap::iterator roomIt = rooms.begin();
    while (roomIt != rooms.end())
    {
        delete roomIt->second;
        roomIt->second = NULL;
        roomIt++;
    }
    rooms.clear();

    // delete all items
    Item::ItemMap::iterator itemIt = items.begin();
    while (itemIt != items.end())
    {
        delete itemIt->second;
        itemIt->second = NULL;
        itemIt++;
    }
    items.clear();
}

/******************************************************
*              Public Member Functions                *
******************************************************/
// loads the game data from the specified fstream
Result World::load(std::ifstream &in)
{
    Result res(Result::SUCCESS);
    
    return res;
}

// Parses and runs the specified Command object
Result World::parse(Command cmd)
{
    Result res(Result::SUCCESS);
    std::string input;          // user input buffer
    int value = -1;             // for converting arg to number
    std::ostringstream oss;     // message builder
    std::ofstream ofs;          // save file stream
    
    switch(cmd.getType())
    {
    case Command::COMMAND_HELP:        // display available commands
        cmd.printCommands(editMode);
        std::cout << "Press Enter to continue...";
        std::getline(std::cin, input);
        break;
    case Command::ITEM_CREATE:         // create new item
        res = addItem();
        break;
    case Command::ITEM_DROP:           // remove item from inventory
        // check for argument
        if (cmd.getArgument().empty())
        {
            res.type = Result::FAILURE;
            res.message = "This command requires an item ID or name as an argument.";
        }
        else
        {
            // check if item ID was entered
            value = std::atoi(cmd.getArgument().c_str());
            // otherwise check if item name was entered
            if (value <= 0)
                value = findItemId(cmd.getArgument());
            res = user.dropItem(value);
        }
        break;
    case Command::ITEM_TAKE:           // put room item in inventory
        if (cmd.getArgument().empty())
        {
            res.type = Result::FAILURE;
            res.message = "This command requires an item ID or name as an argument.";
        }
        else
        {
            // check if item ID was entered
            value = std::atoi(cmd.getArgument().c_str());
            // otherwise check if item name was entered
            if (value <= 0)
                value = findItemId(cmd.getArgument());
            res = user.take(value);
        }
        break;
    case Command::ITEM_USE:            // use item on environment
        if (cmd.getArgument().empty())
        {
            res.type = Result::FAILURE;
            res.message = "This command requires an item ID or name as an argument.";
        }
        else
        {
            // check if item ID was entered
            value = std::atoi(cmd.getArgument().c_str());
            // otherwise check if item name was entered
            if (value <= 0)
                value = findItemId(cmd.getArgument());
            res = user.useItem(value);
        }
        break;
    case Command::PLAYER_INVENTORY:    // view player inventory
        user.viewItems();
        break;
    case Command::PLAYER_MAX_QUANTITY: // set player maxQuantity
        if (cmd.getArgument().empty())
        {
            res.type = Result::FAILURE;
            res.message = "Specify the number of items that can be held in inventory.";
        }
        else
        {
            // check if a number was entered
            value = std::atoi(cmd.getArgument().c_str());
            if (value > 0)
            {
                user.setMaxQuantity(value);
                res.message = "Maximum quantity updated.";
            }
            else
            {
                res.type = Result::FAILURE;
                res.message = "You must enter a positive quantity greater than 0.";
            }
        }
        break;
    case Command::PLAYER_MAX_SIZE:     // set player maxSize
        if (cmd.getArgument().empty())
        {
            res.type = Result::FAILURE;
            res.message = "Specify the largest size of item that can be held in inventory.";
        }
        else
        {
            // check if a number was entered
            value = std::atoi(cmd.getArgument().c_str());
            if (value > 0)
            {
                user.setMaxSize(value);
                res.message = "Maximum size updated.";
            }
            else
            {
                res.type = Result::FAILURE;
                res.message = "You must enter a positive size greater than 0.";
            }
        }
        break;
    case Command::PLAYER_MAX_WEIGHT:   // set player maxWeight
        if (cmd.getArgument().empty())
        {
            res.type = Result::FAILURE;
            res.message = "Specify the maximum total weight that can be held in inventory.";
        }
        else
        {
            // check if a number was entered
            value = std::atoi(cmd.getArgument().c_str());
            if (value > 0)
            {
                user.setWeightLimit(value);
                res.message = "Maximum weight updated.";
            }
            else
            {
                res.type = Result::FAILURE;
                res.message = "You must enter a positive quantity greater than 0.";
            }
        }
        break;
    case Command::ROOM_DELETE_NORTH:   // delete exit to north
        res = user.getCurrentRoom()->clearExit(NORTH);
        break;
    case Command::ROOM_DELETE_EAST:    // delete exit to east
        res = user.getCurrentRoom()->clearExit(EAST);
        break;
    case Command::ROOM_DELETE_SOUTH:   // delete exit to south
        res = user.getCurrentRoom()->clearExit(SOUTH);
        break;
    case Command::ROOM_DELETE_WEST:    // delete exit to west
        res = user.getCurrentRoom()->clearExit(WEST);
        break;
    case Command::ROOM_EDIT_DESC:      // edit room description
        res = user.getCurrentRoom()->setDescription(cmd.getArgument());
        break;
    case Command::ROOM_MAKE_NORTH:     // make exit to north
        res = addExit(NORTH, cmd.getArgument());
        break;
    case Command::ROOM_MAKE_EAST:      // make exit to east
        res = addExit(EAST, cmd.getArgument());
        break;
    case Command::ROOM_MAKE_SOUTH:     // make exit to south
        res = addExit(SOUTH, cmd.getArgument());
        break;
    case Command::ROOM_MAKE_WEST:      // make exit to west
        res = addExit(WEST, cmd.getArgument());
        break;
    case Command::ROOM_MOVE_NORTH:     // go north from current location
        res = user.move(NORTH);
        break;
    case Command::ROOM_MOVE_EAST:      // go east from current location
        res = user.move(EAST);
        break;
    case Command::ROOM_MOVE_SOUTH:     // go south from current location
        res = user.move(SOUTH);
        break;
    case Command::ROOM_MOVE_WEST:      // go west from current location
        res = user.move(WEST);
        break;
    case Command::ROOM_SET_REQUIRED:   // set item required to toggle room
        if (cmd.getArgument().empty())
        {
            res.type = Result::FAILURE;
            res.message = "This command requires an item ID or name as an argument.";
        }
        else
        {
            // check if item ID was entered
            value = std::atoi(cmd.getArgument().c_str());
            // otherwise check if item name was entered
            if (value <= 0)
                value = findItemId(cmd.getArgument());
            // make sure ID is valid
            Item::ItemMap::iterator it = items.find(value);
            if (it != items.end())
            {
                res = user.getCurrentRoom()->setRequired(it->second);
            }
            else
            {
                res.type = Result::FAILURE;
                res.message = "That item does not exist.";
            }
        }
        break;
    case Command::ROOM_SET_TARGET:     // set target of SwitchRoom
        value = std::atoi(cmd.getArgument().c_str());
        if (value <= 0)
        {
            res.type = Result::FAILURE;
            res.message = "This command requires a room ID as an argument.";
        }
        else
        {
            // make sure ID is valid
            Room::RoomMap::iterator it = rooms.find(value);
            if (it != rooms.end())
            {
                res = user.getCurrentRoom()->setTarget(it->second);
            }
            else
            {
                res.type = Result::FAILURE;
                res.message = "That room does not exist.";
            }
        }
        break;
    case Command::ROOM_TOGGLE:         // toggles room state
        res = user.getCurrentRoom()->toggle();
        break;
    case Command::WORLD_CLEAN:         // delete unused rooms and items
        res = cleanUpOrphans();
        break;
    case Command::WORLD_DELETE_ITEM:   // delete existing item
        value = std::atoi(cmd.getArgument().c_str());
        if (value <= 0)
        {
            res.type = Result::FAILURE;
            res.message = "This command requires an item ID as an argument.";
        }
        else
        {
            res = deleteItem(value);
        }
        break;
    case Command::WORLD_DELETE_ROOM:   // delete existing room
        value = std::atoi(cmd.getArgument().c_str());
        if (value <= 0)
        {
            res.type = Result::FAILURE;
            res.message = "This command requires a room ID as an argument.";
        }
        else
        {
            res = deleteRoom(value);
        }
        break;
    case Command::WORLD_EDIT:          // toggle edit mode
        editMode = !editMode;
        oss << "Edit mode " << (editMode ? "enabled." : "disabled.");
        res.message = oss.str();
        break;
    case Command::WORLD_ITEM_LIST:     // list all items in world
        listItems();
        std::cout << "Press Enter to continue...";
        std::getline(std::cin, input);
        break;
    case Command::WORLD_ROOM_LIST:     // list all rooms in world
        listRooms();
        std::cout << "Press Enter to continue...";
        std::getline(std::cin, input);
        break;
    case Command::WORLD_SAVE:          // save the world to file
        // use default filename if none specified
        if (cmd.getArgument().empty())
            input = DEFAULT_FILENAME;
        ofs.open(input.c_str());
        res = save(ofs);
        ofs.close();
        break;
    case Command::WORLD_SET_END:       // set end point
        endpoint = user.getCurrentRoom();
        res.message = "Updated goal to current room.";
        break;
    case Command::WORLD_SET_START:     // set start point
        start = user.getCurrentRoom();
        res.message = "Updated starting point to current room.";
        break;
    case Command::WORLD_SET_TIME:      // set time limit
        value = std::atoi(cmd.getArgument().c_str());
        if (value <= 0)
        {
            res.type = Result::FAILURE;
            res.message = "You must enter the time limit in seconds as an argument.";
        }
        else
        {
            timeLimit = value;
        }
        break;
    case Command::WORLD_EXIT:          // exit the game
        res.type = Result::EXIT;
        res.message = "Thank you for playing!";
        break;
    case Command::INVALID_COMMAND:     // invalid command
        if (res.message.empty())
            res.message = "That command is invalid. Try a different one.";
    }
    
    return res;
}

// starts the game loop
void World::run()
{
    Result res;
    Command cmd;
    std::string input;
    
    // create initial room if none exist
    if (rooms.size() == 0)
    {
        start = new BasicRoom();
        rooms[start->getRoomId()] = start;
    }
    
    // set start point if NULL
    if (!start)
    {
        start = rooms.begin()->second;
    }
    
    // initialize Player object
    user = Player(start);
    
    // game loop
    do
    {
        // display time left starting at 3 minutes.
        time_t timeLeft = timeLimit + startTime - time(0);
        time_t minsLeft = timeLeft / 60;
        if (minsLeft <= 3)
            std::cout << "You only have " << minsLeft
                      << " minutes left to reach the goal!\n\n";
        
        // display current room
        user.getCurrentRoom()->view(editMode);
        std::cout << std::endl;
        
        // get user input
        std::cout << "What do you want to do? ";
        std::getline(std::cin, input);
        
        // convert input to Command object
        cmd.read(input, editMode);
        
        // execute Command
        res = parse(cmd);
        
        // display command message
        std::cout << std::endl << res.message;
        if (!res.message.empty())
        {
            std::cout << std::endl << std::endl;
        }
    } while (res.type != Result::EXIT);
}

// saves the current game data to the specified fstream
Result World::save(std::ofstream &out)
{
    Result res(Result::SUCCESS);
    
    return res;
}

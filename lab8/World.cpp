/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           2/28/2015
 * Last Modification Date: 2/28/2015
 * Course:                 CS162_400
 * Assignment:             Lab 8
 * Filename:               World.cpp
 *
 * Overview:
 *     Implementation for World class.
 ************************************************************************/
#include <iostream>

#include "utility.hpp"
#include "World.hpp"

/******************************************************
*             Private Member Functions                *
******************************************************/
// populates the commands map
void World::buildCommandMap()
{
    commands["north"] = MOVE_NORTH;
    commands["n"] = MOVE_NORTH;
    commands["east"] = MOVE_EAST;
    commands["e"] = MOVE_EAST;
    commands["south"] = MOVE_SOUTH;
    commands["s"] = MOVE_SOUTH;
    commands["west"] = MOVE_WEST;
    commands["w"] = MOVE_WEST;
    commands["label"] = TOGGLE_LABEL;
    commands["l"] = TOGGLE_LABEL;
    commands["edit"] = TOGGLE_EDIT;
    commands["description"] = EDIT_DESC;
    commands["desc"] = EDIT_DESC;
    commands["makenorth"] = MAKE_NORTH;
    commands["mn"] = MAKE_NORTH;
    commands["makeeast"] = MAKE_EAST;
    commands["me"] = MAKE_EAST;
    commands["makesouth"] = MAKE_SOUTH;
    commands["ms"] = MAKE_SOUTH;
    commands["makewest"] = MAKE_WEST;
    commands["mw"] = MAKE_WEST;
    commands["deletenorth"] = DELETE_NORTH;
    commands["dn"] = DELETE_NORTH;
    commands["deleteeast"] = DELETE_EAST;
    commands["de"] = DELETE_EAST;
    commands["deletesouth"] = DELETE_SOUTH;
    commands["ds"] = DELETE_SOUTH;
    commands["deletewest"] = DELETE_WEST;
    commands["dw"] = DELETE_WEST;
    commands["setstart"] = SET_START;
    commands["setend"] = SET_END;
    commands["help"] = HELP;
    commands["h"] = HELP;
    commands["exit"] = EXIT;
    commands["x"] = EXIT;
    commands["quit"] = EXIT;
    commands["q"] = EXIT;
}

// loads the world from a file
Result World::load(std::ifstream &in)
{
    Result result = RESULT_FAILURE;
    return result;
}

// saves the world to a file
Result World::save(std::ofstream &out)
{
    Result result = RESULT_FAILURE;
    return result;
}

// deletes the room in the specified direction
// and any orphaned rooms
Result World::deleteRoom(Direction d)
{
    Result result = RESULT_SUCCESS;
    switch (d)
    {
    case NORTH:
        if (current->north != NULL)
        {
            // remove return pointer
            current->north->south = NULL;
            
            deleteTree(current->north);
            delete current->north;
            current->north = NULL;
        }
        else
        {
            result = RESULT_FAILURE;
        }
        break;
    case EAST:
        if (current->east != NULL)
        {
            // remove return pointer
            current->east->west = NULL;
            
            deleteTree(current->east);
            delete current->east;
            current->east = NULL;
        }
        else
        {
            result = RESULT_FAILURE;
        }
        break;
    case SOUTH:
        if (current->south != NULL)
        {
            // remove return pointer
            current->south->north = NULL;
            
            deleteTree(current->south);
            delete current->south;
            current->south = NULL;
        }
        else
        {
            result = RESULT_FAILURE;
        }
        break;
    case WEST:
        if (current->west != NULL)
        {
            // remove return pointer
            current->west->east = NULL;
            
            deleteTree(current->west);
            delete current->west;
            current->west = NULL;
        }
        else
        {
            result = RESULT_FAILURE;
        }
        break;
    }
    
    if (result == RESULT_FAILURE)
        std::cout << "No room exists there!\n";
    else
    {
        // make current room new starting point if not connected anymore
        if (!isConnectedToStart(current, d))
        {
            std::cout << "Making the current room the new starting point.\n";
            start = current;
        }
    }
    return result;
}

// recursively deletes the entire tree connected to the specified room
World::Room *World::deleteTree(World::Room *rm)
{
    if (rm->north != NULL)
    {
        // remove return pointer
        rm->north->south = NULL;
        delete deleteTree(rm->north);
    }
    if (rm->east != NULL)
    {
        // remove return pointer
        rm->east->west = NULL;
        delete deleteTree(rm->east);
    }
    if (rm->south != NULL)
    {
        // remove return pointer
        rm->south->north = NULL;
        delete deleteTree(rm->south);
    }
    if (rm->west != NULL)
    {
        // remove return pointer
        rm->west->east = NULL;
        delete deleteTree(rm->west);
    }
    return rm;
}

// edits the description of the current room
Result World::editRoom()
{
    std::string input;
    std::cout << "\nEnter a new description or blank to keep the old one.\n: ";
    std::getline(std::cin, input);
    if (!input.empty())
        current->description = input;
    return RESULT_SUCCESS;
}

// checks if room has path to starting point
bool World::isConnectedToStart(Room *rm, Direction source) const
{
    bool result = rm == start;
    
    if (rm->north != NULL && source != NORTH && !result)
        result = isConnectedToStart(rm->north, SOUTH);
    if (rm->east != NULL && source != EAST && !result)
        result = isConnectedToStart(rm->east, WEST);
    if (rm->south != NULL && source != SOUTH && !result)
        result = isConnectedToStart(rm->south, NORTH);
    if (rm->west != NULL && source != WEST && !result)
        result = isConnectedToStart(rm->west, EAST);
    return result;
}

// creates a new room in the specified direction
Result World::makeRoom(Direction d)
{
    Result result = RESULT_SUCCESS;
    switch (d)
    {
    case NORTH:
        if (current->north == NULL)
        {
            current->north = new Room(nextRoomId++);
            current->north->south = current;
        }
        else
        {
            result = RESULT_FAILURE;
        }
        break;
    case EAST:
        if (current->east == NULL)
        {
            current->east = new Room(nextRoomId++);
            current->east->west = current;
        }
        else
        {
            result = RESULT_FAILURE;
        }
        break;
    case SOUTH:
        if (current->south == NULL)
        {
            current->south = new Room(nextRoomId++);
            current->south->north = current;
        }
        else
        {
            result = RESULT_FAILURE;
        }
        break;
    case WEST:
        if (current->west == NULL)
        {
            current->west = new Room(nextRoomId++);
            current->west->east = current;
        }
        else
        {
            result = RESULT_FAILURE;
        }
        break;
    }
    
    if (result == RESULT_FAILURE)
        std::cout << "\nA room already exists in that direction!\n";
    
    return result;
}

// moves in the specified direction
Result World::move(Direction d)
{
    Result result = RESULT_SUCCESS;
    switch (d)
    {
    case NORTH:
        if (current->north != NULL)
            current = current->north;
        else
        {
            result = RESULT_FAILURE;
        }
        break;
    case EAST:
        if (current->east != NULL)
            current = current->east;
        else
        {
            result = RESULT_FAILURE;
        }
        break;
    case SOUTH:
        if (current->south != NULL)
            current = current->south;
        else
        {
            result = RESULT_FAILURE;
        }
        break;
    case WEST:
        if (current->west != NULL)
            current = current->west;
        else
        {
            result = RESULT_FAILURE;
        }
        break;
    }
    
    if (result == RESULT_FAILURE)
        std::cout << "\nYou cannot go that direction.\n";
    return result;
}

// displays the help menu
void World::printHelp() const
{
    std::cout << "\nThe following commands are available.\n";
    
    // display editing commands if in edit mode
    if (editMode)
    {
    std::cout << "  edit              : Toggle edit mode\n"
              << "  description (desc): Edit the room description\n"
              << "  makenorth     (mn): Make a room to the North\n"
              << "  makeeast      (me): Make a room to the East\n"
              << "  makesouth     (ms): Make a room to the South\n"
              << "  makewest      (mw): Make a room to the West\n"
              << "  deletenorth   (dn): Delete the room to the North\n"
              << "  deleteeast    (de): Delete the room to the East\n"
              << "  deletesouth   (ds): Delete the room to the South\n"
              << "  deletewest    (dw): Delete the room to the West\n"
              << "  setstart          : Set the starting point of the world\n"
              << "  setend            : Set the end of the world\n";
              
    }
    std::cout << "  north (n): Move to the North\n"
              << "  east  (e): Move to the East\n"
              << "  south (s): Move to the South\n"
              << "  west  (w): Move to the West\n"
              << "  label (l): Toggle label visibility\n"
              << "  help  (h): Show this guide\n"
              << "  exit  (x): Exit the program\n";
}

// runs the specified command
Result World::runCommand(Command c)
{
    Result result = RESULT_SUCCESS;
    switch (c)
    {
    case MOVE_NORTH:
        move(NORTH);
        break;
    case MOVE_EAST:
        move(EAST);
        break;
    case MOVE_SOUTH:
        move(SOUTH);
        break;
    case MOVE_WEST:
        move(WEST);
        break;
    case TOGGLE_LABEL:
        std::cout << "\nToggling room label\n";
        labelVis = !labelVis;
        break;
    case TOGGLE_EDIT:
        std::cout << "\nToggling edit mode\n";
        editMode = !editMode;
        break;
    case EDIT_DESC:
        if (editMode)
            editRoom();
        else
            result = RESULT_FAILURE;
        break;
    case MAKE_NORTH:
        if (editMode)
            makeRoom(NORTH);
        else
            result = RESULT_FAILURE;
        break;
    case MAKE_EAST:
        if (editMode)
            makeRoom(EAST);
        else
            result = RESULT_FAILURE;
        break;
    case MAKE_SOUTH:
        if (editMode)
            makeRoom(SOUTH);
        else
            result = RESULT_FAILURE;
        break;
    case MAKE_WEST:
        if (editMode)
            makeRoom(WEST);
        else
            result = RESULT_FAILURE;
        break;
    case DELETE_NORTH:
        if (editMode)
            deleteRoom(NORTH);
        else
            result = RESULT_FAILURE;
        break;
    case DELETE_EAST:
        if (editMode)
            deleteRoom(EAST);
        else
            result = RESULT_FAILURE;
        break;
    case DELETE_SOUTH:
        if (editMode)
            deleteRoom(SOUTH);
        else
            result = RESULT_FAILURE;
        break;
    case DELETE_WEST:
        if (editMode)
            deleteRoom(WEST);
        else
            result = RESULT_FAILURE;
        break;
    case SET_START:
        if (editMode)
        {
            std::cout << "Setting starting point to current room.\n";
            start = current;
        }
        else
            result = RESULT_FAILURE;
        break;
    case SET_END:
        if (editMode)
        {
            std::cout << "Setting end point to current room.\n";
            end = current;
        }
        else
            result = RESULT_FAILURE;
        break;
    case HELP:
        printHelp();
        break;
    case EXIT:
        std::cout << "\nAre you sure you want to exit? (y/N): ";
        if (getYesNo())
            result = RESULT_EXIT;
        break;
    }
    if (result == RESULT_FAILURE)
        std::cout << "\nInvalid command. Try again or type 'help' for help.\n";

    return result;
}
    
/******************************************************
*            Constructors and Destructor              *
******************************************************/
World::World()
{
    labelVis = true;
    editMode = false;
    
    // start room ID at 1
    nextRoomId = 1;
    
    // create first room and set to current
    current = new Room(nextRoomId++);
    start = current;
    end = NULL;
    
    // populate commands
    buildCommandMap();
}

World::~World()
{
    // delete all rooms
    start = deleteTree(start);
    delete start;
    start = NULL;
}

/******************************************************
*              Public Member Functions                *
******************************************************/
// parses user input and runs the specified command
Result World::command(std::string &input)
{
    Result result = RESULT_SUCCESS;
    
    // check command map for Command
    std::map<std::string, Command>::iterator iter;
    iter = commands.find(input);
    if (iter != commands.end())
    {
        result = runCommand(iter->second);
    }
    else
    {
        std::cout << "\nInvalid command. Try again or type 'help' for help.\n";
        result = RESULT_FAILURE;
    }
    return result;
}

// displays the current room
void World::printRoom() const
{
    // display room label only if labels are enabled
    if (labelVis)
        std::cout << "\nRoom " << current->roomId << "\n";
    // display description only if not empty
    if (!current->description.empty())
        std::cout << std::endl << current->description << "\n";
    // display if this is starting point
    if (current == start)
        std::cout << "\nThis is where you started your journey.\n";
    // display if this is the end point
    if (current == end)
        std::cout << "\nCongratulations, you have reached your destination.\n"
                  << "Type 'x' to exit or you may continue to explore.\n";
    // always display available directions
    std::cout << "\nYou can go the following directions: ";
    std::cout << (current->north != NULL ? "North " : "")
              << (current->east != NULL ? "East " : "")
              << (current->south != NULL ? "South " : "")
              << (current->west != NULL ? "West" : "") << "\n\n";
}

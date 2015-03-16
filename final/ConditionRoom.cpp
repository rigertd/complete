/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           3/14/2015
 * Last Modification Date: 3/14/2015
 * Course:                 CS162_400
 * Assignment:             Final Project
 * Filename:               ConditionRoom.hpp
 *
 * Overview:
 *     Implementation for ConditionRoom class.
 ************************************************************************/
#include "ConditionRoom.hpp"

#include <iostream>
#include <sstream>
#include <cstdlib>

#include "Item.hpp"
#include "World.hpp"

// constructor
ConditionRoom::ConditionRoom(World *w) : Room(w)
{
    description = "A new conditional room that can disable certain exits.\n";
    altDescription = "The description when toggled exits are disabled.\n";
    northEnabled = false;
    eastEnabled = false;
    southEnabled = false;
    westEnabled = false;
    enabled = true;
    required = NULL;
}

// clears the item required to use the room
Result ConditionRoom::clearRequired()
{
    Result res(Result::SUCCESS);
    
    required = NULL;
    res.message = "Cleared the item required to operate this room.";
    return res;
}

// gets a pointer to the specified direction
Room *ConditionRoom::getExit(Direction d) const
{
    // return NULL for enabled exits if room is in disabled state
    if (!enabled)
    {
        switch (d)
        {
        case NORTH:
            return northEnabled ? NULL : north;
        case EAST:
            return eastEnabled ? NULL : east;
        case SOUTH:
            return southEnabled ? NULL : south;
        case WEST:
            return westEnabled ? NULL : west;
        }
        return NULL;
    }
    else
        return Room::getExit(d);
}

// serializes the room data into the save file format
void ConditionRoom::serialize(std::ostream &out)
{
    // output room type
    out << "condition" << std::endl;
    
    // output room ID
    out << id << std::endl;
    
    // output required item ID or 0 for NULL
    out << (required == NULL ? 0 : required->getId()) << std::endl;
    
    // output enabled state
    out << (enabled ? 1 : 0) << std::endl;
    
    // output exit states
    out << (northEnabled ? 1 : 0) << std::endl;
    out << (eastEnabled ? 1 : 0) << std::endl;
    out << (southEnabled ? 1 : 0) << std::endl;
    out << (westEnabled ? 1 : 0) << std::endl;
    
    // output description
    out << "##ROOMDESCRIPTION##" << std::endl;
    out << description;
    out << "##ENDROOMDESCRIPTION##" << std::endl;
    
    // output alternate description
    out << "##ROOMALTDESCRIPTION##" << std::endl;
    out << altDescription;
    out << "##ENDROOMALTDESCRIPTION##" << std::endl;
    
    // output item IDs on one line
    std::map<unsigned, Item *>::iterator it = items.begin();
    while (it != items.end())
    {
        out << it->first << ' ';
        ++it;
    }
    out << std::endl;
}

// deserializes the room data and configures the Room object
void ConditionRoom::deserialize(std::istream &in)
{
    std::string input;      // input buffer
    std::ostringstream oss; // description builder
    
    // get room ID
    std::getline(in, input);
    id = std::atoi(input.c_str());
    nextId = (nextId > id ? nextId : id + 1);
    
    // get required item ID or 0 for NULL
    std::getline(in, input);
    unsigned val = std::atoi(input.c_str());
    required = global->findItem(val);
    
    // get enabled state
    std::getline(in, input);
    val = std::atoi(input.c_str());
    enabled = val;
    
    // get exit states
    std::getline(in, input);
    val = std::atoi(input.c_str());
    northEnabled = val;
    std::getline(in, input);
    val = std::atoi(input.c_str());
    eastEnabled = val;
    std::getline(in, input);
    val = std::atoi(input.c_str());
    southEnabled = val;
    std::getline(in, input);
    val = std::atoi(input.c_str());
    westEnabled = val;
    
    // get room description
    std::getline(in, input);  // heading
    std::getline(in, input);  // first line
    while (input != "##ENDROOMDESCRIPTION##")
    {
        oss << input << std::endl;
        std::getline(in, input);
    }
    description = oss.str();
    oss.str("");
    oss.clear();
    
    // get alternate room description
    std::getline(in, input);  // heading
    std::getline(in, input);  // first line
    while (input != "##ENDROOMALTDESCRIPTION##")
    {
        oss << input << std::endl;
        std::getline(in, input);
    }
    altDescription = oss.str();
    
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

// prompts the user to set the room description
Result ConditionRoom::setDescription(std::string desc)
{
    Result res(Result::SUCCESS);
    std::string input;      // user input buffer
    std::ostringstream oss; // description string builder
    
    // update and exit without prompt if argument was passed
    if (!desc.empty())
    {
        if (enabled)
            description = desc;
        else
            altDescription = desc;
        res.message = "Description updated.";
        return res;
    }
    
    if (enabled)
        std::cout << "Enter a new enabled state description one line at a time.\n";
    else
        std::cout << "Enter a new disabled state description one line at a time.\n";
    std::cout << "Press Enter with no input to keep the old one.\n"
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
        if (enabled)
        {
            description = oss.str();
            res.message = "Description updated.";
        }
        else
        {
            altDescription = oss.str();
            res.message = "Alternate description updated.";
        }
    }
    return res;
}

// sets the item required to use the room
Result ConditionRoom::setRequired(Item *itm)
{
    Result res(Result::SUCCESS);
    required = itm;
    
    std::ostringstream oss; // message builder
    
    oss << "Required item set to ";
    if (itm)
        oss << itm->getName() << " (" << itm->getId() << ").";
    else
        oss << "NULL.";
    res.message = oss.str();
    
    return res;
}

// toggles the state of a room with multiple states
Result ConditionRoom::toggle()
{
    Result res(Result::SUCCESS);
    
    enabled = !enabled;
    if (enabled)
        res.message = "Enabled all exits.";
    else
        res.message = "Disabled exits that can be toggled.";
    
    return res;
}

// enables toggling in the specified direction
Result ConditionRoom::toggleExit(Direction d)
{
    Result res(Result::SUCCESS);
    switch (d)
    {
    case NORTH:
        northEnabled = !northEnabled;
        if (northEnabled)
            res.message = "Enabled toggling of north exit.";
        else
            res.message = "Disabled toggling of north exit.";
        break;
    case EAST:
        eastEnabled = !eastEnabled;
        if (eastEnabled)
            res.message = "Enabled toggling of east exit.";
        else
            res.message = "Disabled toggling of east exit.";
        break;
    case SOUTH:
        southEnabled = !southEnabled;
        if (southEnabled)
            res.message = "Enabled toggling of south exit.";
        else
            res.message = "Disabled toggling of south exit.";
        break;
    case WEST:
        westEnabled = !westEnabled;
        if (westEnabled)
            res.message = "Enabled toggling of west exit.";
        else
            res.message = "Disabled toggling of west exit.";
        break;
    default:
        res.type = Result::FAILURE;
        res.message = "Invalid direction.";
    }
    return res;
}

// attempts to use the specified Item in this room
Result ConditionRoom::useItem(Item *itm)
{
    Result res(Result::FAILURE);
    if (itm == required)
    {
        res.message = "You hear a rumbling noise.";
        res.type = Result::SUCCESS;
        toggle();
    }
    else
        res.message = "You cannot use that here.";
    
    return res;
}
Result ConditionRoom::useItem(unsigned id)
{
    Result res(Result::FAILURE);
    
    // get pointer to item
    std::map<unsigned, Item *>::iterator it = items.find(id);
    if (it == items.end())
    {
        res.message = "You cannot find that item anywhere.";
        return res;
    }
    
    return useItem(it->second);
}

// displays the current room info in the terminal window
void ConditionRoom::view(bool editMode)
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
                  << std::endl
                  << "Toggled Exits Are "
                  << (enabled ? "Accessible" : "Blocked")
                  << std::endl
                  << "Toggled Exits: "
                  << (northEnabled ? "North " : "")
                  << (eastEnabled ? "East " : "")
                  << (southEnabled ? "South " : "")
                  << (westEnabled ? "West" : "")
                  << std::endl
                  << "Required Item ID: ";
        if (required)
            std::cout << required->getId();
        else
            std::cout << "None";
        std::cout << std::endl << std::endl;
    }
    
    // show room description
    std::cout << (enabled ? description : altDescription);
    
    // show item descriptions
    std::map<unsigned, Item *>::iterator it = items.begin();
    while (it != items.end())
    {
        std::cout << it->second->getDesc() << std::endl;
        it++;
    }
}

/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           3/14/2015
 * Last Modification Date: 3/15/2015
 * Course:                 CS162_400
 * Assignment:             Final Project
 * Filename:               SwitchRoom.hpp
 *
 * Overview:
 *     Implementation for SwitchRoom class.
 ************************************************************************/
#include "SwitchRoom.hpp"

#include <iostream>
#include <sstream>
#include <cstdlib>

#include "Item.hpp"
#include "World.hpp"

// constructor
SwitchRoom::SwitchRoom(World *w) : Room(w)
{
    description = "A room that switches the state of a different room.\n";
    altDescription = "The description when the switch is flipped.\n";
    enabled = false;
    required = NULL;
    target = NULL;
}

// serializes the room data into the save file format
void SwitchRoom::serialize(std::ostream &out)
{
    // output room type
    out << "switch" << std::endl;
    
    // output room ID
    out << id << std::endl;
    
    // output required item ID or 0 for NULL
    out << (required == NULL ? 0 : required->getId()) << std::endl;
    
    // output target room ID or 0 for NULL
    out << (target == NULL ? 0 : target->getRoomId()) << std::endl;
    
    // output enabled state
    out << (enabled ? 1 : 0) << std::endl;
    
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
void SwitchRoom::deserialize(std::istream &in)
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
    
    // get target room ID or 0 for NULL
    std::getline(in, input);
    val = std::atoi(input.c_str());
    target = global->findRoom(val);
    
    // get enabled state
    std::getline(in, input);
    val = std::atoi(input.c_str());
    enabled = val;
    
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
Result SwitchRoom::setDescription(std::string desc)
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
Result SwitchRoom::setRequired(Item *itm)
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

// sets the target room affected by this room
Result SwitchRoom::setTarget(Room *rm)
{
    Result res(Result::SUCCESS);
    
    target = rm;
    res.message = "Target room updated.";
    
    return res;
}

// toggles the state of a room with multiple states
Result SwitchRoom::toggle()
{
    Result res(Result::SUCCESS);
    
    enabled = !enabled;
    if (target)
    {
        target->toggle();
    }
    if (enabled)
        res.message = "Turned the switch on.";
    else
        res.message = "Turned the switch off.";
    
    return res;
}

// attempts to use the specified Item in this room
Result SwitchRoom::useItem(Item *itm)
{
    Result res(Result::FAILURE);
    if (itm == required)
    {
        res.message = "You hear a distant rumbling noise.";
        res.type = Result::SUCCESS;
    }
    else
        res.message = "You cannot use that here.";
    
    return res;
}
Result SwitchRoom::useItem(unsigned id)
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
void SwitchRoom::view(bool editMode)
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
                  << "Switch Is "
                  << (enabled ? "On" : "Off")
                  << std::endl
                  << "Target Room ID: ";
        if (target)
            std::cout << target->getRoomId();
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

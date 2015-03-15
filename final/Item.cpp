/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           3/13/2015
 * Last Modification Date: 3/13/2015
 * Course:                 CS162_400
 * Assignment:             Final Project
 * Filename:               Item.cpp
 *
 * Overview:
 *     Implementation for the Item class.
 ************************************************************************/
#include "Item.hpp"

// starting value for Item ID
unsigned Item::nextId = 1;

// constructor
Item::Item(std::string name, int size, int weight)
{
    id = nextId++;
    this->realName = name;
    this->names.insert(name);
    this->size = size;
    this->weight = weight;
}

// adds a name to the list of acceptable names
void Item::addName(std::string name)
{
    // set realName to first name added if not already set
    if (realName.empty())
        realName = name;
    // add to list of acceptable names
    names.insert(name);
}

// determines whether a string is an acceptable name
bool Item::hasName(std::string name)
{
    std::set<std::string>::iterator it = names.find(name);
    return it != names.end();
}

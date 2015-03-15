/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           3/13/2015
 * Last Modification Date: 3/13/2015
 * Course:                 CS162_400
 * Assignment:             Final Project
 * Filename:               Item.hpp
 *
 * Overview:
 *     Represents an item in the world. 
 ************************************************************************/
#ifndef ITEM_HPP
#define ITEM_HPP

#include <map>
#include <string>
#include <set>
class Item
{
    friend class UnitTest;      // for unit testing
public:
    // define type for mapping ID to Item pointer
    typedef std::map<unsigned, Item *> ItemMap;
    
private:
    unsigned id;                // unique ID of item
    std::string realName;       // actual name of item
    std::set<std::string> names;// names of item
    std::string desc;           // description of item
    int size;                   // size of item
    int weight;                 // weight of item
    
    static unsigned nextId;     // ID of next Item object to be created
    
public:
    Item()
    {
        id = nextId++;
        size = 0;
        weight = 0;
    }
    Item(std::string, int size = 0, int weight = 0);

    void addName(std::string);
    std::string getDesc() const { return desc; }
    unsigned getId() const      { return id; }
    std::string getName() const { return realName; }
    int getSize() const         { return size; }
    int getWeight() const       { return weight; }
    bool hasName(std::string);
    void setDesc(std::string desc)   { this->desc = desc; }
};
#endif  // end of ITEM_HPP definition

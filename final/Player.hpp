/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           3/13/2015
 * Last Modification Date: 3/14/2015
 * Course:                 CS162_400
 * Assignment:             Final Project
 * Filename:               Player.hpp
 *
 * Overview:
 *     Represents the player in the game world. Manages inventory, size
 *     and weight limits, and the current location of the player.
 ************************************************************************/
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Result.hpp"
#include "Direction.hpp"
#include "Item.hpp"
#include "Room.hpp"

class Player
{
    friend class UnitTest;      // for unit testing
private:
    Room *location;             // current location of player
    Item::ItemMap inventory;    // items carried by player
    unsigned weightLimit;       // maximum weight the player can carry
    unsigned maxSize;           // maximum size of item the player can pick up
    unsigned maxQuantity;       // maximum number of items the player can carry
    
public:
    // constructors
    Player()    { location = NULL; }
    Player(Room *, unsigned = 100, unsigned = 10, unsigned = 5);
    
    // drop the item with the specified ID in inventory
    Result dropItem(unsigned);
    
    // get a pointer to the current location of the player
    Room *getCurrentRoom() const    { return location; }
    
    // get the combined weight of all items in inventory
    int getInventoryWeight();
    
    // gets the maximum item size that can be held in inventory
    unsigned getMaxSize() const     { return maxSize; }
    
    // gets the maximum total weight that can be held in inventory
    unsigned getMaxWeight() const   { return weightLimit; }
    
    // move in the specified direction
    Result move(Direction);
    
    // sets the maximum quantity to hold in inventory
    void setMaxQuantity(unsigned val)   { maxQuantity = val; }
    
    // sets the maximum size of item to hold in inventory
    void setMaxSize(unsigned val)       { maxSize = val; }
    
    // sets the maximum total weight of items to hold in inventory
    void setWeightLimit(unsigned val)   { weightLimit = val; }
    
    // pick up the item with the specified ID in the current location
    Result take(unsigned);
    
    // use an item with the specified ID in inventory or in the room
    Result useItem(unsigned);
    
    // view all items in inventory
    void viewItems();
};

#endif  // end of PLAYER_HPP definition
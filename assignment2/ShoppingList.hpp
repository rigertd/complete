#ifndef SHOPPING_LIST_HPP
#define SHOPPING_LIST_HPP

#include <string>
#include <vector>
#include <ostream>

class Item;

class ShoppingList
{
private:
    /********** Private Member Variables **********/
    std::vector<Item> list; // items in the shopping list
    std::string filename;   // current filename (if any).
    bool modified;          // whether list has been modified since last save
    
    /********** Private Member Functions **********/
    // gets total price of all items on list
    double getTotalCost();
    
    // writes each item on separate line, with details delimited by tab char
    void writeTabDelimited(std::ostream &);

public:
    /********** Static Constants **********/
    static const std::string DEFAULT_FILENAME;

    /********** Constructor **********/
    // default constructor
    ShoppingList()
    {
        this->modified = false;
        this->filename = DEFAULT_FILENAME;
    }
    
    /********** Public Member Functions **********/
    // gets whether modified since last save
    bool isModified()   { return modified; }
    
    // prompts user for input and adds item to list
    void addItem();
    
    // prompts user to select and edit an existing item
    void editItem();
    
    // prompts user to select an item to remove from list
    void removeItem();
    
    // displays the items to the terminal window
    void listItems();
    
    // saves list to default or user-specified file
    void saveList();
    
    // loads list from default or user-specified file
    void loadList();
    
    // gets the total price of all items on list formatted as a string
    std::string getFormattedTotal();
    
    // prints header row as single line of text to terminal window
    void printHeaderRow();
};

#endif  // end of SHOPPING_LIST_HPP include guard
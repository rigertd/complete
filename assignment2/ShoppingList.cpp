#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "ShoppingList.hpp"
#include "Item.hpp"
#include "utility.hpp"

/********** Static Constants **********/
const std::string ShoppingList::DEFAULT_FILENAME = "list.txt";

/********** Private Member Functions **********/
// gets total price of all items on list
double ShoppingList::getTotalCost()
{
    double total = 0.0;
    for (int i = 0; i < list.size(); i++)
    {
        total += list[i].getTotal();
    }
    
    return total;
}

// writes each item on separate line, with details delimited by tab char
void ShoppingList::writeTabDelimited(std::ostream &out)
{
    for (int i = 0; i < list.size(); i++)
    {
        out << list[i].convertToTabDelim() << std::endl;
    }
}

/********** Public Member Functions **********/
// prompts user for input and adds item to list
void ShoppingList::addItem()
{
    std::string name;
    std::string unit;
    std::string unitPrice;
    std::string quantity;
    double newUnitPrice;
    double newQuantity;
    
    // prompt user for item name and validate input
    std::cout << "Enter the product name: ";
    std::getline(std::cin, name);
    if (name.empty())
    {
        std::cout << "\nThe item name cannot be blank.\n";
        return;
    }
    
    // prompt user for unit price and validate input
    std::cout << "Enter the unit price: $";
    std::getline(std::cin, unitPrice);
    if (unitPrice.empty() || !isDouble(unitPrice))
    {
        std::cout << "\nInvalid price entered.\n";
        return;
    }
    else
    {
        newUnitPrice = std::strtod(unitPrice.c_str(), NULL);
        if (newUnitPrice < 0)
        {
            std::cout << "\nThe store will not pay you to take its merchandise.\n";
            return;
        }
    }
    
    // prompt user for quantity and validate input
    std::cout << "Enter the quantity to buy: ";
    std::getline(std::cin, quantity);
    if (quantity.empty() || !isDouble(quantity))
    {
        std::cout << "\nInvalid quantity entered.\n";
        return;
    }
    else
    {
        newQuantity = std::strtod(quantity.c_str(), NULL);
        if (newQuantity <= 0)
        {
            std::cout << "\nThe store does not buy merchandise from customers.\n";
            return;
        }
    }
    
    // prompt user for unit type and validate input
    std::cout << "Enter the unit in which the product is sold: ";
    std::getline(std::cin, unit);
    if (unit.empty())
    {
        std::cout << "\nThe unit cannot be blank.\n";
    }
    
    // If execution reaches here, all input was valid.
    double q = strtod(quantity.c_str(), NULL);
    double up = strtod(unitPrice.c_str(), NULL);
    
    // Instantiate new Item object and add to list.
    list.push_back(Item(name, unit, up, q));
    
    // set the modified flag
    this->modified = true;
}

// prompts user to select and edit an existing item
void ShoppingList::editItem()
{
    std::string input;  // input variable
    
    // list items currently in list
    listItems();
    
    // test for empty list
    if (list.empty())
    {
        return;
    }
    
    // prompt user for selection
    std::cout << "\nChoose the item to edit: ";
    std::getline(std::cin, input);
    
    // validate input
    if (input.empty() || !isInteger(input))
    {
        std::cout << "\nInvalid selection.\n";
        return;
    }
    
    // convert to integer
    int selection = std::atoi(input.c_str());
    
    // validate selection
    if (selection < 1 || selection > list.size())
    {
        std::cout << "\nItem not found in list.\n";
        return;
    }
    
    // edit the selected item
    list[selection - 1].edit();
    
    // set the modified flag
    this->modified = true;
}

// prompts user to select an item to remove from list
void ShoppingList::removeItem()
{
    std::string input;  // input variable
    
    // list items currently in list
    listItems();
    
    // test for empty list
    if (list.empty())
    {
        return;
    }
    
    // prompt user for selection
    std::cout << "\nChoose the item to remove: ";
    std::getline(std::cin, input);
    
    // validate input
    if (input.empty() || !isInteger(input))
    {
        std::cout << "\nInvalid selection.\n";
        return;
    }
    
    // convert to integer
    int selection = std::atoi(input.c_str());
    
    // validate selection
    if (selection < 1 || selection > list.size())
    {
        std::cout << "\nItem not found in list.\n";
        return;
    }
    
    // erase the selected item
    list.erase(list.begin() + (selection - 1));
    
    // set the modified flag
    this->modified = true;
}

// displays the items to the terminal window
void ShoppingList::listItems()
{
    // test for empty list
    if (!list.empty())
    {
        printHeaderRow();
        for (int i = 0; i < list.size(); i++)
        {
            std::cout << std::right << std::setw(3) << (i + 1) << ": "
                      << std::left << list[i] << std::endl;
        }
        std::cout << std::endl 
                  << std::setw(Item::NAME_LEN + Item::PRICE_LEN + Item::QUANTITY_LEN + Item::SUBTOTAL_LEN + 8)
                  << std::right << getFormattedTotal() << std::left << std::endl;
    }
    else
    {
        std::cout << "\nYour shopping list is empty.\n";
    }
}

// saves list to default or user-specified file
void ShoppingList::saveList()
{
    std::ofstream ofs;
    std::string buffer;
    
    // prompt user for path to save file
    std::cout << "\nCurrent save path: " << this->filename << "\n"
              << "\nType a new path or Enter to overwrite this one.\n"
              << "Path: ";
    std::getline(std::cin, buffer);
    
    // validate filename and use current if empty
    if (buffer.empty())
    {
        buffer = this->filename;
    }
    
    // attempt to open file
    ofs.open(buffer);
    if (ofs)
    {
        // iterate through list and save one entry per line as tab-delimited values
        std::cout << "\nSaving to " << buffer << "... ";
        for (int i = 0; i < this->list.size(); i++)
        {
            ofs << this->list[i].convertToTabDelim() << std::endl;
        }
        std::cout << "Done!\n";

        // clear modified flag and update filename
        this->modified = false;
        this->filename = buffer;
    }
    else
    {
        std::cout << "\nERROR: Unable to open " << buffer << "for writing.\n";
        return;
    }
    
    // close the filestream
    ofs.close();
}

// loads list from default or user-specified file
void ShoppingList::loadList()
{
    std::ifstream ifs;
    std::string buffer;
    
    // check for unsaved data before loading
    if (modified)
    {
        std::cout << "\nYour current list has been modified.\n"
                  << "Are you sure you want to continue? (y/n)";
        if (!getYesNo())
        {
            return;
        }
    }
    
    // clear current list and modified flag
    this->list.clear();
    this->modified = false;
    
    // prompt user for file path to load
    std::cout << "\nType the path of the file to load, "
              << "or Enter to use the default " 
              << ShoppingList::DEFAULT_FILENAME << ".\n"
              << "Path: ";
    std::getline(std::cin, this->filename);

    // validate filename and use default if empty
    if (this->filename.empty())
    {
        this->filename = ShoppingList::DEFAULT_FILENAME;
    }
    
    // attempt to open file
    ifs.open(this->filename);
    if (ifs)
    {
        // read file one line at a time and call deserialization constructor
        std::cout << "\nLoading " << this->filename << "... ";
        while (std::getline(ifs, buffer))
        {
            this->list.push_back(Item(buffer));
        }
        std::cout << "Done!\n";
    }
    else
    {
        std::cout << "\nERROR: Unable to load " << this->filename << ".\n";
    }
    
    // close the filestream
    ifs.close();
}

// gets the total price of all items on list formatted as a string
std::string ShoppingList::getFormattedTotal()
{
    std::ostringstream oss;
    oss << "Total: $" << std::fixed << std::setprecision(2)
        << getTotalCost();
    
    return oss.str();
}

// prints header row as single line of text to terminal window
void ShoppingList::printHeaderRow()
{
    std::cout << std::left
              << "     " // for the item number
              << std::setw(Item::NAME_LEN + 1) << "Item Name" 
              << std::setw(Item::PRICE_LEN + 1) << "Unit Price"
              << std::setw(Item::QUANTITY_LEN + 1) << "Quantity"
              << std::setw(Item::SUBTOTAL_LEN) << "Subtotal"
              << std::endl;
}

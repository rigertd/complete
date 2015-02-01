#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include "Item.hpp"
#include "utility.hpp"

/********** Static Constants **********/
// for formatting terminal window output columns
const int Item::NAME_LEN = 35;
const int Item::PRICE_LEN = 10;
const int Item::QUANTITY_LEN = 15;
const int Item::SUBTOTAL_LEN = 10;

/********** Private Member Functions **********/
// gets the total cost of the specified quantity as a formatted string
std::string Item::getFormattedTotal()
{
    std::ostringstream oss;
    oss << "$" << std::fixed << std::setprecision(2)
        << (this->quantity * this->unitPrice);
    return oss.str();
}

// gets the unit price as a formatted string
std::string Item::getFormattedPrice()
{
    std::ostringstream oss;
    oss << "$" << std::fixed << std::setprecision(2)
        << (this->unitPrice);
    return oss.str();
}

// gets the quantity and unit as a formatted string
std::string Item::getFormattedQuantity()
{
    std::ostringstream oss;
    oss << this->quantity << " " << this->unit;
    return oss.str();
}

/********** Constructor **********/
// deserialization constructor
Item::Item(std::string &serialized)
{
    // put name in separate string
    size_t pos1 = serialized.find('\t', 0);
    this->name = serialized.substr(0, pos1);
    // tokenize remaining input using a stringstream object
    std::istringstream iss (serialized.substr(pos1));
    iss >> this->unitPrice;
    iss >> this->quantity;
    iss >> this->unit;
    
    // throw exception on invalid serialized data
    if (iss.fail())
        throw std::invalid_argument("ERROR: Invalid Item data.");
}
    
/********** Public Member Functions **********/
// prompts user for new information and updates object
void Item::edit()
{
    std::string name;
    std::string unit;
    std::string unitPrice;
    std::string quantity;
    double newUnitPrice = 0.0;
    double newQuantity = 0.0;
    
    std::cout << "The current name is \"" << getName() << "\".\n"
              << "Type a new name or press Enter to keep it: ";
    std::getline(std::cin, name);
    std::cout << "The current price is " 
              << getFormattedPrice() << ".\n"
              << "Type a new price or press Enter to keep it: $";
    std::getline(std::cin, unitPrice);
    if (!unitPrice.empty() && !isDouble(unitPrice))
    {
        std::cout << "Invalid price entered. Aborting edit.\n";
        return;
    }
    else
    {
        newUnitPrice = std::strtod(unitPrice.c_str(), NULL);
        if (newUnitPrice < 0)
        {
            std::cout << "\nThe store will not pay you to take its goods.\n";
            return;
        }
    }
    std::cout << "The current quantity is "
              << getFormattedQuantity() << ".\n"
              << "Type a new quantity or press Enter to keep it: ";
    std::getline(std::cin, quantity);
    if (!quantity.empty() && !isDouble(quantity))
    {
        std::cout << "Invalid quantity entered. Aborting edit.\n";
        return;
    }
    else
    {
        newQuantity = std::strtod(quantity.c_str(), NULL);
        if (newQuantity <= 0)
        {
            std::cout << "\nThe store does not buy goods from customers.\n";
            return;
        }
    }
    std::cout << "Type a new unit or press Enter to keep it: ";
    std::getline(std::cin, unit);
    
    // if execution reaches here, all input was valid.
    // test for input and set member variables accordingly.
    if (!name.empty())
        this->name = name;
    if (!unit.empty())
        this->unit = unit;
    if (!unitPrice.empty())
        this->unitPrice = newUnitPrice;
    if (!quantity.empty())
        this->quantity = newQuantity;
}

// converts object to tab-delimited string
std::string Item::convertToTabDelim()
{
    std::ostringstream oss;
    oss << name << "\t" 
        << std::setprecision(2) << std::fixed   // price formatting
        << unitPrice << "\t"
        << std::resetiosflags(std::ios::fixed)       // turn off for quantity
        << std::setprecision(16)
        << quantity << "\t"
        << unit;
    return oss.str();
}
    
/********** Friend Functions **********/
// converts item to single line of text for display
// truncates any that exceed column width.
std::ostream &operator<<(std::ostream &out, Item itm)
{
    out << std::left << std::setw(Item::NAME_LEN)
        << itm.name.substr(0, Item::NAME_LEN) << " "
        << std::right << std::setw(Item::PRICE_LEN)
        << itm.getFormattedPrice().substr(0, Item::PRICE_LEN) << " "
        << std::left << std::setw(Item::QUANTITY_LEN)
        << itm.getFormattedQuantity().substr(0, Item::QUANTITY_LEN) << " "
        << std::right << std::setw(Item::SUBTOTAL_LEN)
        << itm.getFormattedTotal().substr(0, Item::SUBTOTAL_LEN) << std::left;
    
    return out;
}

#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <ostream>

class Item
{
private:
    /********** Private Member Variables **********/
    std::string name;       // name of item
    std::string unit;       // unit of sale
    double unitPrice;       // price per unit
    double quantity;        // amount in units

    /********** Private Member Functions **********/
    // gets the total cost of the specified quantity as a formatted string
    std::string getFormattedTotal();
    
    // gets the unit price as a formatted string
    std::string getFormattedPrice();
    
    // gets the quantity as a formatted string
    std::string getFormattedQuantity();

public:
    /********** Static Constants **********/
    static const int NAME_LEN;
    static const int PRICE_LEN;
    static const int QUANTITY_LEN;
    static const int SUBTOTAL_LEN;
    
    
    /********** Constructors **********/
    // default constructor
    Item()
    {
        this->unitPrice = 0.0;
        this->quantity = 0.0;
    }
    
    // deserialization constructor
    Item(std::string &);
    
    // four-argument constructor using initialization list
    Item(std::string name, std::string unit, double unitPrice, double quantity):
        name(name), unit(unit), unitPrice(unitPrice), quantity(quantity)
    {
    }
    
    /********** Public Member Functions **********/
    // converts object to tab-delimited string
    std::string convertToTabDelim();
    
    // gets the name of the item
    std::string getName()
    {
        return this->name;
    }
    
    // gets the total price of the item
    double getTotal()
    {
        return this->quantity * this->unitPrice;
    }
    
    // prompts user for new information and updates object
    void edit();
    
    /********** Friend Functions **********/
    // converts item to single line of text for display
    friend std::ostream &operator<<(std::ostream &out, Item itm);
};

#endif  // end of ITEM_HPP include guard
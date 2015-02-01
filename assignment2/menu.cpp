#include <string>
#include <iostream>

#include "Item.hpp"
#include "ShoppingList.hpp"
#include "utility.hpp"

// prints the menu to the terminal window
void printMenu();

int main ()
{
    ShoppingList slist;
    int selection;
    std::string buffer;
    
    // start of menu loop
    do
    {
        // display the menu
        std::cout << std::endl;
        printMenu();
        std::cout << std::endl;
        
        // prompt the user and get input
        std::cout << "Enter your selection: ";
        std::cin >> selection;
        
        // clear the keyboard buffer
        clearInputBuffer();
        
        // parse selection
        switch (selection)
        {
        case 1: // list current items
            std::cout << "\nYour shopping list currently contains "
                      << "the following items.\n\n";
            slist.listItems();
            std::cout << "\nPress Enter to continue...";
            std::getline(std::cin, buffer); // swallow any input
            break;
        case 2: // add new item
            slist.addItem();
            break;
        case 3: // edit existing item
            slist.editItem();
            break;
        case 4: // remove existing item
            slist.removeItem();
            break;
        case 5: // load list from file
            slist.loadList();
            break;
        case 6: // save list to file
            slist.saveList();
            break;
        case 7: // exit program
            if (slist.isModified())
            {
                std::cout << "\nThe list has been modified but not saved.\n"
                          << "Are you sure you want to exit? (y/n): ";
                // test if user entered something other than 'y'
                if (!getYesNo())
                {
                    selection = 0;
                    continue;
                }
            }
            std::cout << "\nThank you and goodbye!\n";
            break;
        default:
            std::cout << "\nInvalid selection. Try again.\n";
        }
    } while (selection != 7);
    
    return 0;
}

void printMenu()
{
    std::cout << "Please choose an option from the menu.\n\n"
              << " 1: List the items on the shopping list\n"
              << " 2: Add an item to the shopping list\n"
              << " 3: Edit an item on the shopping list\n"
              << " 4: Remove an item from the shopping list\n"
              << " 5: Load a shopping list from a file\n"
              << " 6: Save the current shopping list to a file\n"
              << " 7: Exit the program\n";
}

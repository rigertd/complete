/*************************************************************************
 * Author:                 David Rigert
 * Date Created:           2/28/2015
 * Last Modification Date: 2/28/2015
 * Course:                 CS162_400
 * Assignment:             Lab 8
 * Filename:               World.cpp
 *
 * Overview:
 *     Implementation for World class.
 ************************************************************************/
// enum Command
// {
    // MOVE_NORTH, // go north
    // MOVE_EAST,  // go east
    // MOVE_SOUTH, // go south
    // MOVE_WEST,  // go west
    // TOGGLE_LABEL,// toggle label visibility
    // TOGGLE_EDIT,// toggle edit mode
    // EDIT_DESC,  // edit description
    // MAKE_NORTH, // make room to north
    // MAKE_EAST,  // make room to east
    // MAKE_SOUTH, // make room to south
    // MAKE_WEST,  // make room to west
    // SET_START,  // set room as start point
    // SET_END,    // set room as end point
    // EXIT,       // exit the program
    // HELP        // show available commands
// };

// enum Direction
// {
    // NORTH,
    // EAST,
    // SOUTH,
    // WEST
// };

void World::buildCommandMap()
{
    commands["north"] = MOVE_NORTH;
    commands["n"] = MOVE_NORTH;
    commands["east"] = MOVE_EAST;
    commands["e"] = MOVE_EAST;
    commands["south"] = MOVE_SOUTH;
    commands["s"] = MOVE_SOUTH;
    commands["west"] = MOVE_WEST;
    commands["w"] = MOVE_WEST;
    commands["label"] = TOGGLE_LABEL;
    commands["l"] = TOGGLE_LABEL;
    commands["edit"] = TOGGLE_EDIT;
    commands["description"] = EDIT_DESC;
    commands["desc"] = EDIT_DESC;
    commands["makenorth"] = MAKE_NORTH;
    commands["mn"] = MAKE_NORTH;
    commands["makeeast"] = MAKE_EAST;
    commands["me"] = MAKE_EAST;
    commands["makesouth"] = MAKE_SOUTH;
    commands["ms"] = MAKE_SOUTH;
    commands["makewest"] = MAKE_WEST;
    commands["mw"] = MAKE_WEST;
    commands["deletenorth"] = DELETE_NORTH;
    commands["dn"] = DELETE_NORTH;
    commands["deleteeast"] = DELETE_EAST;
    commands["de"] = DELETE_EAST;
    commands["deletesouth"] = DELETE_SOUTH;
    commands["ds"] = DELETE_SOUTH;
    commands["deletewest"] = DELETE_WEST;
    commands["dw"] = DELETE_WEST;
    commands["setstart"] = SET_START;
    commands["setend"] = SET_END;
    commands["help"] = HELP;
    commands["h"] = HELP;
    commands["exit"] = EXIT;
    commands["x"] = EXIT;
    commands["quit"] = EXIT;
    commands["q"] = EXIT;
}
    
/******************************************************
*             Private Member Functions                *
******************************************************/
// loads the world from a file
void World::load(std::ifstream &in)
{
    
}

// saves the world to a file
void World::save(std::ofstream &out)
{
    
}

// deletes the room in the specified direction
// and any orphaned rooms
void World::deleteRoom(Direction d)
{
    switch (d)
    {
    case NORTH:
        if (north != NULL)
        {
            // just delete pointer if still connected to starting point
            if (isConnectedToStart(*north))
            {
                // remove return pointer
                north->south = NULL;
                delete north;
                north = NULL;
            }
            // otherwise delete the entire tree
            else
            {
                deleteTree(north);
                north = NULL;
            }
        }
        else
        {
            std::cout << "No room exists there!\n";
        }
        break;
    case EAST:
        if (north != NULL)
        {
            // just delete pointer if still connected to starting point
            if (isConnectedToStart(*north))
            {
                // remove return pointer
                north->south = NULL;
                delete north;
                north = NULL;
            }
            // otherwise delete the entire tree
            else
            {
                deleteTree(north);
                north = NULL;
            }
        }
        else
        {
            std::cout << "No room exists there!\n";
        }
        break;
    case SOUTH:
        if (north != NULL)
        {
            // just delete pointer if still connected to starting point
            if (isConnectedToStart(*north))
            {
                // remove return pointer
                north->south = NULL;
                delete north;
                north = NULL;
            }
            // otherwise delete the entire tree
            else
            {
                deleteTree(north);
                north = NULL;
            }
        }
        else
        {
            std::cout << "No room exists there!\n";
        }
        break;
    case WEST:
        if (north != NULL)
        {
            // just delete pointer if still connected to starting point
            if (isConnectedToStart(*north))
            {
                // remove return pointer
                north->south = NULL;
                delete north;
                north = NULL;
            }
            // otherwise delete the entire tree
            else
            {
                deleteTree(north);
                north = NULL;
            }
        }
        else
        {
            std::cout << "No room exists there!\n";
        }
        break;
    }
}

// deletes the entire tree connected to the specified room
void World::deleteTree(Room *)
{
    
}

// edits the description of the current room
void World::editRoom()
{
    
}

// checks if room has path to starting point
bool World::isConnectedToStart(Room *rm) const
{
    return false;
}

// creates a new room in the specified direction
void makeRoom(Direction d);

// moves in the specified direction
void World::move(Direction d);

// displays the help menu
void printHelp() const;

// displays the current room
void printRoom() const;

// runs the specified command
void World::runCommand(Command c)
{
    switch (c)
    {
    case MOVE_NORTH:
        break;
    case MOVE_EAST:
        break;
    case MOVE_SOUTH:
        break;
    case MOVE_WEST:
        break;
    case TOGGLE_LABEL:
        break;
    case TOGGLE_EDIT:
        break;
    case EDIT_DESC:
        break;
    case MAKE_NORTH:
        makeRoom(NORTH);
        break;
    case MAKE_EAST:
        makeRoom(EAST);
        break;
    case MAKE_SOUTH:
        makeRoom(SOUTH);
        break;
    case MAKE_WEST:
        makeRoom(WEST);
        break;
    case DELETE_NORTH:
        deleteRoom(NORTH);
        break;
    case DELETE_EAST:
        deleteRoom(EAST);
        break;
    case DELETE_SOUTH:
        deleteRoom(SOUTH);
        break;
    case DELETE_WEST:
        deleteRoom(WEST);
        break;
    case SET_START:
        break;
    case SET_END:
        break;
    case HELP:
        break;
    case EXIT:
        break;
    default:
        std::cout << "Invalid command. Try again or type 'help' for help.\n";
    }
}
    
/******************************************************
*            Constructors and Destructor              *
******************************************************/
World();
~World();

/******************************************************
*              Public Member Functions                *
******************************************************/
// parses user input and runs the specified command
void command(std::string &);

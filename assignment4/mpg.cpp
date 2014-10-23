/**********************************************************
 * Author:                 David Rigert
 * Date Created:           10/22/2014
 * Last Modification Date: 10/22/2014
 * Assignment:             Assignment 4
 * Filename:               mpg.cpp
 *
 * Overview:
 *     This program takes the liters of gasoline consumed
 *     and the distance travelled in miles as input, 
 *     and gives the miles per gallon.
 * 
 * Input:
 *     Liters used and miles travelled as real numbers
 *
 * Output:
 *     The output to the console will be in the form of:
 *
 *     Enter how many liters of gasoline you used: [10]
 *     Enter how many miles you travelled: [100]
 *
 *     Your fuel economy was 37.85 MPG.
 *
 *     Do you want to calculate your MPG again? [Y/N]: [n]
 *********************************************************/
#include <iostream> // For cout
#include <iomanip>  // For setprecision and fixed
#include <limits>   // For numeric_limits
#include <string>   // For string class

// Function prototypes

// Calculates the MPG based on liters and miles.
double GetMpg(double, double); 
// Gets a double value from the user.
double GetValidatedDouble(std::string); 
// Gets a yes or no response from the user.
bool GetYesNo(std::string);

const double GALLONS_PER_LITER = 0.264179;

int main()
{
    do
    {
        double liters;  // Liters of gasoline used
        double miles;   // Miles travelled
        double mpg;     // Equivalent MPG

        // Get valid user input. Value must be greater than 0.
        liters = GetValidatedDouble(
	    "Enter how many liters of gasoline you used: ");
        while (liters <= 0)
        {
            liters = GetValidatedDouble(
	        "You must enter a value larger than 0: ");
        }

        miles = GetValidatedDouble("Enter how many miles you travelled: ");
        while (miles <= 0)
        {
            miles = GetValidatedDouble(
	        "You must enter a value larger than 0: ");
        }
        
        // Get the MPG
        mpg = GetMpg(liters, miles);
        // Print the miles per gallon, up to 2 decimal places
        std::cout << std::endl << "Your fuel economy was "
                  << std::fixed << std::setprecision(2) << mpg << " MPG."
                  << std::endl << std::endl;
    }
    while (GetYesNo("Do you want to calculate your MPG again? [Y/N]: "));
    
    return 0;
}

/**********************************************************
 *  double GetMpg(double liters, double miles)
 *
 *  Purpose: This function takes liters and miles as arguments
 *           and returns the equivalent miles per gallon.
 *
 *  Preconditions: liters and miles are greater than 0
 *   
 *  Postconditions: Returns miles per gallon as a double value
 *********************************************************/
double GetMpg(double liters, double miles)
{
    return miles / (liters * GALLONS_PER_LITER);
}

/**********************************************************
 *  double GetValidatedDouble(std::string prompt)
 *
 *  Purpose: This function displays the specified prompt
 *           and gets a valid double from the user.
 *
 *  Preconditions: None.
 *   
 *  Postconditions: Leaves the input buffer empty 
 *                  and returns a double value.
 *********************************************************/
double GetValidatedDouble(std::string prompt)
{
    double input = 0; // for storing user input
    
    std::cout << prompt;
    std::cin >> input;
    
    /* Implements validation by detecting the next char in the cin buffer.
     * Unless the next char is a newline, 
     * it means there is unused data still in the buffer.
     */
    while (std::cin.get() != '\n') // true if next char not newline
    {
        std::cout << std::endl << "That is not a valid number. " << std::endl
                  << "Please enter a real number between 0 and "
                  << std::numeric_limits<double>::max()  // Max double
                  << std::endl << "without any commas: ";
        std::cin.clear();     // Clear any errors.
        // Ignore any remaining input in buffer, up to the max buffer size.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> input;    // Store console input in 'input' variable.
    }
    
    return input; // Send the input value back to the caller.
}

/**********************************************************
 *  bool GetYesNo()
 *
 *  Purpose: This function displays the specified prompt
 *           until the first char of the input is
 *           'Y', 'y', 'N', or 'n'.
 *
 *  Preconditions: None.
 *   
 *  Postconditions: Leaves the input buffer empty 
 *                  and returns true if 'y' or 'Y'.
 *********************************************************/
bool GetYesNo(std::string prompt)
{
    char input; // for storing user input
    
    std::cout << prompt;
    std::cin >> input;
    
    // Flushes buffer and prompts for another value until
    // a valid value is entered.
    while (input != 'y' && input != 'Y' && input != 'n' && input != 'N')
    {
        std::cout << "Please type yes or no: ";
        std::cin.clear();     // Clear any errors.
        // Ignore any remaining input in buffer, up to the max buffer size.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> input;    // Store console input in 'input' variable.
    }
    
    // Clear any text left in buffer.
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return input == 'Y' || input == 'y';
}

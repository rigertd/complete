/**********************************************************
 * Author:                 David Rigert
 * Date Created:           10/22/2014
 * Last Modification Date: 10/22/2014
 * Assignment:             Assignment 4
 * Filename:               size.cpp
 *
 * Overview:
 *     This program prompts the user for their height, weight
 *     and age, and uses them to calculate the hat size,
 *     jacket size, and waist size.
 * 
 * Input:
 *     Double values for height (in inches) and weight (in pounds)
 *     and an integer value for age.
 *
 * Output:
 *     The output to the console will be in the form of:
 *
 *     Welcome to Joe's Clothing Emporium! 
 *     Tell me about yourself so I can give you your clothing sizes.
 *
 *     Enter your height (in inches): [60.75]
 *     Enter your weight (in pounds): [140.5]
 *     Enter your age: [30]
 *
 *     Your hat size is 6,
 *     your jacket size is 29,
 *     and your waist size is 24.
 *
 *     Do you want to calculate your sizes again? [Y/N]: [n]
 *********************************************************/
#include <iostream> // For cout
#include <iomanip>  // For setprecision and fixed
#include <limits>   // For numeric_limits
#include <string>   // For string class

// Function prototypes

// Calculates the hat size based on height and weight.
int GetHatSize(double, double); 
// Calculates the jacket size based on height, weight, and age.
int GetJacketSize(double, double, int); 
// Calculates the waist size based on weight and age.
int GetWaistSize(double, int); 
// Calculates the age-based adjustment multiplier.
int GetAdjustment(int, int, int); 
// Prompts the user for a valid integer and returns it.
int GetValidatedInt(std::string);
// Gets a double value from the user.
double GetValidatedDouble(std::string); 
// Gets a yes or no response from the user.
bool GetYesNo(std::string);

// Global constants
const double MAX_HEIGHT = 96.0;   // The maximum height to accept
const double MAX_WEIGHT = 1000.0; // The maximum weight to accept
const int MAX_AGE = 120;          // The maximum age to accept

int main()
{

    std::cout << "Welcome to Joe's Clothing Emporium!" << std::endl
              << "Tell me about yourself so I can give you "
              << "your clothing sizes." << std::endl << std::endl;
    do
    {
        double height; // User height
        double weight; // User weight
        int age;       // User age

        // Get valid user input. Value must be greater than 0.
        height = GetValidatedDouble("Enter your height (in inches): ");
        while (height <= 0 || height > MAX_HEIGHT)
        {
            height = GetValidatedDouble(
                (height <= 0 ? "That's impossible. Try again: "
                : "We don't carry basketball player sizes. Try again: "));
        }

        weight = GetValidatedDouble("Enter your weight (in pounds): ");
        while (weight <= 0 || weight > MAX_WEIGHT)
        {
            weight = GetValidatedDouble(
                (weight <= 0 ? "That's impossible. Try again: "
                : "Our sizes only go up to 5XL. Try again: "));
        }
        
        age = GetValidatedInt("Enter your age: ");
        while (age < 0 || age > MAX_AGE)
        {
            age = GetValidatedInt(
                (age < 0 ? "You haven't been born yet? Try again: "
                         : "You should be dead by now. Try again: "));
        }
        
        // Print out the sizes.
        std::cout << std::endl << "Your hat size is "
                  << GetHatSize(height, weight) << "," << std::endl
                  << "your jacket size is " 
                  << GetJacketSize(height, weight, age) << "," << std::endl
                  << "and your waist size is "
                  << GetWaistSize(weight, age) << "."
                  << std::endl << std::endl;
    }
    while (GetYesNo("Do you want to calculate your sizes again? [Y/N]: "));
    
    return 0;
}

/**********************************************************
 *  int GetHatSize(double height, double weight)
 *
 *  Purpose: This function takes height and weight as arguments
 *           and returns the hat size.
 *
 *  Preconditions: height and weight are greater than 0
 *   
 *  Postconditions: Returns hat size as an integer value.
 *********************************************************/
int GetHatSize(double height, double weight)
{
    const double HAT_MULT = 2.9;
    return static_cast<int>(weight / height * HAT_MULT);
}

/**********************************************************
 *  int GetJacketSize(double height, double weight, int age)
 *
 *  Purpose: This function takes height, weight, and age as arguments
 *           and returns the jacket size.
 *
 *  Preconditions: height and weight are greater than 0,
 *                 age is greater than or equal to 0
 *   
 *  Postconditions: Returns jacket size as an integer value.
 *********************************************************/
int GetJacketSize(double height, double weight, int age)
{
    const double JACKET_MULT = 1.0 / 288;
    return static_cast<int>((height * weight * JACKET_MULT) 
                           + (0.125 * GetAdjustment(age, 30, 10)));
}

/**********************************************************
 *  int GetWaistSize(double weight, int age)
 *
 *  Purpose: This function takes weight and age as arguments
 *           and returns the waist size.
 *
 *  Preconditions: weight are greater than 0,
 *                 age is greater than or equal to 0
 *   
 *  Postconditions: Returns waist size as an integer value.
 *********************************************************/
int GetWaistSize(double weight, int age)
{
    const double WAIST_MULT = 1.0 / 5.7;
    return static_cast<int>((weight * WAIST_MULT) 
                          + (0.1 * GetAdjustment(age, 28, 2)));
}

/**********************************************************
 *  int GetAdjustment(int age, int startAge, int interval)
 *
 *  Purpose: This function returns a multiplier value based on
 *           the number of complete intervals after startAge.
 *
 *  Preconditions: None
 *   
 *  Postconditions: Returns a value to multiply the adjustment by.
 *********************************************************/
int GetAdjustment(int age, int startAge, int interval)
{
    int ageDiff = age - startAge;
    // Integer truncation to only get complete intervals.
    return ageDiff > 0 ? ageDiff / interval : 0;
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
 *  int GetValidatedInt(std::string prompt)
 *
 *  Purpose: This function displays the specified prompt
 *           and gets a valid integer from the user.
 *
 *  Preconditions: None.
 *   
 *  Postconditions: Leaves the input buffer empty 
 *                  and returns an integer value.
 *********************************************************/
int GetValidatedInt(std::string prompt)
{
    int input = 0; // for storing user input
    
    std::cout << prompt;
    std::cin >> input;
    
    /* Implements validation by detecting the next char in the cin buffer.
     * Unless the next char is a newline, 
     * it means there is unused data still in the buffer.
     */
    while (std::cin.get() != '\n') // true if next char not newline
    {
        std::cout << std::endl << "That is not a valid number. " << std::endl
                  << "Please enter a whole number between 0 and "
                  << std::numeric_limits<int>::max()  // Max int
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

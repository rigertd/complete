/**********************************************************
* Author:                 David Rigert
* Date Created:           10/10/2014
* Last Modification Date: 10/11/2014
* Assignment:             Assignment 2 Project
* Filename:               numGuess.cpp
*
* Overview:
*     This program prompts one user for a secret number between 1 and 100
*     and then asks a second user to guess the secret number.
*     The user has five chances to guess the number.
*     The program tells the user if their guess was too high or too low, 
*     and provides a range of possible numbers based on previous guesses.
*     The user has a chance to play again after they win or lose.
* 
* Input:
*     Secret number
*     Five guesses of number
*     Whether to play again (Y/N)
*
* Output:
*     The output to the console will be in the form of:
*
*     Player 1, type a secret number between 1 and 100 and press Enter. [10]
*
*     (clear screen)
*     Player 2, try to guess Player 1's secret number. You have 5 guesses.
*     The number is between 1 and 100. Guess: [50]
*     
*     Your guess is too high. You have 4 guesses left.
*     The number is between 1 and 49. Guess: [5]
*
*     Your guess is too low. You have 3 guesses left.
*     The number is between 6 and 49. Guess: [10]
*
*     You guessed Player 1's secret number!
*     Do you want to play again? (Y/N) [N]
*
*     Thank you for playing!
**********************************************************/
#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

// Function prototypes
int getValidInt(string);  // Gets integer and flushes buffer
char getYesNo();          // Gets 'y' or 'n' and flushes buffer

int main()
{
    const int RANGE_LOW = 1;    // the low end of the range
    const int RANGE_HIGH = 100; // the high end of the range
    char yesOrNo;               // stores play again response

    do // Beginning of play again loop
    {
        int secretNum = 0,          // secret number
            guessNum = 0,           // player 2's guess
            guessLeft = 5,          // guesses remaining
            highGuess = RANGE_HIGH, // high end of range
            lowGuess = RANGE_LOW;   // low end of range
        
        // Create prompt string
        stringstream ss;
        ss << "Player 1, type a secret number between " << lowGuess << " and "
           << highGuess << " and press Enter. ";
        // Get the secret number from Player 1
        secretNum = getValidInt(ss.str());
        
        // Make sure number is in valid range.
        while (secretNum < lowGuess || secretNum > highGuess)
        {
            cout << "The number must be between " << lowGuess << " and "
                 << highGuess << "." << endl;
            secretNum = getValidInt("Type the number and press Enter. ");
        }
        
        // clear console with 24 newlines
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        
        // Prompt Player 2 for first guess.
        // Decrement guessLeft by one.
        cout << "Player 2, try to guess Player 1's secret number. "
             << "You have " << guessLeft-- << " guess" 
             << (guessLeft == 1 ? "." : "es.") << endl 
             << "The number is between " << lowGuess << " and " 
             << highGuess << ".";
        guessNum = getValidInt(" Guess: ");
        
        // Repeatedly ask for guess until user guesses number
        // or runs out of guesses.
        while (guessNum != secretNum && guessLeft > 0)
        {
            // Make sure input is within range
            while (guessNum < lowGuess || guessNum > highGuess) 
            {
                cout << "Don't waste your guesses!" << endl
                     << "The number is between " << lowGuess << " and " 
                     << highGuess << ".";
                guessNum = getValidInt(" Guess: ");
            }
            
            // Tell user if too high or too low
            if (guessNum > secretNum)
            {
                highGuess = guessNum - 1; // Set high end of range to guess - 1
                cout << endl << "Your guess is too high. ";
            }
            else if (guessNum < secretNum)
            {
                lowGuess = guessNum + 1;  // Set low end of range to guess + 1
                cout << endl << "Your guess is too low. ";
            }
            
            // Get next guess.
            // Decrement guessLeft by 1.
            cout << "You have " << guessLeft-- << " guess"
                 << (guessLeft == 1 ? "" : "es") << " left." << endl
                 << "The number is between " << lowGuess << " and " 
                 << highGuess << "."; 
            guessNum = getValidInt(" Guess: ");
        }
        
        if (guessNum == secretNum)
        {
            cout << endl << "You guessed Player 1's secret number!" << endl;
        }
        else
        {
            cout << endl << "Sorry, you are out of guesses. "
                 << "The secret number was " << secretNum << "." << endl
                 << "Your closest guess was off by " 
                 << abs(static_cast<float>(guessNum - secretNum)) 
                 << "." << endl;
        }
        
        // Get play again input
        yesOrNo = getYesNo();
    }
    while (yesOrNo == 'y'); // End of play again loop
    
    cout << endl << "Thank you for playing!" << endl;
    
    return 0;
}

/*  Gets the first char value from the console.
 *  Prompts the user for a yes or no answer.
 *  Reprompts until the first char is 'Y', 'y', 'N', or 'n'.
 *  Returns a lowercase 'y' or 'n'.
 */
char getYesNo()
{
    char input; // for storing user input
    
    cout << "Do you want to play again? (Y/N) ";
    cin >> input;
    
    // Flushes buffer and prompts for another value until
    // a valid value is entered.
    while (input != 'y' && input != 'Y' && input != 'n' && input != 'N')
    {
        cout << "Please type yes or no: ";
        cin.clear();     // Clear any errors.
        // Ignore any remaining input in buffer, up to the max buffer size.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> input;    // Store console input in 'input' variable.
    }
    
    // Clear any text left in buffer.
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Always return lowercase 'y' or 'n'
    return input == 'Y' || input == 'y' ? 'y' : 'n';
}

/*  Gets an integer value from the console.
 *  Prints the specified prompt before accepting input.
 *  Performs basic input validation to ensure a valid integer.
 */
int getValidInt(string prompt)
{
    int input = 0; // for storing user input
    
    cout << prompt;
    cin >> input;
    
    /* Implements validation by detecting the next char in the cin buffer.
     * Unless the next char is a newline, 
     * it means there is unused data still in the buffer.
     */
    while (cin.get() != '\n') // true if next char not newline
    {
        cout << endl << "That is not a valid number. " << endl 
             << "Please enter a whole number without any commas: ";
        cin.clear();     // Clear any errors.
        // Ignore any remaining input in buffer, up to the max buffer size.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> input;    // Store console input in 'input' variable.
    }
    
    return input; // Send the input value back to the caller.
}

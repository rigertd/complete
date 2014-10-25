/**********************************************************
 * Author:                 David Rigert
 * Date Created:           10/24/2014
 * Last Modification Date: 10/24/2014
 * Assignment:             Assignment 4
 * Filename:               secretWord.cpp
 *
 * Overview:
 *     This program prompts Player 1 for a single, secret word
 *     composed of just letters, and then asks Player 2 to guess
 *     individual letters in the secret word, or to guess the entire
 *     word based on the letters shown.
 *     For every letter correctly guessed, that letter is uncovered
 *     in the secret word.
 *     For every letter or word incorrectly guessed, a body part is
 *     added to the hangman image. Once the hangman is complete,
 *     Player 2 loses.
 *     Player 2 wins if he or she correctly guesses the word before
 *     the hangman is complete.
 * 
 * Input:
 *     Player 1: secret word
 *     Player 2: letter and word guesses, and whether to play again
 *
 * Output:
 *     The output to the console will be in the form of:
 *
 *     Welcome to Hangman!
 *     Player 1, enter your secret word using only letters: [Word]
 *
 *     (clear screen)
 *     Player 2, try to guess Player 1's secret word.
 *     You can try to guess a letter in the secret word, or
 *     the word itself if you think you know what it is.
 *     Be careful! For every incorrect guess, another body part
 *     will be added to the hangman picture.
 *
 *     -------
 *     |
 *     |
 *     |
 *     |
 *     |_______
 *     
 *     Secret Word: _ _ _ _
 *     Correct letters guessed: 
 *     Incorrect letters guessed: 
 *     Incorrect words guessed:
 *     
 *     What is your guess?:
 *********************************************************/

#include <iostream>
#include <string>
#include <cctype>

/****************************************
 Function Prototypes
 ****************************************/
// Prints the gallows and the hangman based on the mistake count
void PrintGallows(int);
// Unmasks the specified letter in the masked string
void UpdateMask(std::string, std::string, std::string);
// Prints the secret word, correct letters and incorrect letters and words
void PrintStatus(std::string, std::string, std::string, std::string);
// Checks if a letter is in the secret word
bool CheckLetter(std::string, std::string);
// Prompts user for yes/no answer and returns true if yes/no
bool GetYesNo();
// Determines whether a string contains non-letter characters
bool ContainsNonAlpha(std::string);
// Gets and validates a guess from the user
std::string GetGuess(std::string);
// Converts an entire string to uppercase
std::string StringToUpper(std::string);

int main()
{
    const int MAX_MISTAKES = 7; // Noose + body parts
    
    do // Start of play again loop
    {
        std::string secret,             // secret word
                    masked,             // masked word
                    guess,              // user guess
                    wrongLetters = "",  // wrong letter guesses
                    wrongWords = "",    // wrong word guesses
                    rightLetters = "";  // right letter guesses
        int wrongGuessCount = 0;        // number of wrong guesses
        
        std::cout << "Welcome to Hangman!\n"
                  << "Player 1, enter your secret word using only letters: ";
        std::getline(std::cin, secret);
        
        // Make sure it is a single word with only letters
        while (ContainsNonAlpha(secret))
        {
            std::cout << "The word must only contain letters. Try again: ";
            std::getline(std::cin, secret);
        }
        
        // Convert to uppercase
        secret = StringToUpper(secret);
        // Clear console
        cout << string('\n', 24);
        
        // Ask for guesses until hangman is complete
        // or player correctly guesses secret word.
        while (wrongGuessCount < MAX_MISTAKES)
        {
            PrintGallows(wrongGuessCount);
            PrintStatus(masked, rightLetters, wrongLetters, wrongWords);
        }
    } while (GetYesNo()); // Play again if user enters 'y'
    return 0;
}
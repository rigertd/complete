/**********************************************************
 * Author:                 David Rigert
 * Date Created:           10/24/2014
 * Last Modification Date: 10/25/2014
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
 *     Guess a letter or the word: 
 *********************************************************/

#include <iostream>
#include <string>
#include <cctype>
#include <limits>

using namespace std;

/****************************************
 Function Prototypes
 ****************************************/
// Prints the gallows and the hangman based on the mistake count
void PrintGallows(int);
// Unmasks the specified letter in the masked string
void UpdateMask(string, string, string &);
// Prints the masked word, correct letters and incorrect letters and words
void PrintStatus(string, string, string, string);
// Checks if the first string is in the second
bool Contains(string, string);
// Prompts user for yes/no answer and returns true if yes/no
bool GetYesNo(string);
// Determines whether a string contains non-letter characters
bool ContainsNonAlpha(string);
// Gets and validates a guess from the user
string GetGuess(string);
// Converts an entire string to uppercase
void StringToUpper(string &);
// Prints the specified string with single spaces between letters
void PrintSpaced(string);

int main()
{
    const int MAX_MISTAKES = 7; // Noose + body parts
    const int MAX_LENGTH = 20;  // max length of secret word
    const int MIN_LENGTH = 1;   // min length of secret word
    
    do // Start of play again loop
    {
        string secret,             // secret word
               masked,             // masked word
               guess,              // user guess
               wrongLetters = "",  // wrong letter guesses
               wrongWords = "",    // wrong word guesses
               rightLetters = "";  // right letter guesses
        int wrongGuessCount = 0;        // number of wrong guesses
        
        cout << "\nWelcome to Hangman!\n"
             << "Player 1, enter a secret word with " << MIN_LENGTH 
             << " to " << MAX_LENGTH << " letters: ";
        getline(cin, secret);
        
        // Make sure it is a single word between 1 and 20 letters
        while (ContainsNonAlpha(secret) || secret.length() < MIN_LENGTH 
               || secret.length() > MAX_LENGTH)
        {
            cout << "Invalid entry. "
                 << "Enter a single word made up of only letters: ";
            getline(cin, secret);
        }
        
        // Convert to uppercase
        StringToUpper(secret);
        // Set masked
        masked = string (secret.length(), '_');
        // Clear console
        cout << string (24, '\n');
        
        // Print instructions for Player 2
        cout << "Player 2, try to guess Player 1's secret word.\n"
             << "You can try to guess a letter in the secret word, or\n"
             << "the word itself if you think you know what it is.\n"
             << "Be careful! For every incorrect guess, another body part\n"
             << "will be added to the hangman picture.\n\n";
 
        
        // Ask for guesses until hangman is complete
        // or player correctly guesses secret word.
        while (wrongGuessCount < MAX_MISTAKES)
        {
            PrintGallows(wrongGuessCount);
            PrintStatus(masked, rightLetters, wrongLetters, wrongWords);
            
            guess = GetGuess("Guess a letter or the word: ");
            // Convert to uppercase
            StringToUpper(guess);
            // Clear old status
            cout << string (20, '\n');
            
            if (guess.length() == 1)
            { // Player entered a letter
                if (Contains(guess, rightLetters))
                { // Player already guessed that letter.
                    cout << "You already guessed that letter! "
                         << "Try another one.\n\n";
                }
                else if (Contains(guess, secret))
                {
                    cout << "Good guess! You uncovered a letter.\n\n";
                    // Update the masked word
                    UpdateMask(guess, secret, masked);
                    // Add correct guess to list
                    rightLetters += guess;
                }
                else
                {
                    // Increment wrong guesses
                    wrongGuessCount++;
                    cout << "Sorry, that is not in the secret word.\n\n";
                    // Add wrong guess to list
                    wrongLetters += guess;
                }
            }
            else
            { // Player entered a word
                if (Contains(guess, wrongWords))
                {
                    cout << "You already guessed that word! "
                         << "Try a different word.\n\n";
                }
                else if (guess != secret)
                {
                    wrongGuessCount++;
                    cout << "Sorry, that is not the secret word.\n\n";
                    // Add wrong guess to list
                    wrongWords += guess + " ";
                }
                else
                {
                    // Player guessed correctly
                    masked = secret;
                    break;
                }
            }
        }
        
        // Check if player guessed the word or ran out of guesses.
        if (secret == masked)
        { // Player guessed the word
            int totalGuesses = wrongGuessCount + rightLetters.length() + 1;
            cout << "Congratulations! You guessed the secret word!\n"
                 << "The secret word is \"" << secret << "\".\n"
                 << "You got it in " << totalGuesses 
                 << (totalGuesses == 1 ? " guess.\n" : " guesses.\n");
        }
        else
        { // Player did not guess the word
            cout << "Sorry, you ran out of guesses.\n"
                 << "The secret word was \"" << secret << "\".\n"
                 << "Better luck next time!\n";
        }
        
        cout << endl;
        
    } // Play again if user enters 'y'
    while (GetYesNo("Do you want to play again? [Y/N]: ")); 
    
    return 0;
}

/**********************************************************
 *  void PrintGallows(int mistakes)
 *
 *  Purpose: This function prints the gallows and the hangman
 *           based on the specified mistake count.
 *
 *  Preconditions: None
 *   
 *  Postconditions: The gallows is printed to the console.
 *********************************************************/
void PrintGallows(int mistakes)
{
    cout << "-------"
         << "\n|     " << (mistakes > 0 ? "|" : "")  // noose
         << "\n|     " << (mistakes > 1 ? "O" : "")  // head
         << "\n|    "  << (mistakes > 3 ? "/" : " ") // left arm
                     << (mistakes > 2 ? "|" : "")  // body
                     << (mistakes > 4 ? "\\" : "") // right arm
         << "\n|    "  << (mistakes > 5 ? "/ " : "") // left leg
                     << (mistakes > 6 ? "\\" : "") // right leg
         << "\n|_______\n\n";
}

/**********************************************************
 *  void UpdateMask(string guess, string secret, string &mask)
 *
 *  Purpose: This function unmasks the specified letter in
 *           the masked string.
 *
 *  Preconditions: guess and secret are uppercase, mask is initialized
 *   
 *  Postconditions: If the guess is found in secret, that underscore
 *                  is replaced with the letter in mask.
 *********************************************************/
void UpdateMask(string guess, string secret, string &mask)
{
    
    for (int i = 0; i < secret.length(); i++)
    {
        // Set the mask to the actual letter if found in secret
        if (secret.at(i) == guess.at(0))
        {
            mask[i] = guess.at(0);
        }
    }
}

/**********************************************************
 *  void PrintStatus(string mask, string rLet, string wLet, string wWord)
 *
 *  Purpose: This function prints the masked secret word,
 *           the correctly guessed letters, and the incorrectly
 *           guessed letters and words.
 *
 *  Preconditions: None
 *   
 *  Postconditions: The information is printed to the console
 *********************************************************/
void PrintStatus(string mask, string rLet, string wLet, string wWord)
{
    cout << "Secret Word:               ";
    PrintSpaced(mask);
    
    cout << "\nCorrect letters guessed:   ";
    if (rLet.length() > 0)
        PrintSpaced(rLet);
    else
        cout << "None"; // Print none if none guessed yet

    cout << "\nIncorrect letters guessed: ";
    if (wLet.length() > 0)
        PrintSpaced(wLet);
    else
        cout << "None"; // Print none if none guessed yet

    cout << "\nIncorrect words guessed:   ";
    if (wWord.length() > 0)
        cout << wWord;
    else
        cout << "None"; // Print none if none guessed yet
    cout << "\n\n";
}

/**********************************************************
 *  bool Contains(string search_for, string search_in)
 *
 *  Purpose: This function returns true if search_in
 *           contains search_for.
 *           This function is case sensitive.
 *
 *  Preconditions: None
 *   
 *  Postconditions: Returns true if search_in contains search_for
 *********************************************************/
bool Contains(string search_for, string search_in)
{
    return search_in.find(search_for) != string::npos;
}

/**********************************************************
 *  bool GetYesNo(string prompt)
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
bool GetYesNo(string prompt)
{
    char input; // for storing user input
    
    cout << prompt;
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

    return input == 'Y' || input == 'y';
}

/**********************************************************
 *  bool ContainsNonAlpha(string text)
 *
 *  Purpose: This function returns true if the specified text
 *           contains non-alphabetic characters.
 *
 *  Preconditions: None
 *   
 *  Postconditions: Returns true if text contains a non-letter
 *********************************************************/
bool ContainsNonAlpha(string text)
{
    // Check each letter individually
    for (int i = 0; i < text.length(); i++)
        if (!isalpha(text.at(i)))
            return true;
    
    // Did not contain any non-letters
    return false;
}

/**********************************************************
 *  string GetGuess(string prompt)
 *
 *  Purpose: This function gets a letter or word from the user
 *           and reprompts the user until input with only letters
 *           is entered.
 *
 *  Preconditions: None
 *   
 *  Postconditions: Returns a single letter or word.
 *********************************************************/
string GetGuess(string prompt)
{
    string input; // for user input
    
    cout << prompt;
    getline(cin, input);
    
    // validate input
    while (input.length() < 1 || ContainsNonAlpha(input))
    {
        cout << "Invalid guess. Type a single word or letter: ";
        getline(cin, input);
    }
    
    return input;
}

// Converts an entire string to uppercase
/**********************************************************
 *  void StringToUpper(string &text)
 *
 *  Purpose: This function converts an entire string to uppercase.
 *
 *  Preconditions: None
 *   
 *  Postconditions: The specified string is converted to uppercase.
 *********************************************************/
void StringToUpper(string &text)
{
    for (int i = 0; i < text.length(); i++)
    {
        text[i] = toupper(text.at(i));
    }
}

/**********************************************************
 *  void PrintSpaced(string word)
 *
 *  Purpose: This function prints the specified string 
 *           with single spaces between letters.
 *
 *  Preconditions: None
 *   
 *  Postconditions: The word is printed to the console.
 *********************************************************/
void PrintSpaced(string word)
{
    // Do nothing if empty string
    if (word.length() < 1)
        return;
    
    // Output first letter (fencepost)
    cout << word.at(0);
    for (int i = 1; i < word.length(); i++)
    {
        cout << " " << word.at(i);
    }
}
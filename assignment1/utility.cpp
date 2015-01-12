#include <iostream>
#include "utility.hpp"

// Include unix header if running on Linux
#if defined(__linux__)
#include <unistd.h>
#endif

// Include Windows header if running on Windows
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

// Clears the console window in both Linux and Windows
void clearWindow()
{
    // Clear window in Linux terminal using ANSI escape codes
    #if defined(__linux__)
    // clear screen
    std::cout << "\033[2J";
    // move cursor to top-left
    std::cout << "\033[1;1H";
    #endif
    
    // Clear window in Windows using cls command
    #if defined(_WIN32) || defined(_WIN64)
    system("cls");
    #endif
}

// Moves the cursor to the top-left corner of the console
void moveCursorToTopLeft()
{
    // Clear window in Linux terminal using ANSI escape codes
    #if defined(__linux__)
    // move cursor to top-left
    std::cout << "\033[1;1H";
    #endif
    
    // Place cursor in top-left position
    #if defined(_WIN32) || defined(_WIN64)
    HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position;
    position.X = 0;
    position.Y = 0;
    SetConsoleCursorPosition(screen, position);
    #endif
}

// Sleeps for the specified number of milliseconds
void sleepMilliseconds(int milliseconds)
{
    #if defined(__linux__)
    unsigned int microseconds = milliseconds * 1000;
    usleep(microseconds);
    #endif

    #if defined(_WIN32) || defined(_WIN64)
    Sleep(milliseconds);
    #endif
}
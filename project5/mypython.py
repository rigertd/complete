#!/usr/bin/python3
"""
Creates three files containing randomly generated strings.

This program creates three files in the current working directory.
Each file contains 10 randomly selected lowercase letters.
The three randomly generated strings are printed to the console,
followed by two random integers between 1 and 42 and the product
of those two numbers.

Author:     David Rigert
Class:      CS344 Winter 2016
Assignment: Program 5 - Python Exploration
"""

from random import choice
from random import randint

# Lowercase alphabet for use with choice function
LOWER_ALPHA = 'abcdefghijklmnopqrstuvwxyz'
# Number of files to generate
FILE_COUNT = 3
# Number of characters per string
CHAR_COUNT = 10
# Prefix for filenames
FILENAME_BASE = 'file'
# Minimum number
NUM_LOW = 1
# Maximum number
NUM_HIGH = 42
# Number of numbers to generate
NUM_COUNT = 2

def generateString(sourceChars, charCount):
    """
    Generates a string of randomly selected characters 
    from the specified string.
    """
    # Use the choice class to randomly choose a character,
    # and a list comprehension that does so charCount times.
    # Then join the list into a single string.
    return ''.join(choice(sourceChars) for j in range(charCount))

# This line ensures the files are only generated if run from the command line
# instead of imported as a module.
if __name__ == '__main__':
    rand_strings = []   # Stores the generated strings

    rand_nums = []      # Stores the generated numbers
    product = 1         # Set this to 1 so we can just multiply by each number

    # Generate a random string once for each file and save it to a list
    for i in range(FILE_COUNT):
        filename = FILENAME_BASE + str(i + 1) + '.txt'
        rand_string = generateString(LOWER_ALPHA, CHAR_COUNT)
        rand_strings.append(rand_string)
        print('Writing the string "{0}" to {1}'.format(rand_string, filename))
        # The file is created if it does not already exist
        with open(filename, 'w') as f:
            f.write(rand_string)

    # Generate a random number NUM_COUNT times and save them to a list
    for i in range(NUM_COUNT):
        # Generates a random integer between NUM_LOW and NUM_HIGH
        num = randint(NUM_LOW, NUM_HIGH)
        rand_nums.append(num)
        product *= num
    
    # Join the numbers together with ' * ' in between and concatenate them
    # with an equals sign and the product
    print(' * '.join(str(i) for i in rand_nums) + ' = ' + str(product))

__author__ = "David Rigert"
__email__ = "rigertd@oregonstate.edu"


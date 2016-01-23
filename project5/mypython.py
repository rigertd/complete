#!/usr/bin/python3
# Author:	David Rigert
# Class:	CS344 Winter 2016
# Assignment:	Program 5 - Python Exploration

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

# Stores the generated strings
rand_strings = []

for i in range(FILE_COUNT):
	filename = FILENAME_BASE + str(i + 1) + '.txt'
	# List comprehension chooses CHAR_COUNT random chars from LOWER_ALPHA
	rand_string = ''.join(choice(LOWER_ALPHA) for j in range(CHAR_COUNT))
	rand_strings.append(rand_string)
	print('Writing the string "{0}" to {1}'.format(rand_string, filename))
	with open(filename, 'w') as f:
		f.write(rand_string)

# Stores the generated numbers
rand_nums = []
product = 1

for i in range(NUM_COUNT):
	# Generates a random integer between NUM_LOW and NUM_HIGH
	num = randint(NUM_LOW, NUM_HIGH)
	rand_nums.append(num)
	product *= num

print(' * '.join(str(i) for i in rand_nums) + ' = ' + str(product))


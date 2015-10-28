#!/usr/bin/env python3
import fileinput    # For reading from file
import ast          # For turning bracketed string of integers into an actual list
import sys          # For command line checking
import os           # For path operations
"""
Project 2 for CS 325 Section 401, Fall 2015
This project calls for the implementation of three different
algorithms for computing the minimum change possible to provide a customer.

Group Members: David Rigert, Isaiah Perrotte-Fentress, Adam McDaniel
"""

def load_problems(filename):
    """
    Loads the problem lists from the specified file.
    This function takes in list notation '[1, 2, 3]'
    alternated with single integer lines like '15'.
    """
    values = []     # Holds V values from text doc
    change = []     # Holds A values from text doc
    try:
        f = open(filename, 'r')
    except IOError as e:
        print('Cannot open {0}. {1}: {2}'.format(filename, e.errno, e.strerror))
    else:
        print('Loading data from {0}...'.format(filename))
        lines = (line.strip('\n') for line in f)        # Get list of strings
        for i, arr in enumerate(lines):                 # Loop through all lines
            if i % 2 == 1:                              # For even lines (odd index) add to change
                change.append(int(arr))
            else:                                       # For odd lines (even index) add to values
                values.append(ast.literal_eval(arr))    # ast.literal_eval converts string to array
        f.close()
    return values, change


def changegreedy(arr, val):
    """
    Naively calcualtes the minimum change given by using the highest amount possible to lower
    the total change left to give. If a coin value is greater than the total amount left to give,
    it moves on to the next smallest.
    """
    # Ensure array is populated correctly
    if arr[0] != 1:
        raise ValueError("The first element of arr must be 1.")
    change = val
    coin_count = 0
    coin_arr = [0]*len(arr)

    # Loop through list starting with highest coin
    for i, x in reversed(list(enumerate(arr))):
        if x <= change:
            coin_arr[i] += int(change / x)      # Use max coins possible
            coin_count += int(change / x)
            change = change % x                 # Save remaining amount

    return coin_arr, coin_count


def changeslow(arr, val):
    """
    Calculates the minimum change that can be given using change denominations in arr,
    and the goal value provided in val.
    This function uses brute force to calculate the lowest change possible by looping through
    all possibilities where coin value is less than change left to give.
    """
    # Ensure array is populated correctly
    if arr[0] != 1:
        raise ValueError("The first element of arr must be 1.")
    # Base case: If coin = remaining value, least coin way to make change for amount provided
    for i, x in reversed(list(enumerate(arr))):
        if x == val:
            coin_arr = [0] * len(arr)
            coin_arr[i] += 1
            coin_count = 1
            return coin_arr, coin_count

    temp_arr = []
    coin_arr = []
    temp_count = 0
    coin_count = 0
    # Recursive case: Recursively check for all coins the least coin way to provide change
    for i, x in reversed(list(enumerate(arr))):
        if x < val:
            temp_arr, temp_count = changeslow(arr, val - x)
            temp_count += 1
            temp_arr[i] += 1
            if coin_count == 0 or temp_count <= coin_count:
                coin_count = temp_count
                coin_arr = temp_arr
    return coin_arr, coin_count


def changedp(arr, val):
    """
    Calculates the minimum change that can be given using change denominations in arr,
    and the goal value provided in val.
    This function uses dynamic programming to remember the minimum amount of change needed
    to reach a certain value between 1...val so that work is not repeated. Removes steps of
    recursion that are unecessary.
    """
    # Ensure array is populated correctly
    if arr[0] != 1:
        raise ValueError("The first element of arr must be 1.")
    lookup_table = [([0] * len(arr), 0)]
    for i in range(1, val + 1):
        min_count = None
        coin_idx = None
        for j, v in enumerate(arr):
            if v <= i and (min_count == None or lookup_table[i - v][1] + 1 < min_count):
                min_count = lookup_table[i - v][1] + 1
                coin_idx = j

        # make a copy of counts for current val - coin val
        count_arr = lookup_table[i - arr[coin_idx]][0][:]
        count_arr[coin_idx] += 1
        lookup_table.append((count_arr, min_count))

    return lookup_table[val]


if __name__ == '__main__':
    # Ensure user provides file name, otherwise exit with instructions
    if len(sys.argv) < 2:
        print('Include the input file name (%.txt) as a command line argument.\nExiting the program.\n')
        sys.exit()

    filename = sys.argv[1]                          # Locate text document name
    v, a = load_problems(filename)                  # Load problems from input file

    output = ''
    #''' Delete the # sign here and at the end of change slow to not run Algorithm #1 (especially with large values of A)
    output += 'Algorithm 1: Change Slow\n'
    for i, arr in enumerate(v):
        coin_arr, count = changeslow(arr, a[i])     # Get coin list and count
        output += str(coin_arr) + '\n'              # Output coin list
        output += str(count) + '\n'                 # Output coin count
    output += '\n'                                  # Add one additional space
    #''' # Delete the first number sign here as mentioned above

    output += 'Algorithm 2: Change Greedy\n'
    for i, arr in enumerate(v):
        coin_arr, count = changegreedy(arr, a[i])   # Get coin list and count
        output += str(coin_arr) + '\n'              # Output coin list
        output += str(count) + '\n'                 # Output coin count
    output += '\n'                                  # Add one additional space

    output += 'Algorithm 3: Change Dynamic Programming\n'
    for i, arr in enumerate(v):
        coin_arr, count = changedp(arr, a[i])       # Get coin list and count
        output += str(coin_arr) + '\n'              # Output coin list
        output += str(count) + '\n'                 # Output coin count
    output += '\n'                                  # Add one additional space

    # Write the results of the above algorithms to a .txt file
    outputname = os.path.splitext(filename)[0] + 'change.txt'
    f = open(outputname, 'w')
    f.writelines(output)
    f.close()

#!/usr/bin/env python3
"""
Project 1 for CS 325 Section 400, Fall 2015

This project calls for the implementation of four different 
algorithms for computing the maximum sum subarray. 
 
Group Members: David Rigert,
"""

import time

def load_problems(filename):
    """
    Loads the problem arrays from the specified file.
    """
    problems = [];
    try:
        f = open(filename, 'r')
    except IOError as e:
        print('Cannot open {0}. {1}: {2}'.format(filename, e.errno, e.strerror))
    else:
        print('Loading data from {0}...'.format(filename))
        lines = (line.strip('[]\n ') for line in f)         # Get list of array strings.
        lines = (line.split(',') for line in lines if line) # Remove any blank lines and split into arrays of values.
        for i, arr in enumerate(lines):
            try:
                problems.append(list(map(int, arr)))        # Convert values to ints.
            except:
                print('Skipping input line {0}.\nInvalid format: {1}'.format(i+1, arr))

        print('Done.\n')
        f.close()

    return problems


def max_subarray_enum(arr):
    """
    Calculates the maximum sum subarray using naive enumeration.
    """
    max_sum = 0                                     # Initialize max_sum to 0.
    for i in range(len(arr)):
        high_sum = 0                                # Initialize high_sum and current_sum to 0.
        for j in range(i, len(arr)):
            current_sum = sum(arr[i:j+1])           # Calculate sum of indices i to j
            high_sum = max(high_sum, current_sum)   # Set high_sum if current_sum is higher.
        max_sum = max(max_sum, high_sum)            # Set max_sum to highest value of high_sum.

    return max_sum                                  # Return the maximum sum.

def max_subarray_better_enum(arr):
    """
    Calculates the maximum sum subarray by keeping a running total of the values
    in arr[i:j+1] and checking for a maximum sum each iteration.
    """
    max_sum = 0                                     # Initialize max_sum to 0.
    for i in range(len(arr)):
        high_sum = current_sum = 0                  # Initialize high_sum and current_sum to 0.
        for j in range(i, len(arr)):
            current_sum += arr[j]                   # Keep running total from indices i to j.
            high_sum = max(high_sum, current_sum)   # Set high_sum to highest value of running total.
        max_sum = max(max_sum, high_sum)            # Set max_sum to highest value of high_sum.

    return max_sum                                  # Return the maximum sum.

def max_subarray_divide_and_conquer(arr, start, end):
    """
    Calculates the maximum sum subarray recursively by finding the maximum
    sum of the first half, the second half, and the array crossing the middle.
    """
    if start >= end:                    # Array only has one element--return it.
        return arr[end]
    else:
        mid = int((end + start) / 2)    # Find the mid point.
        left = max_subarray_divide_and_conquer(arr, start, mid)             # Recursively find the left MSS.
        right = max_subarray_divide_and_conquer(arr, mid+1, end)            # Recursively find the right MSS.
        cross = max_suffix(arr, start, mid) + max_prefix(arr, mid+1, end)   # Find the cross MSS in linear time.
        return max(left, right, cross)  # Return the largest MSS.

def max_suffix(arr, start, end):
    """
    Calculates the maximum sum subarray of arr from end to start.
    """
    max_sum = current_sum = 0
    for i in range(end, start-1, -1):
        current_sum += arr[i]
        max_sum = max(max_sum, current_sum)

    return max_sum

def max_prefix(arr, start, end):
    """
    Calculates the maximum sum subarray of arr from start to end.
    """
    max_sum = current_sum = 0
    for i in range(start, end+1):
        current_sum += arr[i]
        max_sum = max(max_sum, current_sum)

    return max_sum

def max_subarray_dynamic(arr):
    """
    Calculates the maximum sum subarray using Kadane's algorithm.
    """
    max_up_to = max_sum = arr[0]                # Set initial max to first value.
    for i in range(1, len(arr)):
        max_up_to = max(0, max_up_to + arr[i])  # Reset to 0 if it ever becomes negative.
        max_sum = max(max_sum, max_up_to)       # Keep track of the highest max sum.

    return max_sum

probs = load_problems('MSS_TestProblems-1.txt')

print('{0:18}{1:18}{2:18}{3:18}'.format('Enum', 'Better Enum', 'Divide & Conquer', 'Dynamic'))
print('-' * 72)

for vals in probs:
    print('{0:17} {1:17} {2:17} {3:17}'.format(max_subarray_enum(vals), max_subarray_better_enum(vals), max_subarray_divide_and_conquer(vals,0,len(vals)-1), max_subarray_dynamic(vals)))

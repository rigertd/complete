#!/usr/bin/env python3
"""
Project 1 for CS 325 Section 401, Fall 2015

This project calls for the implementation of four different 
algorithms for computing the maximum sum subarray.
 
Group Members: David Rigert, Isaiah Perrotte-Fentress, Adam McDaniel
"""

def load_problems(filename):
    """
    Loads the problem arrays from the specified file.

    This function supports text input files with both 
    array notation such as '[1, 2, 3]' and CSV notation such as '1, 2, 3'.
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

    This function uses brute force to calculate the sum of arr from indices i to j
    for every permutation of (i, j) where i <= j.
    """
    max_sum = 0                                 # Initialize max_sum to 0.
    mss_start = mss_end = 0                     # Initialize the start and end indices to 0.
    for i in range(len(arr)):
        high_sum = 0                            # Initialize high_sum to 0.
        start, end = i, None                    # Initialize start and end for this i
        for j in range(i, len(arr)):
            current_sum = sum(arr[i:j+1])       # Calculate sum of indices i to j
            if current_sum > high_sum:
                high_sum = current_sum          # Set high_sum if current_sum is higher.
                end = j                         # Set the end index for the current high sum subarray.
        if high_sum > max_sum:
            max_sum = high_sum                  # Set max_sum to highest value of high_sum.
            mss_start, mss_end = start, end     # Set start and end of highest MSS so far.

    return max_sum, mss_start, mss_end          # Return the maximum sum and the first and last indices of the MSS.

def max_subarray_better_enum(arr):
    """
    Calculates the maximum sum subarray by keeping a running total of the values.

    This function calculates the sum of arr from indices i to j for every permutation
    of (i, j) where i <= j. It reuses the sum of (i, j) when calculating (i, j+1).
    """
    max_sum = 0                                 # Initialize max_sum to 0.
    mss_start = mss_end = 0                     # Initialize the start and end indices to 0.
    for i in range(len(arr)):
        high_sum = current_sum = 0              # Initialize high_sum and current_sum to 0.
        start, end = i, None                    # Initialize start and end for this i
        for j in range(i, len(arr)):
            current_sum += arr[j]               # Keep running total from indices i to j.
            if current_sum > high_sum:
                high_sum = current_sum          # Set high_sum to highest value of running total.
                end = j                         # Set the end index.
        if high_sum > max_sum:
            max_sum = high_sum                  # Set max_sum to highest value of high_sum.
            mss_start, mss_end = start, end     # Set start and end of highest MSS so far.

    return max_sum, mss_start, mss_end          # Return the maximum sum and the first and last indices of the MSS.

def max_subarray_divide_and_conquer(arr, start = 0, end = None):
    """
    Calculates the maximum sum subarray recursively using divide and conquer.
    
    This function recursively calculates the maximum sum of the first half of arr,
    of the second half of arr, and of the range that spans the middle.
    The base case is when start is equal to end (just one element).
    """
    if end == None:                     # Set end to array length if none was specified.
        end = len(arr) - 1
    if start >= end:                    # Array only has one element--return it.
        return arr[end], start, end
    else:
        mss_start = mss_end = 0         # Initialize start and end indices of MSS to 0
        mid = int((end + start) / 2)    # Find the mid point.
        left, lstart, lend = max_subarray_divide_and_conquer(arr, start, mid)   # Recursively find the left MSS.
        right, rstart, rend = max_subarray_divide_and_conquer(arr, mid+1, end)  # Recursively find the right MSS.
        lcross, cstart = max_suffix(arr, start, mid)                            # Find the left-side cross MSS.
        rcross, cend = max_prefix(arr, mid+1, end)                              # Find the right-side cross MSS.
        cross = lcross + rcross         # Find the total sum of the MSS that crosses the middle.
        if cross > left:                # Return the largest MSS along with first and last indices.
            if cross > right:
                return cross, cstart, cend
            else:
                return right, rstart, rend
        elif left >= right:             # && left >= cross. Use >= to match left-to-right of other algorithms.
            return left, lstart, lend
        else:                           # left >= cross && right >= left
            return right, rstart, rend

def max_suffix(arr, start, end):
    """
    Calculates the maximum sum subarray of arr from end to start.
    """
    max_sum = current_sum = 0
    mss_start = end
    for i in range(end, start-1, -1):
        current_sum += arr[i]
        if current_sum > max_sum:
            max_sum = current_sum
            mss_start = i

    return max_sum, mss_start  # Return the maximum sum and start index of cross MSS.

def max_prefix(arr, start, end):
    """
    Calculates the maximum sum subarray of arr from start to end.
    """
    max_sum = current_sum = 0
    mss_end = start
    for i in range(start, end+1):
        current_sum += arr[i]
        if current_sum > max_sum:
            max_sum = current_sum
            mss_end = i

    return max_sum, mss_end  # Return the maximum sum and end index of cross MSS.

def max_subarray_dynamic(arr):
    """
    Calculates the maximum sum subarray using Kadane's algorithm.
    """
    max_up_to = max_sum = arr[0]                # Set initial max to first value.
    mss_start = mss_end = 0                     # Initialize the MSS start and end indices to 0.
    start = end = 0                             # Initialize the temp start and end indices to 0.
    
    for i in range(1, len(arr)):
        max_up_to = max_up_to + arr[i]          # Add next value in array.
        if max_up_to < 0:
            max_up_to = 0                       # Reset to 0 if it ever becomes negative.
            start = i + 1                       # Set start to next index.
        if max_up_to > max_sum:
            max_sum = max_up_to                 # Keep track of the highest max sum.
            mss_start, mss_end = start, i       # Set start and end indices of MSS.

    return max_sum, mss_start, mss_end

if __name__ == '__main__':
    probs = load_problems('MSS_Problems.txt')
    
    output = 'Algorithm 1: Enumation\n'
    for vals in probs:
        output += str(vals) + '\n'                  # Output full array.
        m_sum, start, end = max_subarray_enum(vals) # Find MSS.
        output += str(vals[start:end + 1]) + '\n'   # Output MSS values only.
        output += str(m_sum) + '\n\n'               # Output maximum sum.

    output += 'Algorithm 2: Better Enumation\n'
    for vals in probs:
        output += str(vals) + '\n'                  # Output full array.
        m_sum, start, end = max_subarray_better_enum(vals) # Find MSS.
        output += str(vals[start:end + 1]) + '\n'   # Output MSS values only.
        output += str(m_sum) + '\n\n'               # Output maximum sum.

    output += 'Algorithm 3: Divide and Conquer\n'
    for vals in probs:
        output += str(vals) + '\n'                  # Output full array.
        m_sum, start, end = max_subarray_divide_and_conquer(vals) # Find MSS.
        output += str(vals[start:end + 1]) + '\n'   # Output MSS values only.
        output += str(m_sum) + '\n\n'               # Output maximum sum.

    output += 'Algorithm 4: Linear-time\n'
    for vals in probs:
        output += str(vals) + '\n'                  # Output full array.
        m_sum, start, end = max_subarray_enum(vals) # Find MSS.
        output += str(vals[start:end + 1]) + '\n'   # Output MSS values only.
        output += str(m_sum) + '\n\n'               # Output maximum sum.


    f = open('MSS_Results.txt', 'w')
    f.writelines(output)
    f.close()

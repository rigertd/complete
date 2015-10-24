#!/usr/bin/env python3
"""
Project 2 for CS 325 Section 401, Fall 2015
This project calls for the implementation of three different 
algorithms for computing the minimum change possible to provide a customer.
 
Group Members: David Rigert, Isaiah Perrotte-Fentress, Adam McDaniel
Helpful: http://www.mathcs.emory.edu/~cheung/Courses/323/Syllabus/DynProg/money-change.html
Helpful: http://stackoverflow.com/questions/12520263/recursive-change-making-algorithm?rq=1
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

def changegreedy(arr, val):
	"""
	Naively calcualtes the minimum change given by using the highest amount possible to lower
	the total change left to give. If a coin value is greater than the total amount left to give,
	it moves on to the next smallest.
	"""
	change = val
	coin_count = 0
	coin_arr = [0]*len(arr)
	
	for i, x in reversed(list(enumerate(arr))):
		while x <= change:
			change = change - x
			coin_arr[i] += 1
			coin_count += 1
	
	return coin_arr, coin_count


def changeslow(arr, val):

	"""
	Calculates the minimum change that can be given using change denominations in arr,
	and the goal value provided in val.
	This function uses brute force to calculate the lowest change possible by looping through
	all possibilities where coin value is less than change left to give. 
	"""

	for i, x in reversed(list(enumerate(arr))):
		if x == val:
			coin_arr = [0] * len(arr)
			coin_arr[i] += 1
			coin_count = 1
			return coin_arr, coin_count

	temp_arr = []
	coin_arr = []
	temp_count = -1
	coin_count = -1
	for i, x in reversed(list(enumerate(arr))):
		if x < val:
			temp_arr, temp_count = changeslow(arr, val-x)
			temp_count += 1
			temp_arr[i] += 1
			if coin_count == -1 or temp_count < coin_count:
				coin_count = temp_count
				coin_arr = temp_arr
	return coin_arr, coin_count

"""
def minCHange(coinValueList, change):
	minCoins = change
	if change in CoinValueList:
		return 1
	else:
		for i in [c for c in coinValueList if c <= change]:
			numCoins = 1 + minChange(coinValueList, change - i)
			if numCoins < minCoins:
				minCoins = numCoins
	return minCoins

"""			
	
	
	
if __name__ == '__main__':
	v = [1, 3, 7, 12]
	a = 29
	result_arr, result_coin = changegreedy(v, a)
	print(result_arr)
	print(result_coin)

	result_arr, result_coin = changeslow(v, a)
	print(result_arr)
	print(result_coin)


	"""
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
	"""




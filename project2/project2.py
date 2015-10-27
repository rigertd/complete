#!/usr/bin/env python3
import fileinput
import ast
"""
Project 2 for CS 325 Section 401, Fall 2015
This project calls for the implementation of three different 
algorithms for computing the minimum change possible to provide a customer.
 
Group Members: David Rigert, Isaiah Perrotte-Fentress, Adam McDaniel
"""

def load_problems(filename):
	"""
	Loads the problem arrays from the specified file.
	This function supports text input files with both 
	array notation such as '[1, 2, 3]' and CSV notation such as '1, 2, 3'.
	"""

	values = []
	change = []
	try:
		f = open(filename, 'r')
	except IOError as e:
		print('Cannot open {0}. {1}: {2}'.format(filename, e.errno, e.strerror))
	else:
		print('Loading data from {0}...'.format(filename))
		lines = (line.strip('\n') for line in f)         # Get list of array strings.
		for i, arr in enumerate(lines):
			if i % 2 == 1:
				change.append(int(arr))
			else:
				values.append(ast.literal_eval(arr))
		f.close()
	return values, change

def changegreedy(arr, val):
	"""
	Naively calcualtes the minimum change given by using the highest amount possible to lower
	the total change left to give. If a coin value is greater than the total amount left to give,
	it moves on to the next smallest.
	"""
	if arr[0] != 1:
		raise ValueError("The first element of arr must be 1.")
	change = val
	coin_count = 0
	coin_arr = [0]*len(arr)
	
	for i, x in reversed(list(enumerate(arr))):
		if x <= change:
			coin_arr[i] += int(change / x)
			coin_count += int(change / x)
			change = change % x
	
	return coin_arr, coin_count


def changeslow(arr, val):
	"""
	Calculates the minimum change that can be given using change denominations in arr,
	and the goal value provided in val.
	This function uses brute force to calculate the lowest change possible by looping through
	all possibilities where coin value is less than change left to give. 
	"""
#	if arr[0] != 1:
#		raise ValueError("The first element of arr must be 1.")
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
	
	#filename = fileinput.input()
	v, a = load_problems('Amount.txt')

	output = 'Algorithm 1: Change Slow\n'
	for i, arr in enumerate(v):
		coin_arr, count = changeslow(arr, a[i]) 	# Get coin array and count
		output += str(coin_arr) + '\n'   			# Output coin array
		output += str(count) + '\n'					# Output coin count
	output += '\n'									# Add one additional space

	output += 'Algorithm 2: Change Greedy\n'
	for i, arr in enumerate(v):
		coin_arr, count = changegreedy(arr, a[i])	# Get coin array and count
		output += str(coin_arr) + '\n'   			# Output coin array
		output += str(count) + '\n'					# Output coin count
	output += '\n'									# Add one additional space

	output += 'Algorithm 3: Change Dynamic Programming\n'
	for i, arr in enumerate(v):
		coin_arr, count = changedp(arr, a[i]) 		# Get coin array and count
		output += str(coin_arr) + '\n'   			# Output coin array
		output += str(count) + '\n'					# Output coin count
	output += '\n'									# Add one additional space

	f = open('Amountchange.txt', 'w')
	f.writelines(output)
	f.close()

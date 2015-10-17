#!/usr/bin/env python3
"""
Project 1 for CS 325 Section 401, Fall 2015

This project calls formulas in the "project1" file and calculates
their run time given different randomly generated inputs.
Exports all results to CSV.
 
Group Members: David Rigert, Isaiah Perrotte-Fentress, Adam McDaniel
"""

import time 				# Used for timing calculations
import csv 					# Used for exporting results
import project1				# Used for importing max sum formulas
from random import randint	# Used to generate a random int

# Change to modify the size of all nValues
m = 1

# Change these variables to change the value of "n"
nValue=[10*m,100*m,200*m,300*m,400*m,500*m,600*m,700*m,800*m,900*m]

# Need to ask user which formula they want to run. Incorrect input validation takes place later.
print ("Pick a formula to test by entering its integer value\n")
print ("1: Enumeration\n2: Better Enumeration\n3: Divide and Conquer\n4: Linear-time")
formulaSelect = int(input("Enter your input: "))

#Initialize the document we will print to 
with open('results.csv', 'w') as csvoutput:  
	writer = csv.writer(csvoutput, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL, lineterminator='\n')
	# Write the formula the user chose, and the header for the data we will print
	writer.writerow(["Formula Used:", formulaSelect])
	writer.writerow(["n Value", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9])

	# We want to run through 10 times for 10 random results
	for x in range(0, len(nValue)):
		# Each new test needs an array to store the time elapsed calculations
		timeList=[]
		# We want 10 results per n value, so have to loop 10 times
		for y in range(0, 10):
			# Generate an array and fill with 'n' random values
			numList=[]
			# Need to create random numbers based on the nValue assigned
			for i in range(0, nValue[x]):
				num = randint(-100,100)
				numList.append(num)
			
			# Print out results 10 times based on the code provided 
			# Essentially switch statement for using the correct formula
			if formulaSelect == 1:
				start_time = time.time()
				project1.max_subarray_enum(numList)
				elapsed_time = time.time() - start_time
				timeList.append(elapsed_time)
			elif formulaSelect == 2:
				start_time = time.time()
				project1.max_subarray_better_enum(numList)
				elapsed_time = time.time() - start_time
				timeList.append(elapsed_time)
			elif formulaSelect == 3:
				start_time = time.time()
				project1.max_subarray_divide_and_conquer(numList)
				elapsed_time = time.time() - start_time
				timeList.append(elapsed_time)
			elif formulaSelect == 4:
				start_time = time.time()
				project1.max_subarray_dynamic(numList)
				elapsed_time = time.time() - start_time
				timeList.append(elapsed_time)
			else:	# This is validation that you entered an int between 1-4... otherwise breaks
				print("You didn't enter a valid int (1-4)")
				exit("Invalid user input")
		
		# After obtaining timings for all timing values for the n numbers, we print them to the CSV file
		writer.writerow([nValue[x],timeList[0],timeList[1],timeList[2],timeList[3],timeList[4],timeList[5],timeList[6],timeList[7],timeList[8],timeList[9]])
		timeList[:] = [] 	# Reset timeList variable

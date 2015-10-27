#!/usr/bin/env python3
"""
Project 2 for CS 325 Section 401, Fall 2015
 
Group 1 members: David Rigert, Isaiah Perrotte-Fentress, Adam McDaniel

CoinChangeTimer -- Determines the running time of the algorithms
implemented in project2.py
"""

#import time 				# Used for timing calculations
#import project2				# Used for importing coin change algorithms

# Ask user which algorithm they want to run. Incorrect input validation takes place later.
print ("Choose an algorithm first:\n")
print ("1: Brute Force\n2: Greedy\n3: Dynamic Programming\n")
algorithmSelect = int(input("Enter your input: "))

# Ask user which set of inputs they want to test
print ("Now choose a pair of coin values and the value for which we want to make change:\n")
print ("1: Coin values = [1, 5, 10, 25, 50] & make change for [2010, 2015, 2020, ..., 2200]\n2: Coin values = [1, 2, 6, 12, 24, 48, 60] & make change for [2000, 2001, 2002, ..., 2200]\n3: Coin values = [1, 6, 13, 37, 150] & make change for [2000, 2001, 2002, ..., 2200]\n4: Coin values = [1, 2, 4, 6, 8, 10, 12, ..., 30] & make change for [2000, 2001, 2002, ..., 2200]\n")
inputSelect = int(input("Enter your input: "))

# Arrays to hold our coin values and the values we want to make change for
coinValues = []
changeValues = []

# Set the values of our arrays according to user selection
if inputSelect == 1:
	coinValues = [1, 5, 10, 25, 50]
	for i in range(2010, 2205, 5):
		changeValues.append(i)
elif inputSelect == 2:
	coinValues = [1, 2, 6, 12, 24, 48, 60]
	for i in range(2000, 2201):
		changeValues.append(i)
elif inputSelect == 3:
	coinValues = [1, 6, 13, 37, 150]
	for i in range(2000, 2201):
		changeValues.append(i)
elif inputSelect == 4:
	for j in range(0, 30):
		coinValues.append(j)
	for i in range(2000, 2201):
		changeValues.append(i)
else:	# This is validation that you entered an int between 1-4... otherwise breaks
	print("You didn't enter a valid int (1-4)")
	exit("Invalid user input")

# trace statements, just checking to be sure the arrays are populating correctly
# for i in range(0, len(changeValues)):
# 	print("Value ",i," = ",changeValues[i],"\n")
# for i in range(0, len(coinValues)):
# 	print("Value ",i," = ",coinValues[i],"\n")

# #Initialize the document we will print to 
# with open('results.csv', 'w') as csvoutput:  
# 	writer = csv.writer(csvoutput, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL, lineterminator='\n')
# 	# Write the formula the user chose, and the header for the data we will print
# 	writer.writerow(["Formula Used:", formulaSelect])
# 	writer.writerow(["n Value", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9])

# 	# We want to run through 10 times for 10 random results
# 	for x in range(0, len(nValue)):
# 		# Each new test needs an array to store the time elapsed calculations
# 		timeList=[]
# 		# We want 10 results per n value, so have to loop 10 times
# 		for y in range(0, 10):
# 			# Generate an array and fill with 'n' random values
# 			numList=[]
# 			# Need to create random numbers based on the nValue assigned
# 			for i in range(0, nValue[x]):
# 				num = randint(-100,100)
# 				numList.append(num)
			
# 			# Print out results 10 times based on the code provided 
# 			# Essentially switch statement for using the correct formula
# 			if formulaSelect == 1:
# 				start_time = time.time()
# 				project1.max_subarray_enum(numList)
# 				elapsed_time = time.time() - start_time
# 				timeList.append(elapsed_time)
# 			elif formulaSelect == 2:
# 				start_time = time.time()
# 				project1.max_subarray_better_enum(numList)
# 				elapsed_time = time.time() - start_time
# 				timeList.append(elapsed_time)
# 			elif formulaSelect == 3:
# 				start_time = time.time()
# 				project1.max_subarray_divide_and_conquer(numList)
# 				elapsed_time = time.time() - start_time
# 				timeList.append(elapsed_time)
# 			elif formulaSelect == 4:
# 				start_time = time.time()
# 				project1.max_subarray_dynamic(numList)
# 				elapsed_time = time.time() - start_time
# 				timeList.append(elapsed_time)
# 			else:	# This is validation that you entered an int between 1-4... otherwise breaks
# 				print("You didn't enter a valid int (1-4)")
# 				exit("Invalid user input")
		
# 		# After obtaining timings for all timing values for the n numbers, we print them to the CSV file
# 		writer.writerow([nValue[x],timeList[0],timeList[1],timeList[2],timeList[3],timeList[4],timeList[5],timeList[6],timeList[7],timeList[8],timeList[9]])
# 		timeList[:] = [] 	# Reset timeList variable
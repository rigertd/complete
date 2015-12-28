#!/usr/bin/env python3
"""
Project 2 for CS 325 Section 401, Fall 2015

Group 1 members: David Rigert, Isaiah Perrotte-Fentress, Adam McDaniel

CoinChangeTimer -- Determines the running time of the algorithms
implemented in project2.py
"""

import time                 # Used for timing calculations
import project2             # Used for importing coin change algorithms
import csv                  # Used for exporting results

# Ask user which algorithm they want to run. Incorrect input validation takes place later.
print ("Choose an algorithm first:\n")
print ("1: Brute Force\n2: Greedy\n3: Dynamic Programming\n")
algorithmSelect = int(input("Enter your input: "))

# Ask user which set of inputs they want to test
print ("Now choose a pair of coin values and the value for which we want to make change:\n")
print ("1: Coin values = [1, 5, 10, 25, 50] & make change for [2010, 2015, 2020, ..., 2200]\n2: Coin values = [1, 2, 6, 12, 24, 48, 60] & make change for [2000, 2001, 2002, ..., 2200]\n3: Coin values = [1, 6, 13, 37, 150] & make change for [2000, 2001, 2002, ..., 2200]\n4: Coin values = [1, 2, 4, 6, 8, 10, 12, ..., 30] & make change for [2000, 2001, 2002, ..., 2200]\n5: Dumbed down version of 1 for Brute Force\n6: Dumbed down version of 2 for Brute Force\n7: Dumbed down version of 3 for Brute Force\n8: Dumbed down version of 4 for Brute Force\n")
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
        coinValues.append(j + 1)
    for i in range(2000, 2201):
        changeValues.append(i)
elif inputSelect == 5:
    coinValues = [1, 5, 10, 25, 50]
    for i in range(10, 50, 5):
        changeValues.append(i)
elif inputSelect == 6:
    coinValues = [1, 2, 6, 12, 24, 48, 60]
    for i in range(15, 40):
        changeValues.append(i)
elif inputSelect == 7:
    coinValues = [1, 6, 13, 37, 150]
    for i in range(15, 40):
        changeValues.append(i)
elif inputSelect == 8:
    for j in range(0, 30):
        coinValues.append(j + 1)
    for i in range(15, 40):
        changeValues.append(i)
else:   # This is validation that you entered an int between 1-8... otherwise breaks
    print("You didn't choose a valid pair (1-8)")
    exit("Invalid user input - coin values/change")

# Customize output filename with input/results info
filename = 'algo{}input{}results.csv'.format(algorithmSelect, inputSelect)

with open(filename, 'w') as csvoutput:
    writer = csv.writer(csvoutput, delimiter=',', quoting=csv.QUOTE_MINIMAL, lineterminator='\n')

    # Call the function for the algorithm the user selected
    if algorithmSelect == 1:
        writer.writerow(['Brute Force', 'Input Pair: ',inputSelect])
        writer.writerow(['A','Number of coins required','Time required'])
        for value in changeValues:
            start_time = time.time()
            coin_array, coin_count = project2.changeslow(coinValues, value)
            run_time = time.time() - start_time
            writer.writerow([value,coin_count,run_time])
    elif algorithmSelect == 2:
        writer.writerow(['Greedy, Input Pair: ',inputSelect])
        writer.writerow(['A','Number of coins required','Time required'])
        for value in changeValues:
            start_time = time.time()
            coin_array, coin_count = project2.changegreedy(coinValues, value)
            run_time = time.time() - start_time
            writer.writerow([value,coin_count,run_time])
    elif algorithmSelect == 3:
        writer.writerow(['Dynamic Programming, Input Pair: ',inputSelect])
        writer.writerow(['A','Number of coins required','Time required'])
        for value in changeValues:
            start_time = time.time()
            coin_array, coin_count = project2.changedp(coinValues, value)
            run_time = time.time() - start_time
            writer.writerow([value,coin_count,run_time])
    else:   # This is validation that you entered an int between 1-3... otherwise breaks
        print("You didn't choose a valid algorithm (1-3)")
        exit("Invalid user input - algorithm")
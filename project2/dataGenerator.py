#!/usr/bin/env python3
"""
Project 2 for CS 325 Section 401, Fall 2015
 
Group 1 members: David Rigert, Isaiah Perrotte-Fentress, Adam McDaniel

DataGenerator -- Creates CSV data for all algorithms and a variety of n and A values.
"""

import time     # Used for timing calculations
import project2 # Used for importing coin change algorithms
import csv      # Used for exporting results

# Generate coin sets with n = [1..30]
coin_sets = []

for i in range(1, 31):
    coin_sets.append([(x**2) for x in range(1, i+1)])

MAX_A_VAL = 10000

# Run greedy and dp on entire range, but brute force only up to A = 40 and n = 7
with open('brute_results.csv', 'w') as csvoutput:
    writer = csv.writer(csvoutput, delimiter=',', quoting=csv.QUOTE_MINIMAL, lineterminator='\n')
    writer.writerow(['', 'Denomination Count'])
    writer.writerow(['A'] + list(range(1, 31)))
    for A in range(1, 40):
        print('A={}'.format(A))
        row = [A]
        for coin_set in coin_sets[:7]:
            print('n={}'.format(len(coin_set)))
            start_time = time.time()
            project2.changeslow(coin_set, A)
            run_time = time.time() - start_time
            row.append(run_time)
        writer.writerow(row)

with open('greedy_results.csv', 'w') as csvoutput:
    writer = csv.writer(csvoutput, delimiter=',', quoting=csv.QUOTE_MINIMAL, lineterminator='\n')
    writer.writerow(['', 'Denomination Count'])
    writer.writerow(['A'] + list(range(1, 31)))
    for A in range(1, MAX_A_VAL):
        print('A={}'.format(A))
        row = [A]
        for coin_set in coin_sets:
            print('n={}'.format(len(coin_set)))
            start_time = time.time()
            project2.changegreedy(coin_set, A)
            run_time = time.time() - start_time
            row.append(run_time)
        writer.writerow(row)

with open('dp_results.csv', 'w') as csvoutput:
    writer = csv.writer(csvoutput, delimiter=',', quoting=csv.QUOTE_MINIMAL, lineterminator='\n')
    writer.writerow(['', 'Denomination Count'])
    writer.writerow(['A'] + list(range(1, 31)))
    for A in range(1, MAX_A_VAL):
        print('A={}'.format(A))
        row = [A]
        for coin_set in coin_sets:
            print('n={}'.format(len(coin_set)))
            start_time = time.time()
            project2.changedp(coin_set, A)
            run_time = time.time() - start_time
            row.append(run_time)
        writer.writerow(row)

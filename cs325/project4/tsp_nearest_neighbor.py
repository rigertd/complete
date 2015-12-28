#!/usr/bin/env python3

#####
# tsp_nearest_neighbor by Adam McDaniel
# CS325, Fall 2015
# Project 4
#
# Generates an adjacency matrix with distances between cities.
# Selects an arbitrary starting point and constructs a Hamiltonian cycle by always connecting
# to the nearest unconnected neighbor.
# Selects a new starting point and runs again (10 times).
# Outputs the cost and path of shortest cycle found.
#####

import sys									#to access command-line arguments
import math									#for sqrt
import random								#for random number
import copy									#for copy operation
from collections import defaultdict			#for city_list dictionary

#calculates the distance between two cities and returns the nearest integer
def distance(x1, y1, x2, y2):
	result = math.sqrt((x1 - x2)**2 + (y1 - y2)**2)
	return int(round(result))

#####
# 1 read file in and populate city_list with (ID, x, y)
#####
city_list = defaultdict(list)

#open the input file specified by the user
input_file = open(sys.argv[1], 'r')

#read the file in to the lines variable, splitting on each newline character
lines = input_file.read().splitlines()

#place the x, y values from the file in the city_list
for line in lines:
	for ID, x, y in [line.split()]:
		city_list[int(ID)].append(int(x))
		city_list[int(ID)].append(int(y))

input_file.close()

#####	
# 2 create an adjacency matrix (if comparing city to city, 0; else, calculate the distance between cities)
#####

adjacency_matrix = [[0 for x in range(len(city_list))] for x in range(len(city_list))]

for i in range(len(city_list)):
	for j in range(len(city_list)):
		if i == j:
			adjacency_matrix[i][j] = 0
		else:
			adjacency_matrix[i][j] = distance(city_list[i][0], city_list[i][1], city_list[j][0], city_list[j][1])

#####
# 3 pick a random start city, select the neighbor with least cost, and then remove the city from set
#####

# clone the city_list to use as our set
our_set = city_list.copy()

latest_city = random.randint(1, len(city_list))

lowest_cost = 100000000000
nearest_neighbor = 0

# select neighbor with least cost
for j in range(len(city_list)):
	if adjacency_matrix[latest_city][j] < lowest:
		lowest_cost = adjacency_matrix[latest_city][j]
		nearest_neighbor = j

#add cost of nearest_neighbor to cost_total
cost_total += lowest_cost

#add city to temp array
temp_array[] = nearest_neighbor

# remove neighbor from set
del our_set[latest_city]

# if nothing is in set, finish

# if set is not empty, repeat from latest neighbor

# if the cost is less than the previous cost, replace the solution array with the temp array
solution_array

solution_cost

# run all of 3 again again from a new random city

#####
# 4 print to output file - cost on top line, then each city in solution array on new line
#####

'''
solution_file_name = sys.argv[1] + ".tour"

solution_file = open(solution_file_name, 'w+')

solution_file.write(solution_cost)
solution_file.write("\n")

for i in len(solution_array):
	solution_file.write(solution_array[i])
	solution_file.write("\n")

solution_file.close()
'''
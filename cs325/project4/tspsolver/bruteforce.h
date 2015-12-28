/**
* Multithreaded brute force algorithm with pruning to find an optimal solution to a TSP.
*
* Author:		David Rigert
* Class:		CS 325 Fall 2015
*				Project Group 1
* Last Update:	12/3/2015
*/
#pragma once

#include "common.h"

namespace tsp { namespace bruteforce {

/**
 * Finds the optimal tour for the loaded TSP.
 *
 * - totalDistance  Set to the total distance of the optimal tour.
 * - shorterThan    Specifies the distance threshold for pruning. Defaults to max unsigned int.
 *
 * Returns a list of the City IDs in the order visited. 
 * Returns an empty list if no path is possible in the specified shorterThan distance.
 */
std::vector<uint> bruteForce(uint& totalDistance, uint shorterThan = MAX_UNSIGNED);

}}

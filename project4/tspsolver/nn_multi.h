/**
 * Multithreaded nearest neighbor algorithm to find an approximate solution to a TSP.
 *
 * Author:		David Rigert
 * Class:		CS 325 Fall 2015
 *				Project Group 1
 * Last Update:	12/4/2015
 */
#pragma once

#include "common.h"

namespace tsp { namespace nneighbor {

std::vector<uint> findTourNN(uint& totalDistance, uint runFor);

} }
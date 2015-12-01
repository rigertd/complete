#pragma once

#include <vector>
#include "City.h"

#ifndef nullptr
#define nullptr NULL
#endif

namespace mstwalk {
/**
 * Loads city coordinate data from the specified path.
 * Terminates the program if an invalid path is specified.
 *
 * - path  Specifies the file path to load.
 */
void load(const char* path);

/**
 * Gets the distance between two cities using the triangle inequality.
 *
 * - u  Specifies the first city.
 * - v  Specifies the second city.
 *
 * Returns the distance rounded to the nearest integer.
 */
uint getDistance(City* u, City* v);

/**
 * Frees all resources allocated by the program.
 * You must call this to prevent memory leaks.
 */
void finalize();

/**
 * Finds the minimum spanning tree of the loaded city data using Prim's algorithm.
 *
 * Returns a pointer to the root city of the MST.
 */
City* findMst();

/**
 * Calculates a Hamiltonian cycle based on a pre-order traversal of the MST 
 * starting from the specified City.
 *
 * - start          Specifies the root City of the MST.
 * - totalDistance  Set to the total distance of the cycle.
 *
 * Returns a list of the City IDs in the order visited.
 */
std::vector<uint> mstPreorderPath(City* start, uint& totalDistance);

/**
 * Saves the specified tour distance and city IDs in a *.tour file.
 * The output filename is the input file with .tour appended.
 *
 * - path           Specifies the path to the file that the city data was loaded from.
 * - totalDistance  Specifies the total distance of the path.
 * - tour           Specifies the IDs of the cities in the order visited.
 */
void save(const char* path, uint totalDistance, std::vector<uint>& tour);

}
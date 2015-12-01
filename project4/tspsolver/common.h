#pragma once

#include <limits>
#include <vector>

#include "City.h"

#ifndef nullptr
#define nullptr NULL
#endif

namespace tsp {

// common namespace global variables
extern uint maxId;
extern std::vector<City> cities;
extern uint** distances;

// namespace constants
const uint MAX_UNSIGNED = std::numeric_limits<uint>::max();

/**
 * Frees all resources allocated by the program.
 * You must call this to prevent memory leaks.
 */
void finalize();

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
 * Loads city coordinate data from the specified path.
 * Terminates the program if an invalid path is specified.
 *
 * - path  Specifies the file path to load.
 */
void load(const char* path);

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
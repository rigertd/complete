#pragma once

#include "common.h"

namespace tsp { namespace aco {

/**
 * Finds an approximate solution to a TSP within the specified number of seconds
 * using the ant colony optimization algorithm.
 *
 * - totalDistance  Set to the total distance of the cycle.
 * - seconds        The number of seconds to run the algorithm for.
 *
 * Returns a list of the City IDs in the order visited.
 */
std::vector<uint> acoPath(uint& totalDistance, time_t seconds);

} }

#pragma once

#include "common.h"

namespace tsp { namespace mstwalk {

/**
 * Finds the minimum spanning tree of the loaded city data using Prim's algorithm.
 *
 * Returns a pointer to the root city of the MST.
 */
City* findMst(uint startIndex);

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

} }
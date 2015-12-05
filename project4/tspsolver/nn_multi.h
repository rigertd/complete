#pragma once

#include "common.h"

namespace tsp { namespace nneighbor {

std::vector<uint> findTourNN(uint& totalDistance, time_t runFor);

} }
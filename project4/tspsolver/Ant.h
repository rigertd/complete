#pragma once

#include <list>
#include "City.h"

struct Ant {
	City* city;
	std::vector<City*> path;
	std::list<City*> unvisited;
	uint distSoFar;
	float probSoFar;
};
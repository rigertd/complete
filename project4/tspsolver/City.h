#pragma once

#include <vector>

typedef unsigned int uint;

struct City {
	uint id;
	uint x;
	uint y;
	uint key;
	City* parent;
	std::vector<City*> children;
};

typedef City* cityptr;

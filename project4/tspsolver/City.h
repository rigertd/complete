/**
 * Represents a city in a Euclidean TSP.
 *
 * Author:		David Rigert
 * Class:		CS 325 Fall 2015
 *				Project Group 1
 * Last Update:	12/3/2015
 */
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

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stack>
#include <string>

#include "mst_walk.h"
#include "PriorityQueue.h"

namespace mstwalk {

// constants
const uint MAX_UNSIGNED = std::numeric_limits<uint>::max();

// namespace globals
uint maxId;
std::vector<City> cities;
uint** distances;

// private function forward delcarations
void freeDists();
void initializeDistances();

/*===============================================================*
 * Private Functions
 *===============================================================*/

// Frees all memory allocated for the distance buffer
void freeDists() {
	for (uint i = 0; i <= maxId; ++i) {
		delete[] distances[i];
	}
	delete[] distances;
	distances = nullptr;
}

// Allocates memory for the distance buffer and initializes all distances to 0
void initializeDistances() {
	distances = new uint*[maxId + 1];
	for (uint i = 0, ilen = cities.size(); i < ilen; ++i) {
		distances[i] = new uint[maxId + 1];
		for (uint j = 0; j < ilen; ++j) {
			distances[i][j] = 0;
		}
	}
}

/*===============================================================*
* Public Functions
*===============================================================*/

void finalize() {
	if (!cities.empty()) {
		freeDists();
		cities.clear();
	}
}

City* findMst() {
	PriorityQueue unvisited;

	// set starting node
	cities[0].key = 0;
	cities[0].parent = nullptr;
	unvisited.push(&cities[0]);

	// add remaining nodes to unvisited
	for (size_t i = 1, ilen = cities.size(); i < ilen; ++i) {
		cities[i].key = MAX_UNSIGNED;
		cities[i].parent = nullptr;
		unvisited.push(&cities[i]);
	}

	while (!unvisited.empty()) {
		City* current = unvisited.top();
		unvisited.pop();

		// add as child of parent node
		if (current->parent != nullptr) {
			current->parent->children.push_back(current);
		}

		// multithreading takes longer, so stay with single threaded.
		for (auto it = unvisited.begin(), end = unvisited.end(); it != end; ++it) {
			if (getDistance(current, *it) < (*it)->key) {
				(*it)->parent = current;
				(*it)->key = getDistance(current, *it);
			}
		}
		unvisited.rebuild_heap();
	}
	return &cities[0];
}

uint getDistance(City* u, City* v) {
	if (distances[u->id][v->id] > 0)
		return distances[u->id][v->id];
	if (u->id != v->id) {
		distances[u->id][v->id] = distances[v->id][u->id] = static_cast<uint>(
			std::round(
				std::sqrt(
					(u->x - v->x)*(u->x - v->x) +
					(u->y - v->y)*(u->y - v->y)
					)
				)
			);
		return distances[u->id][v->id];
	}
	else {
		return 0;
	}
}

void load(const char* path) {
	// Check if cities are already loaded. 
	// If so, free the distance buffer and cities buffer.
	finalize();

	std::ifstream ifs(path);

	// Load data from file and store in vector buffer
	if (ifs) {
		std::string lineBuffer;
		maxId = 0;

		while (std::getline(ifs, lineBuffer)) {
			if (!lineBuffer.empty()) {
				std::istringstream iss(lineBuffer);
				City c;
				iss >> c.id;
				iss >> c.x;
				iss >> c.y;
				cities.push_back(c);
				maxId = c.id > maxId ? c.id : maxId;
			}
		}

		// Initialize the distance buffer to the size of the newly loaded data
		initializeDistances();
	}
	else {
		std::cout << "Invalid path specified: " << path << std::endl;
		exit(1);
	}
}

std::vector<uint> mstPreorderPath(City* start, uint& totalDistance) {
	std::vector<uint> tour;
	std::stack<City*> waiting;
	City* current = nullptr;
	City* previous = nullptr;
	totalDistance = 0;

	waiting.push(start);
	while (!waiting.empty()) {
		current = waiting.top();
		waiting.pop();

		// pre-order
		tour.push_back(current->id);
		if (previous != nullptr) {
			totalDistance += getDistance(previous, current);
		}

		for (size_t i = 0, ilen = current->children.size(); i < ilen; ++i) {
			waiting.push(current->children[i]);
		}
		previous = current;
	}
	totalDistance += getDistance(current, start);
	return tour;
}

void save(const char* path, uint totalDistance, std::vector<uint>& tour) {
	std::string saveFile(path);
	saveFile.append(".tour");
	std::ofstream ofs(saveFile.c_str());
	
	if (ofs) {
		ofs << totalDistance << std::endl;
		for (size_t i = 0, ilen = tour.size(); i < ilen; ++i) {
			ofs << tour[i] << std::endl;
		}

		ofs.flush();
		ofs.close();
	}
}

}
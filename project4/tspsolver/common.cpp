#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "common.h"

namespace tsp {

// common namespace global variables
extern uint maxId = 0;
extern std::vector<City> cities(0);
extern uint** distances = nullptr;

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
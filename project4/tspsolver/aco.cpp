#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>

#include "aco.h"
#include "Ant.h"

namespace tsp { namespace aco {

// namespace globals
float** pheromones;			// stores the pheromone trail between cities
std::vector<Ant> ants;		// stores the ants
std::vector<uint> shortestPath; 
uint shortestDist = MAX_UNSIGNED;

const float Q = 1.0f;		// constant Q value for heuristic factor
const float ALPHA = 1.0f;	// power to raise pheromone amount to
const float BETA = 1.0f;	// power to raise heuristic factor to
const float DECAY = 0.05f;	// pheromone decay factor
const float EXTENT = static_cast<float>(maxX > maxY ? maxX : maxY); // Q value for pheromone update
const int MAX_RANDOM = std::numeric_limits<int>::max();

/*===============================================================*
 * Private Functions
 *===============================================================*/

 // Frees all memory allocated for the pheromone buffer
void freePheromones() {
	for (uint i = 0; i <= maxId; ++i) {
		delete[] pheromones[i];
	}
	delete[] pheromones;
	pheromones = nullptr;
}

// Allocates memory for the pheromone buffer and initializes all values to the specified value
void initializePheromones(float val) {
	pheromones = new float*[maxId + 1];
	for (uint i = 0, ilen = maxId + 1; i < ilen; ++i) {
		pheromones[i] = new float[maxId + 1];
		for (uint j = 0; j < ilen; ++j) {
			pheromones[i][j] = val;
		}
	}
}

// Determines number of ants based on number of cities and randomly assigns ants to cities
void populateAnts() {
	uint cityCount = cities.size();
	uint antCount = static_cast<uint>(std::ceil(std::sqrt(cityCount)) * 2);
	for (uint i = 0; i < antCount; ++i) {
		Ant a;
		a.city = &cities[rand() % cityCount];
		a.distSoFar = 0;
		a.probSoFar = 0.0f;
		uint id = a.city->id;

		// add all other cities to unvisited
		for (uint j = 0; j < id; ++j) {
			a.unvisited.push_back(&cities[j]);
		}
		for (uint j = id + 1; j < cityCount; ++j) {
			a.unvisited.push_back(&cities[j]);
		}
		ants.push_back(a);
	}
}

float getNumerator(City* u, City* v) {
	return std::powf(pheromones[u->id][v->id], ALPHA)
		* std::powf(Q / static_cast<float>(getDistance(u, v)), BETA);
}

float getProbability(Ant* a, City* u, City* v) {
	return getNumerator(u, v) / a->probSoFar;
}

void updatePheromones() {
	// pheromone decay
	for (size_t i = 0, ilen = cities.size(); i < ilen; ++i) {
		for (size_t j = i + 1; j < ilen; ++j) {
			pheromones[cities[i].id][cities[j].id] *= 1 - DECAY;
			pheromones[cities[j].id][cities[i].id] = pheromones[cities[i].id][cities[j].id];
		}
	}

	// add new pheromones
	for (size_t i = 0, ilen = ants.size(); i < ilen; ++i) {
		for (size_t j = 1, jlen = ants[i].path.size(); j < jlen; ++j) {
			pheromones[ants[i].path[j - 1]->id][ants[i].path[j]->id]
				+= EXTENT / ants[i].distSoFar;
		}
	}
}

void finishRound() {
	// complete the cycle for each ant and find shortest path
	uint idxOfShortest = ants.size();
	for (size_t i = 0, ilen = ants.size(); i < ilen; ++i) {
		// add distance to starting city
		ants[i].distSoFar += getDistance(ants[i].city, ants[i].path[0]);
		if (ants[i].distSoFar < shortestDist) {
			shortestDist = ants[i].distSoFar;
			idxOfShortest = i;
		}
	}

	// update pheromones
	updatePheromones();

	// update shortestPath and shortestDist info
	if (idxOfShortest < ants.size()) {
		shortestPath.clear();
		for (size_t i = 0, jlen = ants[idxOfShortest].path.size(); i < jlen; ++i) {
			shortestPath.push_back(ants[idxOfShortest].path[i]->id);
		}
		shortestDist = ants[idxOfShortest].distSoFar;
		std::cout << "New shortest distance found: " << shortestDist << std::endl;
	}
}

void resetAnts() {
	for (size_t i = 0, ilen = ants.size(); i < ilen; ++i) {
		ants[i].distSoFar = 0;
		ants[i].probSoFar = 0.0f;
		ants[i].path.clear();
		ants[i].unvisited.clear();

		uint id = ants[i].city->id;
		// add all other cities to unvisited
		for (uint j = 0; j < id; ++j) {
			ants[i].unvisited.push_back(&cities[j]);
		}
		for (uint j = id + 1, cityCount = cities.size(); j < cityCount; ++j) {
			ants[i].unvisited.push_back(&cities[j]);
		}
	}
}

void stepSimulation() {
	// get a random value between [0, 1)
	float randVal = static_cast<float>(rand()) / static_cast<float>(MAX_RANDOM);
	for (size_t i = 0, ilen = ants.size(); i < ilen; ++i) {
		bool updated = false;
		size_t citiesLeft = ants[i].unvisited.size();
		if (citiesLeft < 1) {
			std::cout << "Warning: attempted to step too many times" << std::endl;
			break;
		}

		// update path
		ants[i].path.push_back(ants[i].city);

		// if first round, pick city at random
		if (citiesLeft == cities.size() - 1) {
			uint offset = rand() % citiesLeft;
			auto it = ants[i].unvisited.begin();
			for (uint j = 0; j < offset; ++j) {
				++it;
			}
			float numerator = getNumerator(ants[i].city, *it);
			// update cumulative probability value
			ants[i].probSoFar += numerator;
			// update cumulative distance
			ants[i].distSoFar += getDistance(ants[i].city, *it);
			// update current city
			ants[i].city = *it;
			// remove from unvisited
			it = ants[i].unvisited.erase(it);
			updated = true;
		}
		if (updated) continue;

		// Try initial method of t_ij^a * n_ij^b / sum(t_ij^a * n_ij^b)
		for (auto it = ants[i].unvisited.begin(), end = ants[i].unvisited.end(); it != end; ++it) {
			float numerator = getNumerator(ants[i].city, *it);
			float prob = numerator / (ants[i].probSoFar + numerator);
			if (prob >= randVal) {
				// update cumulative probability value
				ants[i].probSoFar += numerator;
				// update cumulative distance
				ants[i].distSoFar += getDistance(ants[i].city, *it);
				// update current city
				ants[i].city = *it;
				// remove from unvisited
				it = ants[i].unvisited.erase(it);
				// break out of loop for this ant and go to next
				updated = true;
				break;
			}
		}
		if (updated) continue;

		// Probability ratio wasn't satisfied. Try secondary method
		float movingSum = 0.0f;
		for (auto it = ants[i].unvisited.begin(), end = ants[i].unvisited.end(); it != end; ++it) {
			float numerator = getNumerator(ants[i].city, *it);
			movingSum += numerator;
			if (movingSum >= randVal) {
				// update cumulative probability value
				ants[i].probSoFar += numerator;
				// update cumulative distance
				ants[i].distSoFar += getDistance(ants[i].city, *it);
				// update current city
				ants[i].city = *it;
				// remove from unvisited
				it = ants[i].unvisited.erase(it);
				// break out of loop for this ant and go to next
				updated = true;
				break;
			}
		}

	}
}

/*===============================================================*
 * Public Functions
 *===============================================================*/

// find the best solution in the specified number of seconds
std::vector<uint> acoPath(uint& totalDistance, time_t seconds) {
	time_t start = time(0);
	srand(static_cast<uint>(start));
	initializePheromones(1.0f / std::sqrtf(static_cast<float>(cities.size())));
	populateAnts();
	time_t firstRoundStart = time(0);
	for (size_t i = 0, ilen = cities.size() - 1; i < ilen; ++i) {
		stepSimulation();
	}
	finishRound();
	time_t timePerRound = time(0) - firstRoundStart;
	while (time(0) - start < timePerRound + seconds) {
		resetAnts();
		for (size_t i = 0, ilen = cities.size() - 1; i < ilen; ++i) {
			stepSimulation();
		}
		finishRound();
	}

	freePheromones();
	totalDistance = shortestDist;

	return shortestPath;
}

} }
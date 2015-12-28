#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <random>

#include "aco.h"
#include "Ant.h"


namespace tsp { namespace aco {

// namespace globals
float** pheromones;			// stores the pheromone trail between cities
std::vector<Ant> ants;		// stores the ants
std::vector<uint> shortestPath; 
uint shortestDist = MAX_UNSIGNED;
std::tr1::mt19937 randEngine;
std::tr1::uniform_real<float> getRand(0,1);

const int MAX_RANDOM = std::numeric_limits<int>::max();
const float Q = 0.2f;		// constant Q value for heuristic factor
const float ALPHA = 1.0f;	// power to raise pheromone amount to
const float BETA = 1.0f;	// power to raise heuristic factor to
const float DECAY = 0.1f;	// pheromone decay factor
float EXTENT = 1.0f;		// Q value for pheromone update

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

// Sets a higher pheromone value on edges that are part of the MST approximation
void setMstPheromones(City* start) {
	std::vector<City*> waiting;
	waiting.push_back(start);
	while (!waiting.empty()) {
		City* current = waiting.back();
		waiting.pop_back();
		for (size_t i = 0, ilen = current->children.size(); i < ilen; ++i) {
			pheromones[current->id][current->children[i]->id] += 10.0f;
			pheromones[current->children[i]->id][current->id] = pheromones[current->id][current->children[i]->id];
			waiting.push_back(current->children[i]);
		}
	}
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
	uint antCount;
	antCount = cityCount > 100 ? 100 : cityCount;

	for (uint i = 0; i < antCount; ++i) {
		Ant a;
		a.city = &cities[rand() % cityCount];
		a.distSoFar = 0;
		a.probSoFar = 0.0f;
		uint id = a.city->id;

		// add all other cities to unvisited
		for (uint j = 0; j < cityCount; ++j) {
			if (cities[j].id != id) {
				a.unvisited.push_back(&cities[j]);
			}
		}
		std::random_shuffle(a.unvisited.begin(), a.unvisited.end());
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
		for (uint j = 0, jlen = cities.size(); j < jlen; ++j) {
			if (cities[j].id != id) {
				ants[i].unvisited.push_back(&cities[j]);
			}
		}

		std::random_shuffle(ants[i].unvisited.begin(), ants[i].unvisited.end());
	}
}

void stepSimulation() {
	// get a random value between [0, 1)
	float randVal;
	for (size_t i = 0, ilen = ants.size(); i < ilen; ++i) {
		bool updated = false;
		size_t citiesLeft = ants[i].unvisited.size();

		// update path
		ants[i].path.push_back(ants[i].city);
		if (citiesLeft < 1) {
			return;
		}

		// Try initial method of t_ij^a * n_ij^b / sum(t_ij^a * n_ij^b)
		for (size_t j = 0, jlen = ants[i].unvisited.size(); j < jlen; ++j) {
			City* c = ants[i].unvisited[j];
			float numerator = getNumerator(ants[i].city, c);
			float prob = numerator / (ants[i].probSoFar + numerator);
			//randVal = static_cast<float>(rand()) / (1.0f + MAX_RANDOM);
			randVal = getRand(randEngine);
			if (prob >= randVal) {
				// update cumulative probability value
				ants[i].probSoFar += numerator;
				// update cumulative distance
				ants[i].distSoFar += getDistance(ants[i].city, c);
				// update current city
				ants[i].city = c;
				// remove from unvisited
				std::swap(ants[i].unvisited[j], ants[i].unvisited.back());
				ants[i].unvisited.pop_back();
				// break out of loop for this ant and go to next
				updated = true;
				break;
			}
		}
		if (updated) continue;

		// Probability ratio wasn't satisfied. Try secondary method
		float movingSum = 0.0f;
		for (size_t j = 0, jlen = ants[i].unvisited.size(); j < jlen; ++j) {
			City* c = ants[i].unvisited[j];
			float numerator = getNumerator(ants[i].city, c);
			movingSum += numerator;
			//randVal = static_cast<float>(rand()) / (1.0f + MAX_RANDOM);
			randVal = getRand(randEngine);
			if (movingSum >= randVal) {
				// update cumulative probability value
				ants[i].probSoFar += numerator;
				// update cumulative distance
				ants[i].distSoFar += getDistance(ants[i].city, c);
				// update current city
				ants[i].city = c;
				// remove from unvisited
				std::swap(ants[i].unvisited[j], ants[i].unvisited.back());
				ants[i].unvisited.pop_back();
				// break out of loop for this ant and go to next
				updated = true;
				break;
			}
		}
		if (updated) continue;

		// just in case any fall through, assign random
		City* c = ants[i].unvisited.back();
		float numerator = getNumerator(ants[i].city, c);
		// update cumulative probability value
		ants[i].probSoFar += numerator;
		// update cumulative distance
		ants[i].distSoFar += getDistance(ants[i].city, c);
		// update current city
		ants[i].city = c;
		// remove from unvisited
		ants[i].unvisited.pop_back();

	}
}

/*===============================================================*
 * Public Functions
 *===============================================================*/

// find the best solution in the specified number of seconds
std::vector<uint> acoPath(uint& totalDistance, time_t seconds, City* mstStart) {
	time_t start = time(0);
	srand(static_cast<uint>(start));
	EXTENT = static_cast<float>(maxX > maxY ? maxX : maxY);
	initializePheromones(1.0f / std::sqrtf(static_cast<float>(cities.size())));
	if (mstStart != nullptr) {
		setMstPheromones(mstStart);
	}
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
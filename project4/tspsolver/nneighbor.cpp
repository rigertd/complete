#include <queue>
#include <iostream>
#include <ctime>
#include "nneighbor.h"

bool Comparator(const cityptr& lhs, const cityptr& rhs) {
	return lhs->key > rhs->key;
}

namespace tsp {	namespace nneighbor {

std::vector<uint> bestTour;
uint bestDist = MAX_UNSIGNED;

void findTourFrom(cityptr start) {
	std::vector<cityptr> waiting;
	std::vector<uint> visited;

	start->key = 0;
	uint distance = 0;

	for (size_t i = 0, ilen = cities.size(); i < ilen; ++i) {
		if (start->id != cities[i].id) {
			cities[i].key = getDistance(start, &cities[i]);
		}
		waiting.push_back(&cities[i]);
	}
	std::make_heap(waiting.begin(), waiting.end(), Comparator);

	cityptr current;
	while (!waiting.empty()) {
		current = waiting.front();
		distance += current->key;
		visited.push_back(current->id);
		std::swap(waiting.front(), waiting.back());
		waiting.pop_back();
		for (size_t i = 0, ilen = waiting.size(); i < ilen; ++i) {
			waiting[i]->key = getDistance(current, waiting[i]);
		}
		std::make_heap(waiting.begin(), waiting.end(), Comparator);
	}

	distance += getDistance(start, current);

	if (distance < bestDist) {
		std::cout << "New best distance found: " << distance << std::endl;
		bestDist = distance;
		bestTour = visited;
	}
}

std::vector<uint> findTourNN(uint& totalDistance, time_t runFor) {
	bestTour.clear();
	if (cities.size() < 1)
		return bestTour;
	
	time_t started = time(0);
	findTourFrom(&cities[0]);
	time_t perTour = time(0) - started;
	uint iterations = static_cast<uint>(runFor) / static_cast<uint>(perTour > 0 ? perTour : 1);
	uint jumpVal = (cities.size() - 1) / iterations;
	if (jumpVal < 1) jumpVal = 1;
	std::cout << "Time to run for " << iterations << " iterations" << std::endl;
	for (size_t i = 1, ilen = cities.size(); i < ilen; i += jumpVal) {
		findTourFrom(&cities[i]);
	}

	totalDistance = bestDist;
	return bestTour;
}

} }
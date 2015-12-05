/**
 * Implementation of the nearest neighbor algorithm to find an approximate solution of a TSP.
 * This program uses all available cores minus one, unless only one core is available.
 *
 * Author:		David Rigert
 * Class:		CS 325 Fall 2015
 *				Project Group 1
 * Last Update: 12/4/2015
 */

#include "nn_multi.h"
#include <iostream>
#include <mutex>
#include <set>
#include <stack>
#include <thread>
#include <queue>
#include <ctime>

#undef max
#undef min

bool Comparator(const cityptr& lhs, const cityptr& rhs) {
	return lhs->key > rhs->key;
}

namespace tsp {	namespace nneighbor {

std::vector<uint> bestTour;
std::mutex guardBest;
uint bestDist = MAX_UNSIGNED;

void findTourFrom(cityptr start) {
	std::vector<City> localCities(cities.size());
	std::copy(cities.begin(), cities.end(), localCities.begin());
	std::vector<cityptr> waiting;
	std::vector<uint> visited;

	start->key = 0;
	uint distance = 0;

	for (size_t i = 0, ilen = localCities.size(); i < ilen; ++i) {
		if (start->id != localCities[i].id) {
			localCities[i].key = getDistance(start, &localCities[i]);
		}
		waiting.push_back(&localCities[i]);
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

	guardBest.lock();
	if (distance < bestDist) {
		std::cout << "New best distance found: " << distance << std::endl;
		bestDist = distance;
		bestTour = visited;
	}
	guardBest.unlock();
}

std::vector<uint> findTourNN(uint& totalDistance, time_t runFor) {
	bestTour.clear();
	if (cities.size() < 1)
		return bestTour;

	time_t started = time(0); // start timer

#if defined(__GNUC__)
	// GCC does not support the hardware_concurrency function.
	uint threadCount = sysconf(_SC_NPROCESSORS_ONLN);
#else
	uint threadCount = std::thread::hardware_concurrency();
#endif
	threadCount = threadCount > 1 ? threadCount - 1 : 1;
	std::vector<std::thread*> threads;

	uint cityCount = cities.size();
	uint current = 0;
	while (current < cityCount && threads.size() < threadCount) {
		threads.push_back(new std::thread(findTourFrom, &cities[current]));
		++current;
	}

	// join threads from first round
	while (threads.size() > 0) {
		if (threads[0]->joinable()) {
			threads[0]->join();
			delete threads[0];
			threads.erase(threads.begin());
		}
	}

	if (current >= cityCount) {
		totalDistance = bestDist;
		return bestTour;
	}

	time_t perRound = time(0) - started;
	time_t remaining = runFor - (time(0) - started);
	uint iterations = static_cast<uint>(remaining) / static_cast<uint>(perRound > 0 ? perRound : 1);
	uint jumpVal = ((cities.size() - 1) / iterations) / threadCount;
	if (jumpVal < 1) jumpVal = 1;
	std::cout << "Time to run for up to " << iterations << " more iterations" << std::endl;
	while (current < cityCount && remaining > perRound) {
		while (current < cityCount && threads.size() < threadCount) {
			threads.push_back(new std::thread(findTourFrom, &cities[current]));
			++current;
		}

		// join threads
		while (threads.size() > 0) {
			if (threads[0]->joinable()) {
				threads[0]->join();
				delete threads[0];
				threads.erase(threads.begin());
			}
		}

		remaining = runFor - (time(0) - started);
	}

	totalDistance = bestDist;
	return bestTour;
}

} }
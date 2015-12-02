#include <algorithm>
#include <iostream>
#include <mutex>
#include <set>
#include <stack>
#include <thread>
#include <queue>

#include "common.h"
#include "PriorityQueue.h"

template <>
bool Comparator<>(const City& lhs, const City& rhs) {
	return lhs.key > rhs.key;
}

namespace tsp { namespace bruteforce {

// globals
uint shortestSoFar = MAX_UNSIGNED;				// stores the shortest path found so far
std::vector<uint> shortestPath;
std::mutex guardShortest;

void solveBruteForcePruningRecursive(City& current, std::vector<City>& unvisited, uint distToCurrent, std::vector<uint>& path) {
	path.push_back(current.id);
	if (unvisited.empty()) {
		std::cout << "found end of path" << std::endl;
		uint totalDistance = distToCurrent + getDistance(&current, &(cities[0]));
		guardShortest.lock();
		if (totalDistance < shortestSoFar) {
			shortestSoFar = totalDistance;
			shortestPath = path;
			std::cout << "New shortest distance found: " << shortestSoFar << std::endl;
		}
		guardShortest.unlock();
	}
	else {
		std::cout << "preparing recursive next unvisited" << std::endl;
		for (size_t i = 0, ilen = unvisited.size(); i < ilen; ++i) {
			if (distToCurrent + unvisited[i].key < shortestSoFar) {
				std::vector<City> nextUnvisited;
				City next;
				for (size_t j = 0; j < ilen; ++j) {
					if (j != i) {
						next = unvisited[j];
						next.key = getDistance(&unvisited[i], &unvisited[j]);
						nextUnvisited.push_back(next);
					}
				}
				std::sort(unvisited.begin(), unvisited.end(), Comparator<City>);

				std::cout << "recursive call" << std::endl;
				solveBruteForcePruningRecursive(unvisited[i], nextUnvisited, distToCurrent + unvisited[i].key, path);
			}
		}
	}
	path.pop_back();
}

std::vector<uint> solveBruteForcePruning() {
	if (cities.empty()) {
		return shortestPath;
	}

	uint maxThreads = std::thread::hardware_concurrency();
	std::queue<std::thread*> threads;
	maxThreads = maxThreads < 2 ? 2 : maxThreads;
    
	std::cout << "setting initial unvisited" << std::endl;
	std::vector<City> unvisited;
	size_t total = cities.size();
	for (size_t i = 0; i < total; ++i) {
		cities[i].key = getDistance(&cities[0], &cities[i]);
		unvisited.push_back(cities[i]);
	}

	std::cout << "sorting unvisited" << std::endl;
	std::sort(unvisited.begin(), unvisited.end(), Comparator<City>);

	size_t processed = 1;
	while (processed < total) {

		while (threads.size() < maxThreads - 1 && processed < total) {
			std::cout << "preparing initial next unvisited" << std::endl;
			std::vector<uint> path;
			path.push_back(0);

			std::vector<City> nextUnvisited;
			City next;
			for (size_t i = 1, ilen = unvisited.size(); i < ilen; ++i) {
				if (i != processed) {
					next = unvisited[i];
					next.key = getDistance(&unvisited[processed], &unvisited[i]);
					nextUnvisited.push_back(next);
				}
			}
			std::sort(nextUnvisited.begin(), nextUnvisited.end(), Comparator<City>);
			std::cout << "creating thread" << std::endl;
			std::thread* t = new std::thread(solveBruteForcePruningRecursive, unvisited[processed], nextUnvisited, unvisited[processed].key, path);
			++processed;
			threads.push(t);
		}
		for (size_t i = 0, ilen = threads.size(); i < ilen; ++i) {
			std::cout << "joining thread " << i << std::endl;
			std::thread* t = threads.front();
			threads.pop();
			if (t->joinable()) {
				t->join();
				delete t;
			}
			else {
				threads.push(t);
			}
		}
	}
	// one more time to join the last batch
	while (!threads.empty()) {
		for (size_t i = 0, ilen = threads.size(); i < ilen; ++i) {
			std::thread* t = threads.front();
			threads.pop();
			if (t->joinable()) {
				t->join();
				delete t;
			}
			else {
				threads.push(t);
			}
		}
	}

	return shortestPath;
}

void freeDists() {
	for (uint i = 0; i <= maxId; ++i) {
		delete[] distances[i];
	}
	delete[] distances;
	distances = nullptr;
}

} }

int main(int argc, char** argv) {
	// Check if argument was specified. If not, display usage instructions.
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}
	std::cout << "loading data" << std::endl;
	tsp::load(argv[1]);
	std::cout << "done loading data" << std::endl;

	std::vector<uint> path = tsp::bruteforce::solveBruteForcePruning();

	std::cout << "saving data" << std::endl;
	tsp::save(argv[1], tsp::bruteforce::shortestSoFar, path);
	std::cout << "done saving data" << std::endl;
	tsp::finalize();
}

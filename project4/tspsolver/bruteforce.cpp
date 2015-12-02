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
        size_t ilen = unvisited.size();
		for (size_t i = ilen - 1; i >= 0; --i) {
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
    
	std::vector<City> unvisited;
	size_t total = cities.size();
	for (size_t i = 0; i < total; ++i) {
		cities[i].key = getDistance(&cities[0], &cities[i]);
		unvisited.push_back(cities[i]);
	}

	std::sort(unvisited.begin(), unvisited.end(), Comparator<City>);

	size_t idx = total - 1;
	while (idx > 0) {

		while (threads.size() < maxThreads - 1 && idx > 0) {
			std::vector<uint> path;
			path.push_back(0);

			std::vector<City> nextUnvisited;
			City next;
			for (size_t i = 1, ilen = unvisited.size(); i < ilen; ++i) {
				if (i != idx) {
					next = unvisited[i];
					next.key = getDistance(&unvisited[idx], &unvisited[i]);
					nextUnvisited.push_back(next);
				}
			}
			std::sort(nextUnvisited.begin(), nextUnvisited.end(), Comparator<City>);
			std::thread* t = new std::thread(solveBruteForcePruningRecursive, unvisited[idx], nextUnvisited, unvisited[idx].key, path);
			--idx;
			threads.push(t);
		}
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
	tsp::load(argv[1]);

	std::vector<uint> path = tsp::bruteforce::solveBruteForcePruning();

	tsp::save(argv[1], tsp::bruteforce::shortestSoFar, path);
	tsp::finalize();
}

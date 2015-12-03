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
        uint distToNext;
		for (size_t i = ilen - 1; i >= 0; --i) {
            distToNext = distToCurrent + unvisited[i].key;
			if (distToNext < shortestSoFar) {
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

				solveBruteForcePruningRecursive(unvisited[i], nextUnvisited, distToNext, path);
			}
		}
	}
	path.pop_back();
}

std::vector<uint> solveBruteForcePruning() {
    shortestPath.clear();
	if (cities.empty()) {
		return shortestPath;
	}

#if defined(__GNUC__)
    uint threadCount = sysconf (_SC_NPROCESSORS_ONLN);
#else
    uint threadCount = std::thread::hardware_concurrency();
#endif
	threadCount = threadCount > 1 ? threadCount - 1 : 1;
    std::vector<std::thread> threads;
    
	std::vector<City> unvisited;
	size_t total = cities.size();
	for (size_t i = 0; i < total; ++i) {
		cities[i].key = getDistance(&cities[0], &cities[i]);
		unvisited.push_back(cities[i]);
	}

	std::sort(unvisited.begin(), unvisited.end(), Comparator<City>);

	for (size_t j = unvisited.size() - 1; j > 0; --j) {
        std::cout << "Creating " << threadCount << " threads" << std::endl;
		while (threads.size() < threadCount && j > 0) {
            std::cout << "Creating thread " << threads.size() + 1 << std::endl;
			std::vector<uint> path;
			path.push_back(0);

			std::vector<City> nextUnvisited;
			City next;
			for (size_t i = 1, ilen = unvisited.size(); i < ilen; ++i) {
				if (i != j) {
					next = unvisited[i];
					next.key = getDistance(&unvisited[j], &unvisited[i]);
					nextUnvisited.push_back(next);
				}
			}
			std::sort(nextUnvisited.begin(), nextUnvisited.end(), Comparator<City>);
            threads.push_back(std::thread(solveBruteForcePruningRecursive, unvisited[j], nextUnvisited, unvisited[j].key, path));
		}
		for (size_t i = 0, ilen = threads.size(); i < ilen; ++i) {
            std::cout << "checking if " << i << " is joinable" << std::endl;
            if (threads[i].joinable()) {
				threads[i].join();
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

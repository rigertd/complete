#include <iostream>
#include <map>
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
std::map<uint, std::stack<uint>> solutions;		// stores the solutions sorted by distance

void solveBruteForcePruningRecursive(City current, PriorityQueue<City> unvisited, uint distToCurrent, std::stack<uint> &path) {
	path.push(current.id);
	if (unvisited.empty()) {
		uint totalDistance = distToCurrent + getDistance(&current, &(cities[0]));
		if (totalDistance < shortestSoFar) {
			shortestSoFar = totalDistance;
			solutions[totalDistance] = path;
			std::cout << "New shortest distance found: " << shortestSoFar << std::endl;
		}
	}
	else {
		while (!unvisited.empty()) {
			City next = unvisited.top();
			unvisited.pop();
			uint distToNext = getDistance(&current, &next);
			if (distToCurrent + distToNext < shortestSoFar) {
				PriorityQueue<City> nextUnvisited;
				for (auto it = unvisited.begin(), end = unvisited.end(); it != end; ++it) {
					it->key = distToNext;
					nextUnvisited.push(*it);
				}
				solveBruteForcePruningRecursive(next, nextUnvisited, distToCurrent + distToNext, path);
			}
		}
	}
	path.pop();
}

std::stack<uint> solveBruteForcePruning() {
	uint maxThreads = std::thread::hardware_concurrency();
	std::queue<std::thread*> threads;
	maxThreads = maxThreads < 2 ? 2 : maxThreads;
	std::vector<std::stack<uint>> paths;

	PriorityQueue<City> unvisited;
	for (auto it = ++(cities.begin()); it != cities.end(); ++it) {
		it->key = getDistance(&cities[0], &(*it));
		unvisited.push(*it);
		std::stack<uint> path;
		path.push(0);
		paths.push_back(path);
	}
	int iter_count = 0;
	while (!unvisited.empty()) {
		City current;
		while (threads.size() < maxThreads - 1 && !unvisited.empty()) {
			current = unvisited.top();
			PriorityQueue<City> nextUnvisited;
			for (auto it = unvisited.begin(), end = unvisited.end(); it != end; ++it) {
				if (it->id != current.id) {
					it->key = getDistance(&cities[0], &(*it));
					nextUnvisited.push(*it);
				}
			}
			std::thread* t = new std::thread(solveBruteForcePruningRecursive, current, nextUnvisited, getDistance(&(cities[0]), &current), paths[iter_count++]);
			unvisited.pop();

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
	auto it = solutions.begin();
	return it->second;
}

std::stack<uint> reversePath(std::stack<uint> path) {
	std::stack<uint> tmp;
	while (!path.empty()) {
		tmp.push(path.top());
		path.pop();
	}
	return tmp;
}

void freeDists() {
	for (uint i = 0; i <= maxId; ++i) {
		delete[] distances[i];
	}
	delete[] distances;
	distances = nullptr;
}

} }

//int main(int argc, char** argv) {
//	// Check if argument was specified. If not, display usage instructions.
//	if (argc < 2) {
//		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
//		return 1;
//	}
//	tsp::load(argv[1]);
//
//	std::stack<uint> path = tsp::bruteforce::solveBruteForcePruning();
//	path = tsp::bruteforce::reversePath(path);
//	std::vector<uint> tour(path.size());
//
//	while (!path.empty()) {
//		tour.push_back(path.top());
//		path.pop();
//	}
//
//	tsp::save(argv[1], tsp::bruteforce::shortestSoFar, tour);
//
//	tsp::finalize();
//}

#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <thread>
#include <queue>

#include "common.h"

namespace tsp { namespace bruteforce {
// for sorting children nodes by distance
struct ChildNode {
	uint id;
	uint distance;
};

struct CompareDist {
	bool operator()(const ChildNode& lhs, const ChildNode& rhs) const {
		return lhs.distance < rhs.distance;
	}
};

// globals
uint shortestSoFar = std::numeric_limits<uint>::max();	// stores the shortest path found so far
std::map<uint, std::stack<uint>> solutions;				// stores the solutions sorted by distance

void solveBruteForcePruningRecursive(City* current, std::set<ChildNode, CompareDist> unvisited, uint distToCurrent, std::stack<uint> &path) {
	path.push(current->id);
	if (unvisited.empty()) {
		uint totalDistance = distToCurrent + getDistance(current, &cities[0]);
		if (totalDistance < shortestSoFar) {
			shortestSoFar = totalDistance;
			solutions[totalDistance] = path;
			std::cout << "New shortest distance found: " << shortestSoFar << std::endl;
		}
	}
	else {
		for (auto it = unvisited.begin(), end = unvisited.end(); it != end; ++it) {
			if (distToCurrent + it->distance < shortestSoFar) {
				std::set<ChildNode, CompareDist> nextUnvisited;
				for (auto it2 = unvisited.begin(), end2 = unvisited.end(); it2 != end2; ++it2) {
					if (it != it2) {
						ChildNode cn;
						cn.distance = distances[it->id][it2->id];
						cn.id = it2->id;
						nextUnvisited.insert(cn);
					}
				}
				solveBruteForcePruningRecursive(&cities[it->id], nextUnvisited, distToCurrent + it->distance, path);
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

	std::set<ChildNode, CompareDist> unvisited;
	for (auto it = ++(cities.begin()); it != cities.end(); ++it) {
		ChildNode cn;
		cn.distance = getDistance(&cities[0], &(*it));
		cn.id = it->id;
		unvisited.insert(cn);
		std::stack<uint> path;
		path.push(0);
		paths.push_back(path);
	}
	int iter_count = 0;
	for (auto it = unvisited.begin(); it != unvisited.end(); ) {
		while (threads.size() < maxThreads - 1 && it != unvisited.end()) {
			std::set<ChildNode, CompareDist> nextUnvisited;
			for (auto it2 = unvisited.begin(), end2 = unvisited.end(); it2 != end2; ++it2) {
				if (it != it2) {
					ChildNode cn;
					cn.distance = getDistance(&cities[it->id], &cities[it2->id]);
					cn.id = it2->id;
					nextUnvisited.insert(cn);
				}
			}
			std::thread* t = new std::thread(solveBruteForcePruningRecursive, it->id, nextUnvisited, it->distance, paths[iter_count++]);
			it++;

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

int main(int argc, char** argv) {
	// Check if argument was specified. If not, display usage instructions.
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}
	tsp::load(argv[1]);

	std::stack<uint> path = tsp::bruteforce::solveBruteForcePruning();
	path = tsp::bruteforce::reversePath(path);
	std::vector<uint> tour(path.size());

	while (!path.empty()) {
		tour.push_back(path.top());
		path.pop();
	}

	tsp::save(argv[1], tsp::bruteforce::shortestSoFar, tour);

	tsp::finalize();
}

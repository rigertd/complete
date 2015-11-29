#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <map>

#ifndef nullptr
#define nullptr NULL
#endif

typedef unsigned int uint;
typedef uint dist;

// stores city info from the file
struct City {
	uint id;
	uint x;
	uint y;
};

// for sorting children nodes by distance
struct ChildNode {
	uint id;
	dist distance;
};

struct CompareDist {
	bool operator()(const ChildNode& lhs, const ChildNode& rhs) const {
		return lhs.distance < rhs.distance;
	}
};

// globals
std::map<uint, City> cities;									// stores all of the cities
dist** distances = nullptr;								// stores the distances between the cities
uint maxId = 0;											// stores the maximum ID value
uint shortestSoFar = std::numeric_limits<uint>::max();	// stores the shortest path found so far
std::map<uint, std::stack<uint>> solutions;				// stores the solutions sorted by distance

void load(const char* path) {
	std::ifstream ifs(path);

	// load data from file and store in vector buffer
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
				cities[c.id] = c;
				maxId = c.id > maxId ? c.id : maxId;
			}
		}
	}
	else {
		std::cout << "Invalid path specified: " << path << std::endl;
		exit(1);
	}
}

void computeDists() {
	distances = new dist*[maxId + 1];
	for (uint i = 0; i <= maxId; ++i) {
		distances[i] = new dist[maxId + 1];
		for (uint j = 0; j <= maxId; ++j) {
			if (cities.find(i) != cities.end() && cities.find(j) != cities.end()) {
				if (i < j) {
					distances[i][j] = static_cast<uint>(
						std::round(
							std::sqrt(
								(cities[i].x - cities[j].x)*(cities[i].x - cities[j].x) +
								(cities[i].y - cities[j].y)*(cities[i].y - cities[j].y)
								)
							)
						);
				}
				else if (j < i) {
					distances[i][j] = distances[j][i];
				}
				else { // i == j
					distances[i][j] = 0;
				}
			}
		}
	}
}

void solveBruteForcePruningRecursive(uint current, std::set<ChildNode, CompareDist> unvisited, uint distToCurrent, std::stack<uint> &path) {
	path.push(current);
	if (unvisited.empty()) {
		uint totalDistance = distToCurrent + distances[current][0];
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
				solveBruteForcePruningRecursive(it->id, nextUnvisited, distToCurrent + it->distance, path);
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
		cn.distance = distances[0][it->first];
		cn.id = it->first;
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
					cn.distance = distances[it->id][it2->id];
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

int main(int argc, char** argv) {
	// Check if argument was specified. If not, display usage instructions.
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}
	load(argv[1]);
	computeDists();

	std::stack<uint> path = solveBruteForcePruning();
	path = reversePath(path);

	char outFilePath[1024];
	snprintf(outFilePath, sizeof outFilePath, "%s%s", argv[1], ".tour");
	std::ofstream ofs(outFilePath);

	ofs << shortestSoFar << std::endl;

	std::cout << "Shortest Path: " << path.top();
	ofs << path.top() << std::endl;
	path.pop();
	while (!path.empty()) {
		uint id = path.top();
		path.pop();
		std::cout << ", " << id;
		ofs << path.top() << std::endl;
	}

	ofs.flush();
	ofs.close();

	freeDists();

	std::getchar();
}
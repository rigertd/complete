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

typedef unsigned int uint;
typedef uint dist;

struct City {
	uint id;
	uint x;
	uint y;
};

struct IdNode {
	uint id;
	IdNode* prev;
};

struct ChildNode {
	uint id;
	dist distance;
};

// globals
City* cities = nullptr;
dist** distances = nullptr;
uint maxId = 0;
uint shortestSoFar = std::numeric_limits<uint>::max();

struct CompareDist {
	bool operator()(const ChildNode& lhs, const ChildNode& rhs) const {
		return lhs.distance < rhs.distance;
	}
};

void load(const char* path) {
	std::ifstream ifs(path);

	// load data from file and store in vector buffer
	if (ifs) {
		std::vector<City> fileBuffer;
		std::string lineBuffer;
		maxId = 0;

		while (std::getline(ifs, lineBuffer)) {
			if (!lineBuffer.empty()) {
				std::istringstream iss(lineBuffer);
				City c;
				iss >> c.id;
				iss >> c.x;
				iss >> c.y;
				fileBuffer.push_back(c);
				maxId = c.id > maxId ? c.id : maxId;
			}
		}

		// initialize cities array indexed by ID
		cities = new City[maxId + 1];
		for (size_t i = 0, ilen = fileBuffer.size(); i < ilen; ++i) {
			cities[fileBuffer[i].id] = fileBuffer[i];
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
			if (i < j) {
				distances[i][j] = static_cast<uint>(
					std::sqrt(
						(cities[i].x - cities[j].x)*(cities[i].x - cities[j].x) +
						(cities[i].y - cities[j].y)*(cities[i].y - cities[j].y)
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

IdNode* findShortest(uint current, std::set<ChildNode, CompareDist> unvisited, uint distToCurrent, IdNode* last) {
	IdNode* cur = nullptr;
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
			IdNode* result = findShortest(it->id, nextUnvisited, distToCurrent + it->distance, last);
			if (result != nullptr) {
				result->prev = new IdNode{ current, nullptr };
				if (cur != nullptr) {
					delete cur;
				}
				cur = result->prev;
			}
		}
	}
	if (unvisited.empty()) {
		uint totalDistance = distToCurrent + distances[current][0];
		if (totalDistance < shortestSoFar) {
			shortestSoFar = totalDistance;
			cur = new IdNode{ current, nullptr };
			last = cur;
			std::cout << "New shortest distance found: " << shortestSoFar << std::endl;
		}
		else {
			return nullptr;
		}
	}
	return cur;
}

std::stack<uint> getShortestPath() {
	uint maxThreads = std::thread::hardware_concurrency();
	std::vector<std::thread*> threads;
	maxThreads = maxThreads < 2 ? 2 : maxThreads;
	IdNode* last = nullptr;

	std::set<ChildNode, CompareDist> unvisited;
	for (uint i = 1; i <= maxId; ++i) {
		ChildNode cn;
		cn.distance = distances[0][i];
		cn.id = i;
		unvisited.insert(cn);
	}
	while (!unvisited.empty()) {
		while (threads.size() < maxThreads - 1) {
			auto it = unvisited.begin();
			std::set<ChildNode, CompareDist> nextUnvisited;
			for (auto it2 = unvisited.begin(), end2 = unvisited.end(); it2 != end2; ++it2) {
				if (it != it2) {
					ChildNode cn;
					cn.distance = distances[it->id][it2->id];
					cn.id = it2->id;
					nextUnvisited.insert(cn);
				}
			}

			std::thread* t = new std::thread(findShortest, it->id, nextUnvisited, it->distance, last);
			unvisited.erase(it);
			threads.push_back(t);
		}
		for (size_t i = 0, ilen = threads.size(); i < ilen; ++i) {
			std::thread* t = threads[i];
			if (t->joinable()) {
				t->join();
				delete t;
			}
		}
	}
	// one more time to join the last batch
	while (!threads.empty()) {
		for (size_t i = 0, ilen = threads.size(); i < ilen; ++i) {
			std::thread* t = threads[i];
			if (t->joinable()) {
				t->join();
				delete t;
			}
		}
	}
	std::stack<uint> path;
	IdNode* node = last;
	while (node != nullptr) {
		path.push(node->id);
		node = node->prev;
	}
	return path;
}

void freeDists() {
	for (uint i = 0; i <= maxId; ++i) {
		delete[] distances[i];
	}
	delete[] distances;
	distances = nullptr;
}

void freeCities() {
	delete[] cities;
	cities = nullptr;
}

int main(int argc, char** argv) {
	// Check if argument was specified. If not, display usage instructions.
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}
	load(argv[1]);
	computeDists();

	getShortestPath();

	freeDists();
	freeCities();

	std::getchar();
}
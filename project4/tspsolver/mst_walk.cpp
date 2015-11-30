#include <queue>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <sstream>
#include <set>

#ifndef nullptr
#define nullptr NULL
#endif
typedef unsigned int uint;

struct City {
	uint id;
	uint x;
	uint y;
	uint key;
	City* parent;
	std::vector<City*> children;
};

bool CityComparator(const City* lhs, const City* rhs) {
	return lhs->key > rhs->key;
}

struct PriorityQueue {
private:
	std::vector<City*> _data;

public:
	bool empty() {
		return _data.empty();
	}

	void pop() {
		std::pop_heap(_data.begin(), _data.end(), CityComparator);
		_data.pop_back();
	}

	void push(City* value) {
		_data.push_back(value);
		std::push_heap(_data.begin(), _data.end(), CityComparator);
	}

	void rebuild_heap() {
		std::make_heap(_data.begin(), _data.end(), CityComparator);
	}

	City* top() {
		return _data.front();
	}
};

// globals
uint maxId;
std::vector<City> cities;
uint** distances;


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
				cities.push_back(c);
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
	distances = new uint*[maxId + 1];
	for (uint i = 0, ilen = cities.size(); i < ilen; ++i) {
		distances[i] = new uint[maxId + 1];
		for (uint j = 0; j < ilen; ++j) {
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

void freeDists() {
	for (uint i = 0; i <= maxId; ++i) {
		delete[] distances[i];
	}
	delete[] distances;
	distances = nullptr;
}

City* findMst() {
	PriorityQueue unvisited;
	std::set<uint> visited;
	uint maxUint = std::numeric_limits<uint>::max();

	// set starting node
	cities[0].key = 0;
	cities[0].parent = nullptr;
	unvisited.push(&cities[0]);

	// add remaining nodes to unvisited
	for (size_t i = 1, ilen = cities.size(); i < ilen; ++i) {
		cities[i].key = maxUint;
		cities[i].parent = nullptr;
		unvisited.push(&cities[i]);
	}

	while (!unvisited.empty()) {
		City* current = unvisited.top();
		unvisited.pop();
		visited.insert(current->id);

		// add as child of parent node
		if (current->parent != nullptr) {
			current->parent->children.push_back(current);
		}

		for (size_t i = 0, ilen = cities.size(); i < ilen; ++i) {
			if (visited.find(cities[i].id) == visited.end()
				&& distances[current->id][cities[i].id] < cities[i].key
				&& cities[i].id != current->id) {
				cities[i].parent = current;
				cities[i].key = distances[current->id][cities[i].id];
			}
		}
		unvisited.rebuild_heap();
	}
	return &cities[0];
}


int main(int argc, char** argv) {
	// Check if argument was specified. If not, display usage instructions.
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}
	load(argv[1]);
	computeDists();

	City* start = findMst();

	freeDists();

	return 0;
}
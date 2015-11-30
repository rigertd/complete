#include <queue>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <thread>

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
	
	std::vector<City*>::iterator begin() {
		return _data.begin();
	}

	std::vector<City*>::iterator end() {
		return _data.end();
	}

	City* operator[](uint index) {
		return _data[index];
	}

	uint size() {
		return _data.size();
	}
};

// constants
const uint MAX_UNSIGNED = std::numeric_limits<uint>::max();
const uint MAX_THREADS = std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 1;

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

void initializeDistances() {
	distances = new uint*[maxId + 1];
	for (uint i = 0, ilen = cities.size(); i < ilen; ++i) {
		distances[i] = new uint[maxId + 1];
		for (uint j = 0; j < ilen; ++j) {
			distances[i][j] = 0;
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

uint getDistance(City* u, City* v) {
	if (distances[u->id][v->id] > 0)
		return distances[u->id][v->id];
	if (u->id != v->id) {
		distances[u->id][v->id] = distances[v->id][u->id] = static_cast<uint>(
			std::round(
				std::sqrt(
					(u->x - v->x)*(u->x - v->x) +
					(u->y - v->y)*(u->y - v->y)
					)
				)
			);
		return distances[u->id][v->id];
	}
	else {
		return 0;
	}
}

City* findMst() {
	PriorityQueue unvisited;

	// set starting node
	cities[0].key = 0;
	cities[0].parent = nullptr;
	unvisited.push(&cities[0]);

	// add remaining nodes to unvisited
	for (size_t i = 1, ilen = cities.size(); i < ilen; ++i) {
		cities[i].key = MAX_UNSIGNED;
		cities[i].parent = nullptr;
		unvisited.push(&cities[i]);
	}

	while (!unvisited.empty()) {
		City* current = unvisited.top();
		unvisited.pop();

		// add as child of parent node
		if (current->parent != nullptr) {
			current->parent->children.push_back(current);
		}

		// multithreading takes longer, so stay with single threaded.
		for (auto it = unvisited.begin(), end = unvisited.end(); it != end; ++it) {
			if (getDistance(current, *it) < (*it)->key) {
				(*it)->parent = current;
				(*it)->key = getDistance(current, *it);
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
	initializeDistances();

	City* start = findMst();

	freeDists();

	return 0;
}
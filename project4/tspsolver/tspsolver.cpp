#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <cmath>
#include <stack>
#include <set>
#include <algorithm>
#include <iterator>
#include <queue>
#include <thread>

typedef unsigned int uint;

class TspSolver {
private:
	struct Distance {
		uint destId;
		uint val;
	};

	struct CompareDistance {
		bool operator()(Distance const& lhs, Distance const& rhs) {
			return lhs.val > rhs.val;
		}
	};

	struct City {
		uint id;
		uint x;
		uint y;
		std::priority_queue<Distance, std::vector<Distance>, CompareDistance> dists;
	};

	void computeDistances() {
		size_t row, col;

		for (size_t i = 0; i < count; ++i) {
			for (size_t j = i; j < count; ++j) {
				row = cities[i].id;
				col = cities[j].id;
				if (i != j) {
					distances[row][col] = distances[col][row] = static_cast<uint>(std::sqrt(
						cities[i].x * cities[i].x +
						cities[j].y * cities[j].y));
				}
				else {
					distances[row][col] = 0;
				}
			}
		}

		for (size_t i = 0; i < count; ++i) {
			for (size_t j = 0; j < count - 1; ++j) {
				if (i != j) {
					Distance d;
					d.destId = cities[j].id;
					d.val = distances[i][j];
					cities[i].dists.push(d);
				}
			}
		}
	}

	std::stack<uint> findShortestRecursive(City current, std::stack<uint>& path, std::set<uint> visited, uint totalDist) {
		while (current.dists.size() > 0) {
			Distance d = current.dists.top();
			current.dists.pop();
			if (visited.find(d.destId) == visited.end()) {
				visited.insert(d.destId);
				totalDist += d.val;
				if (totalDist < shortestDist) {
					findShortestRecursive(cities[d.destId], path, visited, totalDist + d.val);
				}
				else {
					break;
				}
				
			}
		}
		if (current.dists.size() == 0) {
			shortestDist = shortestDist > totalDist ? totalDist : shortestDist;
			std::cout << "Reached end of path, distance: " << totalDist << std::endl;
			std::stack<uint> path;
			path.push(current.id);
		}
	}

	// allocate memory for a 2d array to store distance values, initialized to max uint
	void initializeDistances() {
		uint maxInt = std::numeric_limits<uint>::max();

		distances = new uint*[cities.size()];
		for (size_t i = 0, ilen = cities.size(); i < ilen; ++i) {
			distances[i] = new uint[ilen];
			for (size_t j = 0; j < ilen; ++j) {
				distances[i][j] = maxInt;
			}
		}
		isInitialized = true;
	}

	// free memory allocated to distances array
	void finalizeDistances() {
		for (size_t i = 0, ilen = cities.size(); i < ilen; ++i) {
			delete[] distances[i];
		}
		delete[] distances;
		isInitialized = false;
	}

	// sorting comparison functions
	static bool sortById(const City& lhs, const City& rhs) {
		return lhs.id < rhs.id;
	}

	static bool sortByDestDistance(const Distance& lhs, const Distance& rhs) {
		return lhs.val < rhs.val;
	}

	uint** distances;
	City* cities;
	size_t count;
	uint shortestDist;
	uint minId;
	uint maxId;
	bool isInitialized;

public:
	TspSolver() {
		// initialize minId and shortestDist to max value
		minId = shortestDist = std::numeric_limits<uint>::max();
		// initialize maxId to smallest value
		maxId = 0;
		isInitialized = false;
	}

	~TspSolver() {
		if (isInitialized) {
			finalizeDistances();
		}
	}

	void findShortest() {
		std::set<uint> visited;
		std::stack<uint> path;
		uint maxThreads = std::thread::hardware_concurrency();
		std::vector<std::thread*> threads;

		maxThreads = maxThreads < 2 ? 2 : maxThreads;

		City start = cities[0];
		visited.insert(start.id);
		while (!start.dists.empty()) {
			while (threads.size() < maxThreads - 1) {
				Distance next = start.dists.top();
				start.dists.pop();
				std::thread* t = new std::thread(&TspSolver::findShortestRecursive, this, cities[next.destId], path, visited, next.val);
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
		std::cout << "Shortest path is " << shortestDist << std::endl;

	}

	std::vector<City> getCities() {
		std::vector<City> updated(cities.size());
		for (size_t i = 0, ilen = cities.size(); i < ilen; ++i) {
			City c;
			c.id = cities[i].id + minId;
			updated.push_back(c);
		}
		return updated;
	}

	void load(const char* path) {
		std::ifstream ifs(path);
		if (ifs) {
			if (isInitialized) {
				finalizeDistances();
			}

			std::string buffer;

			while (std::getline(ifs, buffer)) {
				if (!buffer.empty())
				{
					std::istringstream iss(buffer);
					City c;
					iss >> c.id;
					iss >> c.x;
					iss >> c.y;
					cities.push_back(c);

					maxId = c.id > maxId ? c.id : maxId;
					minId = c.id < minId ? c.id : minId;
				}
			}

			// set IDs to be 0-based if not already
			if (minId > 0) {
				for (size_t i = 0, ilen = cities.size(); i < ilen; ++i) {
					cities[i].id = cities[i].id - minId;
				}
			}

			// sort IDs in ascending order
			std::sort(cities.begin(), cities.end(), sortById);

			// precalculate all distances between cities
			initializeDistances();
			computeDistances();
		}
		else {
			std::cout << "Invalid path specified: " << path << std::endl;
			exit(1);
		}
	}
};

int main(int argc, char** argv) {
	// Check if argument was specified. If not, display usage instructions.
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}

	TspSolver solver;

	// Try to load the data.
	solver.load(argv[1]);

	solver.findShortest();

	std::cout << "Done. Press Enter to continue.";
	std::getchar();
	return 0;
}

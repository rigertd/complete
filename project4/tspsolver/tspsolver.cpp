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

class TspSolver {
private:
	struct Distance {
		unsigned int destId;
		unsigned int val;
	};

	struct City {
		unsigned int id;
		unsigned int x;
		unsigned int y;
		std::vector<Distance> dists;
	};

	void findShortestRecursive(std::vector<City>& notVisited, std::vector<unsigned int>& path, unsigned int& totalDist) {
		if (notVisited.size() > 0) {

		}
		else {
			shortestDist = totalDist;
		}
	}

	void initializeDistances() {
		distances = new unsigned int*[cities.size()];
		for (int i = 0; i < cities.size(); ++i) {
			distances[i] = new unsigned int[cities.size()];
		}
	}

	void finalizeDistances() {
		for (int i = 0; i < cities.size(); ++i) {
			delete[] distances[i];
		}
		delete[] distances;
	}

	static bool sortById(const City& lhs, const City& rhs) {
		return lhs.id < rhs.id;
	}

	static bool sortByDestDistance(const Distance& lhs, const Distance& rhs) {
		return lhs.val < rhs.val;
	}

	unsigned int** distances;
	std::vector<City> cities;
	unsigned int shortestDist;
	unsigned int minId;
	unsigned int maxId;
	std::string path;

public:
	TspSolver() {
		// initialize minId and shortestDist to max value
		minId = shortestDist = std::numeric_limits<unsigned int>::max();
		// initialize maxId to smallest value
		maxId = 0;
	}

	~TspSolver() {
		if (cities.size() > 0) {
			finalizeDistances();
		}
	}

	void computeDistances() {
		unsigned int count = cities.size();
		unsigned int row, col;

		initializeDistances();

		for (int i = 0; i < count; ++i) {
			for (int j = i; j < count; ++j) {
				row = cities[i].id;
				col = cities[j].id;
				if (i != j) {
					distances[row][col] = distances[col][row] = std::sqrt(
						cities[i].x * cities[i].x +
						cities[j].y * cities[j].y);
				}
				else {
					distances[row][col] = 0;
				}
			}
		}

		for (int i = 0; i < count; ++i) {
			for (int j = 0; j < count - 1; ++j) {
				if (i != j) {
					Distance d;
					d.destId = cities[i].id;
					d.val = distances[i][j];
					cities[i].dists.push_back(d);
				}
			}
			std::sort(cities[i].dists.begin(), cities[i].dists.end(), TspSolver::sortByDestDistance);
		}
	}

	void findShortest(std::vector<City>& cities, std::vector<std::vector<unsigned int>> distances) {
		std::stack<unsigned int> waiting;
		std::set<unsigned int> visited;
		std::vector<unsigned int> path;

		waiting.push(cities[0].id);
		while (waiting.size() > 0) {
			for (int i = 1; i < cities.size(); ++i) {
				if (visited.find(cities[i].id) != visited.end()) {
					waiting.push(cities[i].id);

				}
			}
		}


	}

	std::vector<City> getCities() {
		std::vector<City> updated(cities.size());
		for (int i = 0; i < cities.size(); ++i) {
			City c;
			c.id = cities[i].id + minId;
			updated.push_back(c);
		}
		return updated;
	}

	void load(std::string path) {
		std::ifstream ifs(path);

		if (ifs) {
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
				for (int i = 0; i < cities.size(); ++i) {
					cities[i].id = cities[i].id - minId;
				}
			}

			// sort IDs in ascending order
			std::sort(cities.begin(), cities.end(), sortById);
		}
		else {
			throw "Invalid path specified: " + path;
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

	// Check if argument is valid.
	try {
		solver.load(std::string(argv[1]));
	}
	catch (std::string message) {
		std::cout << message << std::endl;
		return 1;
	}

	solver.computeDistances();

	std::getchar();
	return 0;
}

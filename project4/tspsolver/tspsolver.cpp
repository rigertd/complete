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

namespace tspsolver {

struct Distance {
	unsigned int destId;
	unsigned int val;
};

struct City {
	unsigned int id;
	unsigned int x;
	unsigned int y;
	Distance* dists;
};

bool byId(const City& lhs, const City& rhs) {
	return lhs.id < rhs.id;
}

bool byDestDistance(const Distance& lhs, const Distance& rhs) {
	return lhs.val < rhs.val;
}

void initializeDistances(unsigned int**& d, unsigned int count) {
	d = new unsigned int*[count];
	for (int i = 0; i < count; ++i) {
		d[i] = new unsigned int[count];
	}
}

void finalizeDistances(unsigned int**& d, unsigned int count) {
	for (int i = 0; i < count; ++i) {
		delete[] d[i];
	}
	delete[] d;
}

void finalizeCities(std::vector<City>& const c) {
	for (int i = 0; i < c.size(); ++i) {
		delete[] c[i].dists;
	}
}

void computeDistances(std::vector<City>& const c, unsigned int** d) {
	unsigned int count = c.size();
	unsigned int row, col;

	for (int i = 0; i < count; ++i) {
		for (int j = i; j < count; ++j) {
			row = c[i].id;
			col = c[j].id;
			if (i != j) {
				d[row][col] = d[col][row] = std::sqrt(
					c[i].x * c[i].x +
					c[j].y * c[j].y);
			}
			else {
				d[row][col] = 0;
			}
		}
	}

	for (int i = 0; i < count; ++i) {
		c[i].dists = new Distance[count];
		for (int j = 0; j < count - 1; ++j) {
			if (i < j) {
				c[i].dists[j - 1].destId = c[i].id;
				c[i].dists[j - 1].val = d[i][j];
			}
			else if (i > j) {
				c[i].dists[j].destId = c[i].id;
				c[i].dists[j].val = d[i][j];
			}
		}
		std::sort(c[i].dists, c[i].dists + count - 1, byDestDistance);
	}
}

void findShortest(std::vector<City>& cities, unsigned int** distances) {
	std::stack<unsigned int> waiting;
	std::set<unsigned int> visited;
	std::vector<unsigned int> path;
	unsigned int minSoFar = std::numeric_limits<unsigned int>::max();

	waiting.push(cities[0].id);
	while (waiting.size() > 0) {
		for (int i = 1; i < cities.size(); ++i) {
			if (visited.find(cities[i].id) != visited.end()) {
				waiting.push(cities[i].id);

			}
		}
	}


}

void restoreOriginalCityIds(std::vector<City>& cities, unsigned int minId) {
	for (int i = 0; i < cities.size(); ++i) {
		cities[i].id = cities[i].id + minId;
	}
}

void loadData(std::ifstream& ifs, std::vector<City>& cities, unsigned int& minId, unsigned int& maxId) {
	if (ifs) {
		std::string buffer;
		maxId = 0;
		minId = std::numeric_limits<unsigned int>::max();

		while (std::getline(ifs, buffer)) {
			if (!buffer.empty())
			{
				std::istringstream iss(buffer);
				tspsolver::City c;
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
		std::sort(cities.begin(), cities.end(), byId);
	}
}

}

int main(int argc, char** argv) {
	// Check if argument was specified. If not, display usage instructions.
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}

	// Check if argument is valid.
	std::ifstream ifs(argv[1]);
	if (!ifs) {
		std::cout << "Invalid file specified: " << argv[1] << std::endl;
		return 1;
	}

	std::vector<tspsolver::City> cities;
	unsigned int** distances;
	unsigned int maxId, minId, range;

	tspsolver::loadData(ifs, cities, minId, maxId);

	range = maxId - minId + 1;
	tspsolver::initializeDistances(distances, range);
	tspsolver::computeDistances(cities, distances);

	// Find optimal solution here
	for (int i = 0; i < range; ++i) {
		for (int j = 0; j < range - 1; ++j)
			std::cout << cities[i].dists[j].val << " ";
		std::cout << std::endl;
	}
	tspsolver::finalizeDistances(distances, range);
	tspsolver::finalizeCities(cities);
	std::cout << "Done\nminId=" << minId << " maxId=" << maxId << " range=" << range << std::endl;
	std::getchar();
	return 0;
}

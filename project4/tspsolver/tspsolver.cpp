#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <cmath>

namespace tspsolver {

struct City {
	unsigned int id;
	unsigned int x;
	unsigned int y;
};

inline bool operator<(const City& lhs, const City& rhs) {
	return lhs.id < rhs.id;
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

void computeDistances(std::vector<City>& const c, unsigned int** d, unsigned int minId) {
	unsigned int count = c.size();
	unsigned int row, col;

	for (int i = 0; i < count; ++i) {
		for (int j = i; j < count; ++j) {
			row = c[i].id - minId;
			col = c[j].id - minId;
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
	}
}

}

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}

	std::ifstream ifs(argv[1]);
	std::vector<tspsolver::City> cities;
	unsigned int** distances;
	unsigned int maxId, minId, range;

	tspsolver::loadData(ifs, cities, minId, maxId);

	range = maxId - minId + 1;
	tspsolver::initializeDistances(distances, range);
	tspsolver::computeDistances(cities, distances, minId);

	// find optimal solution here

	tspsolver::finalizeDistances(distances, range);
	std::cout << "Done\nminId=" << minId << " maxId=" << maxId << " range=" << range << std::endl;
	std::getchar();
	return 0;
}

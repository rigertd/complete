#include <iostream>
#include <ctime>
#include <chrono>

#include "nn_multi.h"

int main(int argc, char** argv) {
	// Check if argument was specified. If not, display usage instructions.
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}
	auto start = std::chrono::high_resolution_clock::now();
	tsp::load(argv[1]);

	long long remaining = 270 - std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start).count();
	if (remaining < 6) {
		std::cout << "Not enough time to run algorithm." << std::endl;
	}
	uint nnDist;
	// leave 5 seconds for saving
	std::vector<uint> nnTour = tsp::nneighbor::findTourNN(nnDist, remaining - 5);
	std::cout << "Nearest neighbor approximation distance: " << nnDist << std::endl;

	tsp::save(argv[1], nnDist, nnTour);

	tsp::finalize();

	long long runtime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start).count();
	std::cout << "Final runtime: " << runtime / (60 * 60) << " hours, " << runtime / 60 << " minutes, " << runtime % 60 << " seconds\n";

	return 0;
}

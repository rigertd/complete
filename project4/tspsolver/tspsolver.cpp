#include <iostream>

#include "mst_walk.h"
#include "bruteforce.h"

int main(int argc, char** argv) {
	// Check if argument was specified. If not, display usage instructions.
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}
	int linesRead = tsp::load(argv[1]);

	City* root = tsp::mstwalk::findMst(0);
	uint totalDist;
	std::vector<uint> tour = tsp::mstwalk::mstPreorderPath(root, totalDist);
	std::cout << "2-approximation distance: " << totalDist << std::endl;

	uint optimalDist;
	std::vector<uint> optimalTour = tsp::bruteforce::bruteForce(optimalDist, totalDist);
	std::cout << "optimal distance: " << optimalDist << std::endl;

	tsp::save(argv[1], optimalDist, optimalTour);

	tsp::finalize();

	return 0;
}

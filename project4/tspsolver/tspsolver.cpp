#include <iostream>

#include "mst_walk.h"

int main(int argc, char** argv) {
	// Check if argument was specified. If not, display usage instructions.
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}
	tsp::load(argv[1]);

	City* start = tsp::mstwalk::findMst(0);
	uint totalDist;
	std::vector<uint> tour = tsp::mstwalk::mstPreorderPath(start, totalDist);

	tsp::save(argv[1], totalDist, tour);

	tsp::finalize();

	return 0;
}

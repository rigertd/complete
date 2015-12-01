#include <iostream>

#include "mst_walk.h"

int main(int argc, char** argv) {
	// Check if argument was specified. If not, display usage instructions.
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}
	mstwalk::load(argv[1]);

	City* start = mstwalk::findMst();
	uint totalDist;
	std::vector<uint> tour = mstwalk::mstPreorderPath(start, totalDist);

	mstwalk::save(argv[1], totalDist, tour);

	mstwalk::finalize();

	return 0;
}

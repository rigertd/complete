#include <iostream>
#include <ctime>

//#include "mst_walk.h"
//#include "bruteforce.h"
#include "nneighbor.h"

int main(int argc, char** argv) {
	// Check if argument was specified. If not, display usage instructions.
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}
	time_t started = time(0);
	tsp::load(argv[1]);

	//City* root = tsp::mstwalk::findMst(0);
	//uint mstDist;
	//std::vector<uint> mstTour = tsp::mstwalk::mstPreorderPath(root, mstDist);
	//std::cout << "MST 2-approximation distance: " << mstDist << std::endl;

	time_t remaining = 290 - (time(0) - started);
	if (remaining < 6) {
		std::cout << "Not enough time to run algorithm." << std::endl;
	}
	uint nnDist;
	// leave 5 seconds for saving
	std::vector<uint> nnTour = tsp::nneighbor::findTourNN(nnDist, remaining - 5);
	std::cout << "Nearest neighbor approximation distance: " << nnDist << std::endl;

	tsp::save(argv[1], nnDist, nnTour);

	tsp::finalize();

	uint runtime = time(0) - started;
	std::cout << "Final runtime: " << runtime / (60 * 60) << " hours, " << runtime / 60 << " minutes, " << runtime % 60 << " seconds\n";

	return 0;
}

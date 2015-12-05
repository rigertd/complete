#include <iostream>

#include "mst_walk.h"
//#include "bruteforce.h"
#include "nneighbor.h"

int main(int argc, char** argv) {
	// Check if argument was specified. If not, display usage instructions.
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}
	tsp::load(argv[1]);

	City* root = tsp::mstwalk::findMst(0);
	uint mstDist;
	std::vector<uint> mstTour = tsp::mstwalk::mstPreorderPath(root, mstDist);
	std::cout << "MST 2-approximation distance: " << mstDist << std::endl;

	uint nnDist;
	std::vector<uint> nnTour = tsp::nneighbor::findTourNN(nnDist, &tsp::cities[0]);
	std::cout << "Nearest neighbor approximation distance: " << nnDist << std::endl;

	tsp::save(argv[1], nnDist, nnTour);

	tsp::finalize();

	return 0;
}

#include <iostream>
#include <stdlib.h>     //srand, rand
#include <time.h>       //time

#include "mst_walk.h"
//#include "bruteforce.h"

int main(int argc, char** argv) {
	// Check if argument was specified. If not, display usage instructions.
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}
	tsp::load(argv[1]);

	srand (time(NULL));

	//Isaiah to do: Can you find out how to pull the min/max values from tsp::load so that they scale based on the input file?
	int min = 0;
	int max = 75;

	uint lowestDist = 1000000000;
	std::vector<uint> bestTour;

	for(int i = 0; i < 1000; i++)
	{
		int mst_number = min + (rand() % (int)(max - min + 1));

		City* root = tsp::mstwalk::findMst(mst_number);
		uint totalDist;
		std::vector<uint> tour = tsp::mstwalk::mstPreorderPath(root, totalDist);
		std::cout << "2-approximation distance: " << totalDist << std::endl;	
		
		if(totalDist < lowestDist)
		{
			lowestDist = totalDist;
			bestTour = tour;
		}	
	}
	

	//uint optimalDist;
	//std::vector<uint> optimalTour = tsp::bruteforce::bruteForce(optimalDist, totalDist);
	//std::cout << "optimal distance: " << optimalDist << std::endl;

	tsp::save(argv[1], lowestDist, bestTour);
	//tsp::save(argv[1], optimalDist, optimalTour);

	//tsp::finalize();

	return 0;
}

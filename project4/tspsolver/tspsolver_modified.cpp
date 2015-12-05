#include <iostream>
#include <stdlib.h>     //srand, rand
#include <time.h>       //time

#include "mst_walk.h"
#include "common.h"
//#include "bruteforce.h"

int main(int argc, char** argv) {
	// Check if argument was specified. If not, display usage instructions.
	if (argc < 2) {
		std::cout << "Usage:\n  tspsolver.exe <input_file>\n\n";
		return 1;
	}
	tsp::load(argv[1]);
	
	// Determine the number of items read in. 0-X, where zero is the starting item position and X is the last item.
	int idCount = tsp::cities.back().id;

	srand (time(NULL));

	// Determine min "city" count and the max "city" count read in from the file
	int min = 0;
	int max = idCount;
	
	// Determine the number of times the MST loop will run based on the number of nodes
	int maxLoop = 0;
	if (idCount < 300) 
	{
		maxLoop = max;
	}
	else
	{
		maxLoop = max / 10;
	}
	
	std::cout << "MAX LOOP: " << maxLoop << std::endl << std::endl;

	uint lowestDist = 1000000000;
	std::vector<uint> bestTour;
	
	for(int i = 0; i < maxLoop; i++)
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

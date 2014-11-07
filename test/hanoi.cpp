#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>

const int SPIKES = 3;

void hanoi (int disks, int from, int to, int spare, int **towers, int height);
void move (int from, int to, int **towers, int height);
void print_towers(int **towers, int height);

int main (int argc, char** argv) {
	int height;

	// get command-line args
	if (argc < 2) {
		std::cout << "Usage: hanoi <number of disks>" << std::endl;
		std::cout << "Try 'hanoi 5'!" << std::endl;
		return 1;
	}
	else {
		height = std::atoi(argv[1]);
	}

	// towers[spike][position] = size of disk at position
	int **towers;

	// can't make a 2-d array of unknown size,
	// so make an array of pointers first
	towers = new int*[SPIKES];

	// set the size of the inner arrays
	for (int i = 0; i < 3; i++) {
		towers[i] = new int[height];

		// zero them out
		for (int j = 0; j < height; j++) {
			towers[i][j] = 0;
		}
	}

	// put the beginning disks
	for (int i = 0; i < height; i++) {
		towers[0][i] = i + 1;
	}
	std::cout << "Reticulating " << std::pow(2, height) -1 << " splines..."
	          << std::endl << "Done. Press [return] to continue.";
	std::cin.get();
	print_towers (towers, height);
	hanoi(height, 0, 2, 1, towers, height);
	std::cout << std::endl;

	return 0;
}

// recursive wrapper for move()
void hanoi (int disks, int from, int to, int spare, int **towers, int height) {
	if ( disks > 0) {
		hanoi (disks -1, from, spare, to, towers, height);
		move (from, to, towers, height);
		hanoi (disks -1, spare, to, from, towers, height);
	}
}

// move top disk of from to top of to; modifies **towers
// also calls the print function
void move (int from, int to, int **towers, int height) {
	int disk = 0;

	// update state array
	// get top disk at source
	for (int i = 0; i < height; i++) {
		if (towers[from][i]) {
			disk = towers[from][i];
			// remove
			towers[from][i] = 0;
			break;
		}
	}

	// put on destination
	for (int i = height - 1; i >= 0; i--) {
		if (!towers[to][i]) {
			towers[to][i] = disk;
			break;
		}
	}

	std::cout << "Press [return] to continue";
	std::cin.get();

	std::cout << "\nmove from " << from << " to " << to << std::endl;

	print_towers (towers, height);
}

// ascii art representation of tower state
void print_towers(int **towers, int height) {
	// spike positions
	int left = height,
	    mid = 3 * height + 2,
	    right = 5 * height + 3;

	// print frame
	int frame_width = 6 * height + 3;

	// disk sizes
	int disk_size[3];

	// line we're going to print
	std::string line;

	// build up the line
	for (int row = 0; row < height; row++) {

		for (int spike = 0; spike < SPIKES; spike++) {
			disk_size[spike] = towers[spike][row];
		}

		// start empty
		line.assign (frame_width, ' ');
		// disk on first spike
		line.replace (left - disk_size[0], disk_size[0] * 2 + 1,
		              std::string(disk_size[0] * 2 + 1, '#'));
		// disk on second spike
		line.replace (mid - disk_size[1], disk_size[1] * 2 + 1,
		              std::string(disk_size[1] * 2 + 1, '#'));
		// disk on third spike
		line.replace (right - disk_size[2], disk_size[2] * 2 + 1,
		              std::string(disk_size[2] * 2 + 1, '#'));

		std::cout << line << std::endl;
	}
}

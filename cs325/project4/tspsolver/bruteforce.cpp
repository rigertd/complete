/**
 * Implementation of a brute-force algorithm to find an optimal tour of a TSP.
 * Any branch that exceeds the shortest distance found so far is cut.
 * This program uses all available cores minus one, unless only one core is available.
 *
 * Author:		David Rigert
 * Class:		CS 325 Fall 2015
 *				Project Group 1
 * Last Update: 12/2/2015
 */

#include <algorithm>
#if defined(__GNUC__)
// more hackery for GCC's lameness
#include <stdatomic.h>
#else
#include <atomic>
#endif
#if defined(__linux__)
#include <unistd.h>
#elif defined(_WIN32)
#include <Windows.h>
#endif
#include <iostream>
#include <mutex>
#include <set>
#include <stack>
#include <thread>
#include <queue>

#include "bruteforce.h"
#include "PriorityQueue.h"

template <>
bool Comparator<>(const City& lhs, const City& rhs) {
	return lhs.key > rhs.key;
}

namespace tsp { namespace bruteforce {

// namespace globals
uint shortestSoFar = MAX_UNSIGNED;				// stores the shortest path found so far
std::vector<uint> shortestPath;
std::mutex guardShortest;

// Recursive part of brute force algorithm.
void bruteForceRecursive(City& current, std::vector<City>& unvisited, uint distToCurrent, std::vector<uint>& path, uint shorterThan, std::atomic<bool>* done) {
	path.push_back(current.id);
	if (unvisited.empty()) {
		uint totalDistance = distToCurrent + getDistance(&current, &(cities[0]));
		guardShortest.lock();
		if (totalDistance < shortestSoFar && totalDistance <= shorterThan) {
			shortestSoFar = totalDistance;
			shortestPath = path;
			std::cout << "New shortest distance found: " << shortestSoFar << std::endl;
		}
		guardShortest.unlock();
	}
	else {
        int ilen = static_cast<int>(unvisited.size());
        uint distToNext;
		for (int i = ilen - 1; i >= 0; --i) {
            distToNext = distToCurrent + unvisited[i].key;
			if (distToNext < shortestSoFar && distToNext < shorterThan) {
				std::vector<City> nextUnvisited;
				City next;
				for (int j = 0; j < ilen; ++j) {
					if (j != i) {
						next = unvisited[j];
						next.key = getDistance(&unvisited[i], &unvisited[j]);
						nextUnvisited.push_back(next);
					}
				}
				std::sort(unvisited.begin(), unvisited.end(), Comparator<City>);

				bruteForceRecursive(unvisited[i], nextUnvisited, distToNext, path, shorterThan, done);
			}
		}
	}
	path.pop_back();
	if (path.size() == 1) {
		done->store(true);
	}
}

std::vector<uint> bruteForce(uint& totalDistance, uint shorterThan) {
    shortestPath.clear();
	if (cities.empty()) {
		return shortestPath;
	}

#if defined(__GNUC__)
	// GCC does not support the hardware_concurrency function.
    uint threadCount = sysconf (_SC_NPROCESSORS_ONLN);
#else
    uint threadCount = std::thread::hardware_concurrency();
#endif
	threadCount = threadCount > 1 ? threadCount - 1 : 1;
    std::vector<std::thread*> threads;
	std::vector<std::atomic<bool>*> flags;
    
	std::vector<City> unvisited;
	size_t total = cities.size();
	cities[0].key = 0;
	for (size_t i = 1; i < total; ++i) {
		cities[i].key = getDistance(&cities[0], &cities[i]);
		unvisited.push_back(cities[i]);
	}

	std::sort(unvisited.begin(), unvisited.end(), Comparator<City>);
	size_t j = unvisited.size();
	while (j > 0) {
		// create as many threads as there are cores available, minus one
		if (threads.size() < threadCount && j > 0) {
			--j; // decrement before use for 0-based subscripts
			std::vector<uint> path;
			path.push_back(0);

			std::vector<City> nextUnvisited;
			City next;
			for (size_t i = 0, ilen = unvisited.size(); i < ilen; ++i) {
				if (i != j) {
					next = unvisited[i];
					next.key = getDistance(&unvisited[j], &unvisited[i]);
					nextUnvisited.push_back(next);
				}
			}
			std::sort(nextUnvisited.begin(), nextUnvisited.end(), Comparator<City>);
			std::cout << "Creating new thread" << std::endl;
			flags.push_back(new std::atomic<bool>(false));
            threads.push_back(new std::thread(bruteForceRecursive, unvisited[j], nextUnvisited, unvisited[j].key, path, shorterThan, flags.back()));
		}
		// join each thread after it finishes its task. 
		for (size_t i = 0, ilen = threads.size(); i < ilen; ++i) {
            if (flags[i]->load()) {
				std::cout << "Joining thread " << i << std::endl;
				if (threads[i]->joinable()) {
					threads[i]->join();
					delete threads[i];
					threads.erase(threads.begin() + i);
					delete flags[i];
					flags.erase(flags.begin() + i);
					--i;
				}
			}
		}
#if defined(__linux__)
		sleep(1);
#elif defined(_WIN32)
		Sleep(1000);
#endif
	}

	// wait for remaining threads
	while (threads.size() > 0) {
		if (threads[0]->joinable()) {
			threads[0]->join();
			delete threads[0];
			threads.erase(threads.begin());
			delete flags[0];
			flags.erase(flags.begin());
		}
	}
	totalDistance = shortestSoFar;
	return shortestPath;
}

} }


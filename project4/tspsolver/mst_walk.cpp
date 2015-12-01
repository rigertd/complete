#include <stack>

#include "mst_walk.h"
#include "PriorityQueue.h"

template <>
bool Comparator<>(const cityptr& lhs, const cityptr& rhs) {
	return lhs->key > rhs->key;
}

namespace tsp { namespace mstwalk {

City* findMst(uint startIndex) {
	PriorityQueue<City*> unvisited;

	// set starting node
	cities[startIndex].key = 0;
	cities[startIndex].parent = nullptr;
	unvisited.push(&cities[startIndex]);

	// add remaining nodes to unvisited
	for (size_t i = 0, ilen = cities.size(); i < ilen; ++i) {
		if (i == startIndex)
			continue;
		
		cities[i].key = MAX_UNSIGNED;
		cities[i].parent = nullptr;
		cities[i].children.clear();
		unvisited.push(&cities[i]);
	}

	while (!unvisited.empty()) {
		City* current = unvisited.top();
		unvisited.pop();

		// add as child of parent node
		if (current->parent != nullptr) {
			current->parent->children.push_back(current);
		}

		// multithreading takes longer, so stay with single threaded.
		for (auto it = unvisited.begin(), end = unvisited.end(); it != end; ++it) {
			if (getDistance(current, *it) < (*it)->key) {
				(*it)->parent = current;
				(*it)->key = getDistance(current, *it);
			}
		}
		unvisited.rebuild_heap();
	}
	return &cities[startIndex];
}

std::vector<uint> mstPreorderPath(City* start, uint& totalDistance) {
	std::vector<uint> tour;
	std::stack<City*> waiting;
	City* current = nullptr;
	City* previous = nullptr;
	totalDistance = 0;

	waiting.push(start);
	while (!waiting.empty()) {
		current = waiting.top();
		waiting.pop();

		// pre-order
		tour.push_back(current->id);
		if (previous != nullptr) {
			totalDistance += getDistance(previous, current);
		}

		for (size_t i = 0, ilen = current->children.size(); i < ilen; ++i) {
			waiting.push(current->children[i]);
		}
		previous = current;
	}
	totalDistance += getDistance(current, start);
	return tour;
}

} }
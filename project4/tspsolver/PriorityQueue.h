#pragma once

#include <vector>
#include <queue>
#include "City.h"

bool CityComparator(const City* lhs, const City* rhs) {
	return lhs->key > rhs->key;
}

/**
 * Wraps a std::vector<City*> with priority queue functionality.
 * This implementation provides additional features to the STL priority_queue.
 * Namely, it supports rebuilding the heap after decrease-key operations.
 */
struct PriorityQueue {
private:
	std::vector<City*> _data;

public:
	bool empty() {
		return _data.empty();
	}

	void pop() {
		std::pop_heap(_data.begin(), _data.end(), CityComparator);
		_data.pop_back();
	}

	void push(City* value) {
		_data.push_back(value);
		std::push_heap(_data.begin(), _data.end(), CityComparator);
	}

	void rebuild_heap() {
		std::make_heap(_data.begin(), _data.end(), CityComparator);
	}

	City* top() {
		return _data.front();
	}

	std::vector<City*>::iterator begin() {
		return _data.begin();
	}

	std::vector<City*>::iterator end() {
		return _data.end();
	}

	City* operator[](uint index) {
		return _data[index];
	}

	uint size() {
		return _data.size();
	}
};

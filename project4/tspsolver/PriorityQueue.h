#pragma once

#include <vector>
#include <queue>
#include "City.h"

template <typename T>
bool Comparator(const T& lhs, const T& rhs) {
	return lhs > rhs;
}

/**
 * Wraps a std::vector<City*> with priority queue functionality.
 * This implementation provides additional features to the STL priority_queue.
 * Namely, it supports rebuilding the heap after decrease-key operations.
 */
template <typename T>
struct PriorityQueue {
private:
	std::vector<T> _data;

public:
	bool empty() {
		return _data.empty();
	}

	void pop() {
		std::pop_heap(_data.begin(), _data.end(), Comparator<T>);
		_data.pop_back();
	}

	void push(T value) {
		_data.push_back(value);
		std::push_heap(_data.begin(), _data.end(), Comparator<T>);
	}

	void rebuild_heap() {
		std::make_heap(_data.begin(), _data.end(), Comparator<T>);
	}

	T top() {
		return _data.front();
	}

	typename std::vector<T>::iterator begin() {
		return _data.begin();
	}

	typename std::vector<T>::iterator end() {
		return _data.end();
	}

	T operator[](uint index) {
		return _data[index];
	}

	uint size() {
		return _data.size();
	}
};

//
// Created by Vova on 05.09.2020.
//

#pragma once

/// ********************************************* Memory management: *********************************************


template < class T >
void graph_matrix<T>::fill_matrix (const T &element)
{
	for (size_t i = 0; i < n; ++i) {
		std::fill(std::execution::par_unseq, data[i], data[i] + n, element);
	}
}

template < class T >
void graph_matrix<T>::alloc (size_t size)
{
	n = size;

	data = new T* [n];

	for (size_t i = 0; i < n; i++) {
		data[i] = new T[n];
	}
}

template < class T >
void graph_matrix<T>::alloc_and_fill (size_t size, const T &element)
{
	alloc(size);
	fill_matrix(element);
}

template < class T >
void graph_matrix<T>::dealloc ()
{
	if (!data) {
		n = 0;
		return;
	}

	for (size_t i = 0; i < n; ++i) {
		delete [] data[i];
		data[i] = nullptr;
	}
	delete data;
	data = nullptr;

	n = 0;
}


/// _______________________________________________ Moving / copying / freeing OOP stuff: _______________________________________________


template < class T >
graph_matrix<T>::~graph_matrix ()
{
	dealloc();
}

template < class T >
graph_matrix<T>::graph_matrix (const graph_matrix &other)
{
	alloc(other.size);

	for (size_t i = 0; i < n; ++i) {
		std::copy(std::execution::par_unseq, other.data[i], other.data[i] + n, this->data[i]);
	}
}

template < class T >
graph_matrix<T>::graph_matrix (graph_matrix &&other) noexcept
{
	// Delete old data from "this":

	// Copy:

}

template < class T >
graph_matrix<T> &graph_matrix<T>::operator= (const graph_matrix &other)
{
	if (&other != this) {
		dealloc();

		alloc(n);

		// Actual copying:
		for (size_t i = 0; i < n; ++i) {
			std::copy(std::execution::par_unseq, other.data[i], other.data[i] + n, this->data[i]);
		}
	}

	return *this;
}

template < class T >
graph_matrix<T> &graph_matrix<T>::operator= (graph_matrix &&other) noexcept
{
	if (this != &other) {
		// Deallocate previous memory:
		dealloc();

		// "Steel" other stack data:
		n = other.n;
		data = other.data;

		// Prepare "other" to be safely deleted:
		other.data = nullptr;
		other.n = 0;
	}

	return *this;
}




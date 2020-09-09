//
// Created by Vova on 05.09.2020.
//

#pragma once

/// ********************************************* Memory management: *********************************************


#include "graph_matrix.h"

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

/// ********************************************* Constructors: *********************************************

template < class T >
graph_matrix<T>::graph_matrix (size_t size) : n(size) {
	// Allocate the array and initialize with the default value of type T:
	alloc_and_fill(size);
}


/// ********************************************* Moving / copying / freeing OOP stuff: *********************************************


template < class T >
graph_matrix<T>::~graph_matrix ()
{
	std::cout << "Deleting matrix..." << std::endl;

	dealloc();
}

template < class T >
graph_matrix<T>::graph_matrix (const graph_matrix &other)
{
	// std::cout << "Matrix copy constructor..." << std::endl;

	alloc(other.n);

	for (size_t i = 0; i < n; ++i) {
		std::copy(std::execution::par_unseq, other.data[i], other.data[i] + n, this->data[i]);
	}
}

template < class T >
graph_matrix<T>::graph_matrix (graph_matrix &&other) noexcept
{
	// std::cout << "Matrix move constructor..." << std::endl;

	data = other.data;
	n = other.n;

	other.data = nullptr;
	other.n = 0;
}

template < class T >
graph_matrix<T> &graph_matrix<T>::operator= (const graph_matrix &other)
{
	// std::cout << "Matrix copy operator..." << std::endl;

	if (&other != this) {
		dealloc();

		alloc(other.n);

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
	// std::cout << "Matrix move operator..." << std::endl;

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


/// ********************************************* Memory management API for user: *********************************************

template < class T >
void graph_matrix<T>::resize (size_t new_size)
{
	dealloc();
	alloc_and_fill(new_size);
}

template < class T >
void graph_matrix<T>::release ()
{
	dealloc();
}

template < class T >
void graph_matrix<T>::fill_default ()
{
	fill_matrix({});
}

/// ********************************************* Edge access for user: *********************************************


template < class T >
T &graph_matrix<T>::get_edge (size_t from, size_t to) const
{
	assert(from < n);
	assert(to < n);

	return const_cast<T&>(data[from][to]);
}

template < class T >
void graph_matrix<T>::set_directed_edge (size_t from, size_t to, const T &value)
{
	assert(from < n);
	assert(to < n);

	data[from][to] = value;
}

template < class T >
void graph_matrix<T>::set_bidirectional_edge (size_t from, size_t to, const T &value)
{
	assert(from < n);
	assert(to < n);

	data[from][to] = value;
	data[to][from] = value;
}


/**
 * Applies the functor to all the vertexes to which it`s possible to come from the given vertex THROUGH one edge
 * @param functor: the functor to apply
 * @param vertex_index: the initial vertex index
 */
template <class T>
template <class Functor>
void graph_matrix<T>::for_vertex_children (size_t vertex_index, const Functor &functor)
{
	for (size_t target_index = 0; target_index < n; ++target_index) {
		if (data[vertex_index][target_index]) {
			functor(target_index);
		}
	}
}


template <class T>
std::vector<size_t> graph_matrix<T>::get_vertex_children (size_t starting_vertex_index)
{
	std::vector<size_t> res;
	res.reserve(n);

	// for_vertex_children(starting_vertex_index, [&](size_t index){ res.push_back(index); });
	for (size_t target_index = 0; target_index < n; ++target_index) {
		if (data[starting_vertex_index][target_index]) {
			res.push_back(target_index);
		}
	}

	return res;
}



template <class T>
template <class Functor>
void graph_matrix<T>::for_vertex_parents (size_t vertex_index, const Functor &functor)
{
	for (size_t target_index = 0; target_index < n; ++target_index) {
		if (data[target_index][vertex_index]) {
			functor(target_index);
		}
	}
}

template <class T>
std::vector<size_t> graph_matrix<T>::get_vertex_parents (size_t starting_vertex_index)
{
	std::vector<size_t> res;
	res.reserve(n);

	// for_vertex_parents(starting_vertex_index, [&](size_t index){ res.push_back(index); });
	for (size_t target_index = 0; target_index < n; ++target_index) {
		if (data[target_index][starting_vertex_index]) {
			res.push_back(target_index);
		}
	}

	return res;
}


/// ********************************************* Graph outputting: *********************************************


template<class Type>
std::ostream& operator<< (std::ostream &os, const graph_matrix<Type> &graph)
{
	constexpr const char* separator = "__________________________________";
	os << separator << std::endl;
	os << "Adjustment matrix graph representation <" << graph.n << ">:" << std::endl;

	// Determine if it has big digits:
	bool has_big_digits = false;

	if constexpr (std::is_integral_v<Type> && !std::is_same_v<bool, Type>) {
		for (size_t i = 0; i < graph.n; i++) {
			bool is_broken = false;
			for (size_t j = 0; j < graph.n; j++) {
				if (graph.get_edge(i, j) > 99 || graph.get_edge(i, j) < -9) has_big_digits = true;
				is_broken = true;
				break;
			}
			if (is_broken) break;
		}
	}

	if constexpr (std::is_floating_point_v<Type>) {
		os << std::setprecision(2);
	}

	// std::cout << has_big_digits << std::endl;

	// The actual matrix output:
	for (size_t i = 0; i < graph.n; i++) {
		for (size_t j = 0; j < graph.n; j++) {
			// For bool:
			if constexpr (std::is_same_v<bool, Type>) {
				os << (graph.get_edge(j, i) ? "1" : "0");
				if (j != graph.n - 1) os << " ";
			}

				// For non-bools:
			else {
				os << graph.get_edge(j, i);

				// For little-size stuff: numbers from [-9, 99]
				if (std::is_integral_v<Type> && !has_big_digits) {
					if (j != graph.n - 1) os << " ";
				}

				else {
					if (j != graph.n - 1) os << "\t";
				}
			}
		}
		os << std::endl;
	}

	os << separator << std::endl;

	return os;
}

/// ********************************************* Graph inputting from parsed data: *********************************************


template < class T >
template < class input_directionality_type, class Vertex_indexing_type >
void
graph_matrix<T>::add_edges_from_list (const std::vector<std::pair<Vertex_indexing_type, Vertex_indexing_type>> &edges)
{
	static_assert(
			std::is_same_v<input_directionality_type, input_bidirectional> || std::is_same_v<input_directionality_type, input_one_directional>,
			"input_directionality_type should be one of: input_bidirectional, input_one_directional"
	);

	static_assert(std::is_integral_v<Vertex_indexing_type>, "Vertex indexing type should be an integral type");
	static_assert(std::is_same_v<T, bool> , "T (graph template parameter) should bool to use this input mode");

	// TODO: make filling function
	// TODO: split into 2 functions: fill and adding edges from edge list
	for (auto& p : edges) {
		auto[i, j] = p;

		data[i][j] =

		if constexpr (std::is_same_v<input_directionality_type, input_bidirectional>) {
			data[j][i] = data[i][j];
		}
	}
}

template <class T>
template <class input_directionality_type, class Vertex_indexing_type>
void graph_matrix<T>::update_from_edge_list (const std::vector<std::pair<Vertex_indexing_type, Vertex_indexing_type>> &edges)
{
	/// Reset all edges and add the ones from the list:
	fill_default();
	add_edges_from_list<input_directionality_type>(edges);
}




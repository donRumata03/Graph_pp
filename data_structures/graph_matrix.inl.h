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
	// std::cout << "Deleting matrix..." << std::endl;

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

	/*
	else {
		std::cout << "self!" << std::endl;
	}
	*/

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

	/*
	else {
		std::cout << "self!" << std::endl;
	}
	*/

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

template<class Type,
		class OutputStream,
		typename std::enable_if_t<std::is_same_v<OutputStream, std::ostream> || std::is_same_v<OutputStream, std::stringstream>, int>* = nullptr
>
OutputStream& operator<< (OutputStream &os, const graph_matrix<Type> &graph)
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
				// if (graph.get_edge(i, j) > 99 || graph.get_edge(i, j) < -9) {
				if (graph.get_edge(i, j) > 9 || graph.get_edge(i, j) < 0) {
					has_big_digits = true;
					is_broken = true;
					break;
				}
			}
			if (is_broken) break;
		}
	}

	if constexpr (std::is_floating_point_v<Type>) {
		os << std::setprecision(2);
		has_big_digits = true;
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
template <class Vertex_indexing_type >
void
graph_matrix<T>::add_edges_from_list (edge_adding_modes directionality,
                                      const std::vector<std::pair<Vertex_indexing_type, Vertex_indexing_type>> &edges)
{
	/*
	static_assert(
			std::is_same_v<input_directionality_type, input_bidirectional> || std::is_same_v<input_directionality_type, input_one_directional>,
			"input_directionality_type should be one of: input_bidirectional, input_one_directional"
	);
	*/

	static_assert(std::is_integral_v<Vertex_indexing_type>, "Vertex indexing type should be an integral type");
	static_assert(std::is_same_v<T, bool> , "T (graph template parameter) should bool to use this input mode");

	for (auto& p : edges) {
		auto[i, j] = p;

		data[i][j] = true;

		/*
		if constexpr (std::is_same_v<input_directionality_type, input_bidirectional>) {
				data[j][i] = data[i][j];
			}
		*/

		if (directionality == edge_adding_modes::bidirectional) {
			data[j][i] = true;
		}
	}
}

template <class T>
template </*class input_directionality_type, */class Vertex_indexing_type>
void graph_matrix<T>::update_from_edge_list (edge_adding_modes directionality, const std::vector<std::pair<Vertex_indexing_type, Vertex_indexing_type>> &edges)
{
	/// Reset all edges and add the ones from the list:
	fill_default();
	add_edges_from_list(directionality, edges);
}



////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
template <class Vertex_indexing_type>
void graph_matrix<T>::add_edges_from_list (graph_matrix::edge_adding_modes directionality,
                                           const std::vector<std::tuple<Vertex_indexing_type, Vertex_indexing_type, T>> &edges)
{
	static_assert(std::is_integral_v<Vertex_indexing_type>, "Vertex indexing type should be an integral type");
	static_assert(!std::is_same_v<T, bool> , "T (graph template parameter) shouldn`t be bool to use this edge adding mode");

	for (auto& p : edges) {
		auto[i, j, value] = p;

		data[i][j] = value;

		/*
		if constexpr (std::is_same_v<input_directionality_type, input_bidirectional>) {
				data[j][i] = data[i][j];
			}
		*/

		if (directionality == edge_adding_modes::bidirectional) {
			data[j][i] = value;
		}
	}
}

template <class T>
template <class Vertex_indexing_type>
void graph_matrix<T>::update_from_edge_list (graph_matrix::edge_adding_modes directionality,
                                             const std::vector<std::tuple<Vertex_indexing_type, Vertex_indexing_type, T>> &edges)
{
	/// Reset all edges and add the ones from the list:

	fill_default();
	add_edges_from_list(directionality, edges);
}


/// ********************************************* Graph inputting from streams: *********************************************

template <class T>
template<
		class InputStream,
		typename std::enable_if_t<std::is_same_v<InputStream, std::istream> || std::is_same_v<InputStream, std::stringstream>, int>*
>
void graph_matrix<T>::input_from_matrix (InputStream &is)
{
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			is >> data[j][i];
		}
	}
}

template <class T>
void graph_matrix<T>::input_from_matrix (const std::string &source_string)
{
	std::stringstream stream;
	stream << source_string;

	input_from_matrix(stream);
}


template <class T,
        class InputStream,
		typename std::enable_if_t<std::is_same_v<InputStream, std::istream> || std::is_same_v<InputStream, std::stringstream>, int>*
>
InputStream &operator>> (InputStream &is, graph_matrix<T> &graph)
{
//		T temp;
//		for (size_t i = 0; i < n; ++i) {
//			for (size_t j = 0; j < n; ++j) {
//				cin >> temp;
//				data[i][j] = temp;
//			}
//		}
	graph.input_from_matrix(is);
	return is;
}

template <class T>
template <
		class InputStream,
		typename std::enable_if_t<std::is_same_v<InputStream, std::istream> || std::is_same_v<InputStream, std::stringstream>, int>*
>
void graph_matrix<T>::add_edges_from_list (graph_matrix::edge_adding_modes directionality, size_t edge_list_size,
                                           InputStream &input_stream)
{
	if constexpr (std::is_same_v<T, bool>) {
		std::vector<std::pair<size_t, size_t>> edges(edge_list_size);

		for (auto &p : edges) {
			input_stream >> p.first >> p.second;
		}

		add_edges_from_list(directionality, edges);
	}
	else {
		std::vector<std::tuple<size_t, size_t, T>> edges(edge_list_size);

		for (auto &p : edges) {
			input_stream >> std::get<0>(p) >> std::get<1>(p) >> std::get<2>(p);
		}

		add_edges_from_list(directionality, edges);
	}
}

template <class T>
void graph_matrix<T>::add_edges_from_list (graph_matrix::edge_adding_modes directionality, size_t edge_list_size,
                                           const std::string &char_source)
{
	std::stringstream stream;
	stream << char_source;

	return add_edges_from_list(directionality, edge_list_size, stream);
}

template <class T>
template <
        class InputStream,
		typename std::enable_if_t<std::is_same_v<InputStream, std::istream> || std::is_same_v<InputStream, std::stringstream>, int>*
>
void graph_matrix<T>::update_from_edge_list (graph_matrix::edge_adding_modes directionality, size_t edge_list_size,
                                             InputStream &input_stream)
{
	/// Reset all edges and add the ones from the list:
	fill_default();
	add_edges_from_list(directionality, edge_list_size, input_stream);
}

template <class T>
void graph_matrix<T>::update_from_edge_list (graph_matrix::edge_adding_modes directionality, size_t edge_list_size,
                                             const std::string &char_source)
{
	std::stringstream stream;
	stream << char_source;

	return update_from_edge_list(directionality, edge_list_size, stream);
}

template <class T>
void graph_matrix<T>::make_all_edges_bidirectional ()
{
	static_assert(std::is_same_v<T, bool>, "the type should be bool");

	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j <= i; ++j) {
			bool has_some_edge = data[i][j] || data[j][i];
			data[i][j] = has_some_edge;
			data[j][i] = has_some_edge;
		}
	}
}




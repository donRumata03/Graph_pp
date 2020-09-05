//
// Created by Vova on 04.09.2020.
//

#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <execution>
#include <ostream>

template <class T>
struct graph_matrix {
	/// Data:
	T** data = nullptr;
	size_t n = 0;

	/// Constructing:
	explicit graph_matrix(size_t size) : n(size) {
		// Allocate the array and initialize with false:
		data = new T* [n];

		for (size_t i = 0; i < n; i++) {
			data[i] = new T [n];
			std::fill(std::execution::par_unseq, data[i], data[i] + n, T{});
		}
	}

	/// Moving / copying / freeing stuff:
	~graph_matrix() {
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

	// Copy constructor:
	graph_matrix(const graph_matrix& other) {
		// Delete old data from "this":

		// Copy:

	}

	// Move constructor:
	graph_matrix(graph_matrix&& other)  noexcept {
		// Delete old data from "this":

		// Copy:

	}




	[[nodiscard]] T& get_edge(size_t from, size_t to) const
	{
		assert(from < n);
		assert(to < n);

		return const_cast<T&>(data[from][to]);
	}

	void set_directed_edge(size_t from, size_t to, const T& value) {
		assert(from < n);
		assert(to < n);

		data[from][to] = value;
	}

	void set_bidirectional_edge(size_t from, size_t to, const T& value) {
		assert(from < n);
		assert(to < n);

		data[from][to] = value;
		data[to][from] = value;
	}

	/// Operator []:
	template<class Type>
	struct col_t {
		Type* data;
		size_t size;

		Type& operator [](size_t index) {
			assert(index < size);

			return data[index];
		};
	};

	col_t<T> operator[] (size_t index) {
		return col_t<T> { data[index], n };
	}

	friend std::ostream &operator<< (std::ostream &os, const graph_matrix &graph)
	{
		constexpr const char* separator = "__________________________________";
		os << separator << std::endl;
		os << "Adjustment matrix graph representation <" << graph.n << ">:" << std::endl;

		// Determine if it has big digits:
		bool has_big_digits = false;

		if constexpr (std::is_integral_v<T> && !std::is_same_v<bool, T>) {
			for (size_t i = 0; i < graph.n; i++) {
				bool is_broken = false;
				for (size_t j = 0; j < graph.n; j++) {
					if (graph.get_edge(i, j) > 9 || graph.get_edge(i, j) < 0) has_big_digits = true;
					is_broken = true;
					break;
				}
				if (is_broken) break;
			}
		}

		// std::cout << has_big_digits << std::endl;

		// The actual matrix output:
		for (size_t i = 0; i < graph.n; i++) {
			for (size_t j = 0; j < graph.n; j++) {
				// For bool:
				if constexpr (std::is_same_v<bool, T>) {
					os << (graph.get_edge(j, i) ? "1" : "0");
					if (j != graph.n - 1) os << " ";
				}

				// For others:
				else {
					os << graph.get_edge(j, i);
					if (std::is_integral_v<T> && !has_big_digits) {
						if (j != graph.n - 1) os << " ";
					}
					else if (j != graph.n - 1) os << "\t";
				}
			}
			os << std::endl;
		}

		os << separator << std::endl;

		return os;
	}
	// friend std::ostream& operator << (std::ostream& os, const adj_matrix_graph& graph);


	/// Graph inputing:
	struct input_bidirectional {};
	struct input_one_directional {};


	template<class input_directionality_type>
	void input_from_matrix(std::istream& is) {
		// T temp_element;
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < n; ++j) {
				// cin >> temp_element;
				// data[i][j] = temp_element;
				is >> data[i][j];
			}
		}
	}

	template<class input_directionality_type, class Vertex_indexing_type>
	void update_from_edge_list(const std::vector<std::pair<Vertex_indexing_type, Vertex_indexing_type>>& edges) {
		static_assert(
				std::is_same_v<input_directionality_type, input_bidirectional> || std::is_same_v<input_directionality_type, input_one_directional>,
				"input_directionality_type should be one of: input_bidirectional, input_one_directional"
		);

		static_assert(std::is_integral_v<Vertex_indexing_type>, "Vertex indexing type should be an integral type");
		static_assert(std::is_same_v<T, bool> , "T (graph template parameter) should bool to use this input mode");

		// TODO: make filling function
		for (auto& p : edges) {
			auto[i, j] = p;

			data[]

			if constexpr (std::is_same_v<input_directionality_type, input_bidirectional>) {
				data[j][i] = data[i][j];
			}
		}
	}


	std::istream& operator >> (std::istream& os) {
//		T temp;
//		for (size_t i = 0; i < n; ++i) {
//			for (size_t j = 0; j < n; ++j) {
//				cin >> temp;
//				data[i][j] = temp;
//			}
//		}
		input_from_matrix(os);
		return os;
	}
};

using adj_matrix_graph =  graph_matrix<bool>;

using max_int_weighted_matrix_graph = graph_matrix<std::intmax_t>;
using max_uint_weighted_matrix_graph = graph_matrix<std::uintmax_t>;

using double_weighted_matrix_graph = graph_matrix<double>;

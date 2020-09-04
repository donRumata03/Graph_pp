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
struct adj_matrix_graph {
	T** data = nullptr;
	size_t n = 0;

	explicit adj_matrix_graph(size_t size) : n(size) {
		// Allocate the array and initialize with false:
		data = new T* [n];

		for (size_t i = 0; i < n; i++) {
			data[i] = new T [n];
			std::fill(std::execution::par_unseq, data[i], data[i] + n, T{});
		}
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

	friend std::ostream &operator<< (std::ostream &os, const adj_matrix_graph &graph)
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
					if (graph.get_edge(i, j) > 9 || graph.get_edge(i, j) < 0) {
						has_big_digits = true;
						is_broken = true;
						break;
					}
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

	// TODO: add stream input operator!!!
};

using non_weighted_adj_matrix_graph =  adj_matrix_graph<bool>;

using max_int_adj_matrix_graph = adj_matrix_graph<std::intmax_t>;
using max_uint_adj_matrix_graph = adj_matrix_graph<std::uintmax_t>;

using double_adj_matrix_graph = adj_matrix_graph<double>;

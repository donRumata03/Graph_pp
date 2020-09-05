//
// Created by Vova on 05.09.2020.
//

#pragma once

#include "graph_base.h"

#include <iomanip>

template <class T>
struct graph_matrix {
	static_assert(
			std::is_integral_v<T> || std::is_floating_point_v<T>,
			"this class only integral and floating point template parameters"
			);

	/// Data:
	T** data = nullptr;
	size_t n = 0;

	/// Memory management:
private:
	void fill_matrix (const T& element = T{});
	void alloc (size_t size);
	void alloc_and_fill (size_t size, const T& element = T{});
	void dealloc ();

public:
	/// Constructing:
	graph_matrix() = default;

	explicit graph_matrix(size_t size) : n(size) {
		// Allocate the array and initialize with the default value of type T:
		alloc_and_fill(size);
	}

	/// _______________________________________________ Moving / copying / freeing OOP stuff: _______________________________________________

	// Destructor:
	~graph_matrix();

	// Copy constructor:
	graph_matrix(const graph_matrix& other);
	// Move constructor:
	graph_matrix(graph_matrix&& other)  noexcept;

	// Copy assignment operator:
	graph_matrix& operator = (const graph_matrix& other);
	// Move assignment operator:
	graph_matrix& operator = (graph_matrix&& other) noexcept;

	/// _______________________________________________ Resizing: _______________________________________________
	void resize(size_t new_size) {
		dealloc();
		alloc_and_fill(new_size);
	}
	void release() {
		dealloc();
	}


	/// _______________________________________________ Edge access: _______________________________________________

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

	/// TODO: add functions to list all the vertices from which there is a edge

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
					if (graph.get_edge(i, j) > 99 || graph.get_edge(i, j) < -9) has_big_digits = true;
					is_broken = true;
					break;
				}
				if (is_broken) break;
			}
		}

		if constexpr (std::is_floating_point_v<T>) {
			os << std::setprecision(2);
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

				// For non-bools:
				else {
					os << graph.get_edge(j, i);

					// For little-size stuff: numbers from [-9, 99]
					if (std::is_integral_v<T> && !has_big_digits) {
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
	// friend std::ostream& operator << (std::ostream& os, const adj_matrix_graph& graph);


	/// _______________________________________________ Graph inputting: _______________________________________________
	struct input_bidirectional {};
	struct input_one_directional {};


	void input_from_matrix(std::istream& is) {
		// T temp_element;
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < n; ++j) {
				// cin >> temp_element;
				// data[i][j] = temp_element;
				is >> data[j][i];
			}
		}
	}

	template<class input_directionality_type, class Vertex_indexing_type>
	void update_from_edge_list(const std::vector<std::pair<Vertex_indexing_type, Vertex_indexing_type>>& edges) {
		/// Reset all edges and

		static_assert(
				std::is_same_v<input_directionality_type, input_bidirectional> || std::is_same_v<input_directionality_type, input_one_directional>,
				"input_directionality_type should be one of: input_bidirectional, input_one_directional"
		);

		static_assert(std::is_integral_v<Vertex_indexing_type>, "Vertex indexing type should be an integral type");
		static_assert(std::is_same_v<T, bool> , "T (graph template parameter) should bool to use this input mode");

		// TODO: make filling function
		for (auto& p : edges) {
			auto[i, j] = p;

			data[i][j] =

			if constexpr (std::is_same_v<input_directionality_type, input_bidirectional>) {
				data[j][i] = data[i][j];
			}
		}
	}


	friend std::istream& operator >> (std::istream& is, graph_matrix<T>& graph) {
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
};


using adj_matrix_graph =  graph_matrix<bool>;

using max_int_weighted_matrix_graph = graph_matrix<std::intmax_t>;
using max_uint_weighted_matrix_graph = graph_matrix<std::uintmax_t>;

using double_weighted_matrix_graph = graph_matrix<double>;

#include "graph_matrix.inl.h"

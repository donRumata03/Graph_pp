//
// Created by Vova on 05.09.2020.
//

#pragma once

#include "graph_base.h"


template <class T>
struct graph_matrix {
	static_assert(
			std::is_integral_v<T> || std::is_floating_point_v<T>,
			"this class only integral and floating point template parameters"
			);

	using value_type      = T;
	using reference       = T&;
	using const_reference = const T&;


	/// 													Data:
	T** data = nullptr;
	size_t n = 0;

	/// 												Memory management:
private:
	void fill_matrix (const T& element = T{});
	void alloc (size_t size);
	void alloc_and_fill (size_t size, const T& element = T{});
	void dealloc ();

public:
	/// 													Constructing:
	graph_matrix() = default;

	explicit graph_matrix(size_t size);

	/// 										 Moving / copying / freeing OOP stuff:

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

	/// 														 User API for memory management:
	void resize(size_t new_size);
	void release();

	void fill_default();



	/// 														 Edge access:

	[[nodiscard]] T& get_edge(size_t from, size_t to) const;

	void set_directed_edge(size_t from, size_t to, const T& value);

	void set_bidirectional_edge(size_t from, size_t to, const T& value);

	template<class Functor>
	void for_vertex_children (size_t vertex_index, const Functor &functor);

	std::vector<size_t> get_vertex_children (size_t starting_vertex_index);

	template<class Functor>
	void for_vertex_parents (size_t vertex_index, const Functor &functor);

	std::vector<size_t> get_vertex_parents (size_t starting_vertex_index);

	/// 														Operator []:
	template<class Type>
	struct col_t {
		Type* data;
		size_t size;

		Type& operator [](size_t index) {
			assert(index < size);

			return data[index];
		};
	};

#if defined(NDEBUG)
	T* operator[] (size_t index) {
		return data[index];
	}
#else
	col_t<T> operator[] (size_t index) {
		assert(index < n);
		return col_t<T> { data[index], n };
	}
#endif

	/// 														Graph outputting:
	template<class Type> friend std::ostream &operator<< (std::ostream &os, const graph_matrix<Type> &graph);
	// friend std::ostream& operator << (std::ostream& os, const adj_matrix_graph& graph);


	///														 Graph inputting from parsed data:

	// COULD BE Tagging types:
	// struct input_bidirectional {};
	// struct input_one_directional {};

	/// The way edges will be added:
	enum class edge_adding_types {
		bidirectional,  // given edges are passable in both directions
		directed        // given edges are passable only in the given direction
	};

	/// For BOOL matrices:

	template<class Vertex_indexing_type>
	void add_edges_from_list (edge_adding_types directionality,
	                          const std::vector<std::pair<Vertex_indexing_type, Vertex_indexing_type>> &edges);

	template</*class input_directionality_type, */class Vertex_indexing_type>
	void update_from_edge_list (edge_adding_types directionality,
	                            const std::vector<std::pair<Vertex_indexing_type, Vertex_indexing_type>> &edges);

	/// For ALL matrices <b>except bool</b>:

	template<class Vertex_indexing_type>
	void add_edges_from_list (edge_adding_types directionality,
	                          const std::vector<std::tuple<Vertex_indexing_type, Vertex_indexing_type, T>> &edges);

	template</*class input_directionality_type, */class Vertex_indexing_type>
	void update_from_edge_list (edge_adding_types directionality,
	                            const std::vector<std::tuple<Vertex_indexing_type, Vertex_indexing_type, T>> &edges);



	/// 												Graph inputting from character sequence:

	// From matrices:
	template<class Stream_type> // <- is usually std::istream or std::stringstream
	void input_from_matrix(Stream_type& is) {
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < n; ++j) {
				is >> data[j][i];
			}
		}
	}
	void input_from_matrix(const std::string& source_string) {
		std::stringstream stream;
		stream << source_string;

		input_from_matrix(stream);
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

	// From edge list:
	template<class input_directionality_type>
	void update_from_edge_list(size_t edge_list_size, std::istream& input_stream) {
		T temp_vertex_from, temp_vertex_to;
		for (size_t edge_index = 0; edge_index < edge_index; ++edge_index) {
			input_stream >>
		}
	}

};


using adj_matrix_graph =  graph_matrix<bool>;

using max_int_weighted_matrix_graph = graph_matrix<std::intmax_t>;
using max_uint_weighted_matrix_graph = graph_matrix<std::uintmax_t>;

using double_weighted_matrix_graph = graph_matrix<double>;

#include "graph_matrix.inl.h"

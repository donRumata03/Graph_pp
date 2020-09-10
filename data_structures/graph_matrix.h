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

	static constexpr T infinity = std::numeric_limits<T>::max();
	static constexpr bool is_adj_matrix = std::is_same_v<T, bool>;


	/// 													Data:
	T** data = nullptr;
	size_t n = 0;

	/// 												Memory management:
private:
	void fill_matrix (const T& element = T{});
	void alloc (size_t size);
	void alloc_and_fill (size_t size, const T& element);
	void alloc_and_fill (size_t size);
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

	/// 														 User API of memory management:
	bool empty();

	void resize(size_t new_size);
	void release();

	void fill_default();

	/// 														 Edge access:
	[[nodiscard]] bool has_edge(size_t from, size_t to) const;
	[[nodiscard]] T& get_edge(size_t from, size_t to) const;

	void set_directed_edge(size_t from, size_t to, const T& value);

	void set_bidirectional_edge(size_t from, size_t to, const T& value);

	template<class Functor>
	void for_vertex_children (size_t starting_vertex_index, const Functor &functor) const;

	[[nodiscard]] std::vector<size_t> get_vertex_children (size_t starting_vertex_index) const;

	template<class Functor>
	void for_vertex_parents (size_t starting_vertex_index, const Functor &functor) const;

	[[nodiscard]] std::vector<size_t> get_vertex_parents (size_t starting_vertex_index) const;

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

	template<class Type,
			class OutputStream,
			typename std::enable_if_t<std::is_same_v<OutputStream, std::ostream> || std::is_same_v<OutputStream, std::stringstream>, int>*
	>
	friend OutputStream &operator<< (OutputStream &os, const graph_matrix<Type> &graph);
	// friend std::ostream& operator << (std::ostream& os, const adj_matrix_graph& graph);


	///														 Graph inputting from parsed data:

	// COULD BE Tagging types:
	// struct input_bidirectional {};
	// struct input_one_directional {};

	/// The way edges will be added:
	enum class edge_adding_modes {
		bidirectional,  // given edges are passable in both directions
		directed        // given edges are passable only in the given direction
	};

	enum class input_numeration_modes {
		from_0, // Computers and programmers via computers usually number from 0 ;)
		from_1  // Users and task checkers usually number from 1
	};

	// TODO: move the enumclasses outside the class scope!

	/// For BOOL matrices:

	template<class Vertex_indexing_type>
	void add_edges_from_list (edge_adding_modes directionality, input_numeration_modes numeration_mode,
	                          const std::vector<std::pair<Vertex_indexing_type, Vertex_indexing_type>> &edges);

	template</*class input_directionality_type, */class Vertex_indexing_type>
	void update_from_edge_list (edge_adding_modes directionality, input_numeration_modes numeration_mode,
	                            const std::vector<std::pair<Vertex_indexing_type, Vertex_indexing_type>> &edges);


	/// For ALL matrices <b>except bool</b>:

	template<class Vertex_indexing_type>
	void add_edges_from_list (edge_adding_modes directionality, input_numeration_modes numeration_mode,
	                          const std::vector<std::tuple<Vertex_indexing_type, Vertex_indexing_type, T>> &edges);

	template</*class input_directionality_type, */class Vertex_indexing_type>
	void update_from_edge_list (edge_adding_modes directionality, input_numeration_modes numeration_mode,
	                            const std::vector<std::tuple<Vertex_indexing_type, Vertex_indexing_type, T>> &edges);



	/// 												Graph inputting from character sequence:

	// From matrices:
	template<
			class InputStream,
			typename std::enable_if_t<std::is_same_v<InputStream, std::istream> || std::is_same_v<InputStream, std::stringstream>, int>* = nullptr
	>
	void input_from_matrix(InputStream& is);

	void input_from_matrix(const std::string& source_string);

	template<
			class InputStream,
			typename std::enable_if_t<std::is_same_v<InputStream, std::istream> || std::is_same_v<InputStream, std::stringstream>, int>*
	>
	friend InputStream& operator >> (InputStream& is, graph_matrix<T>& graph);

	// From edge list:
	template<
	        class InputStream,
			typename std::enable_if_t<std::is_same_v<InputStream, std::istream> || std::is_same_v<InputStream, std::stringstream>, int>* = nullptr
			>
	void add_edges_from_list(edge_adding_modes directionality, input_numeration_modes numeration_mode, size_t edge_list_size, InputStream& input_stream);
	void add_edges_from_list(edge_adding_modes directionality, input_numeration_modes numeration_mode, size_t edge_list_size, const std::string& char_source);

	template<
			class InputStream,
			typename std::enable_if_t<std::is_same_v<InputStream, std::istream> || std::is_same_v<InputStream, std::stringstream>, int>* = nullptr
	>
	void update_from_edge_list(edge_adding_modes directionality, input_numeration_modes numeration_mode, size_t edge_list_size, InputStream& input_stream);
	void update_from_edge_list(edge_adding_modes directionality, input_numeration_modes numeration_mode, size_t edge_list_size, const std::string& char_source);

	/// 															Common operations with graph matrices:
	void make_all_edges_bidirectional();
};


using adj_matrix_graph =  graph_matrix<bool>;

using max_int_weighted_matrix_graph = graph_matrix<std::intmax_t>;
using max_uint_weighted_matrix_graph = graph_matrix<std::uintmax_t>;

using double_weighted_matrix_graph = graph_matrix<double>;

#include "graph_matrix.inl.h"

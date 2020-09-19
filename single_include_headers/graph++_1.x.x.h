//
// Created by Vova on 12.09.2020.
//


/// ————————————————————————————————————————	data_structures/graph_base.h	————————————————————————————————————————

//
// Created by Vova on 04.09.2020.
//

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include <algorithm>
#include <ostream>

#ifdef WIN32
#include <execution>
#endif

#include <cassert>
#include <iomanip>

using namespace std::string_literals;

/// Console encoding stuff:
#ifdef WIN32
#define IS_WINDOWS
#include "windows.h"
#undef max
#undef min
#endif

inline void switch_console_to_utf8() {
#ifdef IS_WINDOWS
    SetConsoleOutputCP(CP_UTF8);
	// system(("chcp "s + std::to_string(CP_UTF8)).c_str());
#else // Assume, it`s Linux => the console already uses utf8 by default:
    // NOP
#endif
}

namespace detail
{
    template <
            typename T,
            typename std::enable_if<std::is_floating_point<T>::value
                                    || (std::is_integral<T>::value && !std::is_same<T, bool>::value), int> * = nullptr
    >
    struct maximize_type_impl
    {
        using type = typename std::conditional<std::is_floating_point<T>::value, double, long long>::type;
    };



// || (std::is_integral<T>::value && !std::is_same_v<T, bool>)>* = nullptr

    template <>
    struct maximize_type_impl<bool>
    {
        // static_assert(false, "Can`t maximize type 'bool' ");
        using type = long long int;
    };

}


template<class T>
using maximize_type = typename detail::maximize_type_impl<T>::type;

/// ————————————————————————————————————————	data_structures/graph_matrix.h	————————————————————————————————————————

template <class T>
struct graph_matrix {
    static_assert(
            std::is_integral<T>::value || std::is_floating_point<T>::value,
            "this class only integral and floating point template parameters"
    );


    using value_type      = T;
    using reference       = T&;
    using const_reference = const T&;

    static constexpr T infinity = std::numeric_limits<T>::max();
    static constexpr bool is_adj_matrix = std::is_same<T, bool>::value;


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
            typename std::enable_if<std::is_same<OutputStream, std::ostream>::value || std::is_same<OutputStream, std::stringstream>::value, int>::type*
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
            typename std::enable_if<std::is_same<InputStream, std::istream>::value || std::is_same<InputStream, std::stringstream>::value, int>::type* = nullptr
    >
    void input_from_matrix(InputStream& is);

    void input_from_matrix(const std::string& source_string);

    template<
            class InputStream,
            typename std::enable_if<std::is_same<InputStream, std::istream>::value || std::is_same<InputStream, std::stringstream>::value, int>::type*
    >
    friend InputStream& operator >> (InputStream& is, graph_matrix<T>& graph);

    // From edge list:
    template<
            class InputStream,
            typename std::enable_if<std::is_same<InputStream, std::istream>::value || std::is_same<InputStream, std::stringstream>::value, int>::type* = nullptr
    >
    void add_edges_from_list(edge_adding_modes directionality, input_numeration_modes numeration_mode, size_t edge_list_size, InputStream& input_stream);
    void add_edges_from_list(edge_adding_modes directionality, input_numeration_modes numeration_mode, size_t edge_list_size, const std::string& char_source);

    template<
            class InputStream,
            typename std::enable_if<std::is_same<InputStream, std::istream>::value || std::is_same<InputStream, std::stringstream>::value, int>::type* = nullptr
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


/// ————————————————————————————————————————	data_structures/graph_matrix.inl.h	————————————————————————————————————————
//
// Created by Vova on 05.09.2020.
//

/// ********************************************* Memory management: *********************************************

template < class T >
void graph_matrix<T>::fill_matrix (const T &element)
{
    for (size_t i = 0; i < n; ++i) {
#ifdef WIN32
        std::fill(std::execution::par_unseq, data[i], data[i] + n, element);
#else
        std::fill(data[i], data[i] + n, element);
#endif
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


template <class T>
void graph_matrix<T>::alloc_and_fill (size_t size)
{
    alloc(size);
    fill_default();
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
#ifdef WIN32
        std::copy(std::execution::par_unseq, other.data[i], other.data[i] + n, this->data[i]);
#else
        std::copy(other.data[i], other.data[i] + n, this->data[i]);
#endif
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
            // std::copy(std::execution::par_unseq, other.data[i], other.data[i] + n, this->data[i]);
#ifdef WIN32
            std::copy(std::execution::par_unseq, other.data[i], other.data[i] + n, this->data[i]);
#else
            std::copy(other.data[i], other.data[i] + n, this->data[i]);
#endif
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


template <class T>
bool graph_matrix<T>::empty ()
{
#ifdef NDEBUG
    // Release

#else
    // Debug
    if (n == 0) assert(!data && "data should be nullptr if size is 0");
    else assert(data && "data shouldn`t be nullptr if size is NOT 0");
#endif

    return n == 0;
}


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
    T default_element;

    if constexpr (is_adj_matrix) {
        default_element = false;
    }
    else {
        default_element = infinity;
    }

    fill_matrix(default_element);

    for (size_t vertex_index = 0; vertex_index < n; ++vertex_index) {
        if constexpr (is_adj_matrix) {
            data[vertex_index][vertex_index] = true;
        }
        else {
            data[vertex_index][vertex_index] = T(0);
        }
    }
}

/// ********************************************* Edge access for user: *********************************************


template <class T>
bool graph_matrix<T>::has_edge (size_t from, size_t to) const
{
    if constexpr (std::is_same<T, bool>::value) {
        return get_edge(from, to);
    }
    else {
        // Check if it`s 'inf' (but 'max' in our case):
        return get_edge(from, to) != infinity;
    }
}

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
 * @param starting_vertex_index: the initial vertex index
 */
template <class T>
template <class Functor>
void graph_matrix<T>::for_vertex_children (size_t starting_vertex_index, const Functor &functor) const
{
    for (size_t target_index = 0; target_index < n; ++target_index) {
        if (starting_vertex_index != target_index && has_edge(starting_vertex_index, target_index)) {
            functor(target_index);
        }
    }
}


template <class T>
std::vector<size_t> graph_matrix<T>::get_vertex_children (size_t starting_vertex_index) const
{
    std::vector<size_t> res;
    res.reserve(n);

    // for_vertex_children(starting_vertex_index, [&](size_t index){ res.push_back(index); });
    for (size_t target_index = 0; target_index < n; ++target_index) {
        if (starting_vertex_index != target_index && has_edge(starting_vertex_index, target_index)) {
            res.push_back(target_index);
        }
    }

    return res;
}



template <class T>
template <class Functor>
void graph_matrix<T>::for_vertex_parents (size_t starting_vertex_index, const Functor &functor) const
{
    for (size_t target_index = 0; target_index < n; ++target_index) {
        if (starting_vertex_index != target_index && has_edge(target_index, starting_vertex_index)) {
            functor(target_index);
        }
    }
}

template <class T>
std::vector<size_t> graph_matrix<T>::get_vertex_parents (size_t starting_vertex_index) const
{
    std::vector<size_t> res;
    res.reserve(n);

    // for_vertex_parents(starting_vertex_index, [&](size_t index){ res.push_back(index); });
    for (size_t target_index = 0; target_index < n; ++target_index) {
        if (starting_vertex_index != target_index && has_edge(target_index, starting_vertex_index)) {
            res.push_back(target_index);
        }
    }

    return res;
}


/// ********************************************* Graph outputting: *********************************************

template<class Type,
        class OutputStream,
        typename std::enable_if<std::is_same<OutputStream, std::ostream>::value || std::is_same<OutputStream, std::stringstream>::value, int>::type* = nullptr
>
OutputStream& operator<< (OutputStream &os, const graph_matrix<Type> &graph)
{
    constexpr const char* separator = "__________________________________";
    os << separator << std::endl;
    os << (graph_matrix<Type>::is_adj_matrix ? "Adjacency" : "Weighted") << " matrix graph representation <" << graph.n << ">:" << std::endl;

    // Determine if it has big digits:
    bool has_big_digits = false;

    if constexpr (std::is_integral<Type>::value && !std::is_same<bool, Type>::value) {
        for (size_t i = 0; i < graph.n; i++) {
            bool is_broken = false;
            for (size_t j = 0; j < graph.n; j++) {
                // if (graph.get_edge(i, j) > 99 || graph.get_edge(i, j) < -9) {
                if (graph.get_edge(i, j) == graph_matrix<Type>::infinity) continue;
                if (graph.get_edge(i, j) > 9 || graph.get_edge(i, j) < 0) {
                    has_big_digits = true;
                    is_broken = true;
                    break;
                }
            }
            if (is_broken) break;
        }
    }

    if constexpr (std::is_floating_point<Type>::value) {
        os << std::setprecision(2);
        has_big_digits = true;
    }

    // std::cout << has_big_digits << std::endl;

    // The actual matrix output:
    for (size_t i = 0; i < graph.n; i++) {
        for (size_t j = 0; j < graph.n; j++) {
            // For bool:
            if constexpr (std::is_same<bool, Type>::value) {
                os << (graph.get_edge(j, i) ? "1" : "0");
                if (j != graph.n - 1) os << " ";
            }

                // For non-bools:
            else {
                os << (graph.get_edge(j, i) == graph_matrix<Type>::infinity ? "❌"s : std::to_string(graph.get_edge(j, i)));

                // For little-size stuff: numbers from [-9, 99]
                if (std::is_integral<Type>::value && !has_big_digits) {
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
graph_matrix<T>::add_edges_from_list (edge_adding_modes directionality, input_numeration_modes numeration_mode,
                                      const std::vector<std::pair<Vertex_indexing_type, Vertex_indexing_type>> &edges)
{
    /*
    static_assert(
            std::is_same_v<input_directionality_type, input_bidirectional> || std::is_same_v<input_directionality_type, input_one_directional>,
            "input_directionality_type should be one of: input_bidirectional, input_one_directional"
    );
    */

    static_assert(std::is_integral<Vertex_indexing_type>::value, "Vertex indexing type should be an integral type");
    static_assert(std::is_same<T, bool>::value , "T (graph template parameter) should bool to use this input mode");

    for (auto& p : edges) {
        auto[i, j] = p;

        if (numeration_mode == input_numeration_modes::from_1) {
            i--;
            j--;
        }

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
void graph_matrix<T>::update_from_edge_list (edge_adding_modes directionality, input_numeration_modes numeration_mode,
                                             const std::vector<std::pair<Vertex_indexing_type, Vertex_indexing_type>> &edges)
{
    /// Reset all edges and add the ones from the list:
    fill_default();
    add_edges_from_list(directionality, numeration_mode, edges);
}



////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
template <class Vertex_indexing_type>
void graph_matrix<T>::add_edges_from_list (graph_matrix::edge_adding_modes directionality, input_numeration_modes numeration_mode,
                                           const std::vector<std::tuple<Vertex_indexing_type, Vertex_indexing_type, T>> &edges)
{
    static_assert(std::is_integral<Vertex_indexing_type>::value, "Vertex indexing type should be an integral type");
    static_assert(!std::is_same<T, bool>::value , "T (graph template parameter) shouldn`t be bool to use this edge adding mode");

    for (auto& p : edges) {
        auto[i, j, value] = p;

        if (numeration_mode == input_numeration_modes::from_1) {
            i--;
            j--;
        }

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
void graph_matrix<T>::update_from_edge_list (graph_matrix::edge_adding_modes directionality, input_numeration_modes numeration_mode,
                                             const std::vector<std::tuple<Vertex_indexing_type, Vertex_indexing_type, T>> &edges)
{
    /// Reset all edges and add the ones from the list:

    fill_default();
    add_edges_from_list(directionality, numeration_mode, edges);
}


/// ********************************************* Graph inputting from streams: *********************************************

template <class T>
template<
        class InputStream,
        typename std::enable_if<std::is_same<InputStream, std::istream>::value || std::is_same<InputStream, std::stringstream>::value, int>::type*
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
        typename std::enable_if<std::is_same<InputStream, std::istream>::value || std::is_same<InputStream, std::stringstream>::value, int>::type* = nullptr
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
        typename std::enable_if<std::is_same<InputStream, std::istream>::value || std::is_same<InputStream, std::stringstream>::value, int>::type*
>
void graph_matrix<T>::add_edges_from_list (graph_matrix::edge_adding_modes directionality, input_numeration_modes numeration_mode, size_t edge_list_size,
                                           InputStream &input_stream)
{
    if constexpr (std::is_same<T, bool>::value) {
        std::vector<std::pair<size_t, size_t>> edges(edge_list_size);

        for (auto &p : edges) {
            input_stream >> p.first >> p.second;
        }

        add_edges_from_list(directionality, numeration_mode, edges);
    }
    else {
        std::vector<std::tuple<size_t, size_t, T>> edges(edge_list_size);

        for (auto &p : edges) {
            input_stream >> std::get<0>(p) >> std::get<1>(p) >> std::get<2>(p);
        }

        add_edges_from_list(directionality, numeration_mode, edges);
    }
}

template <class T>
void graph_matrix<T>::add_edges_from_list (graph_matrix::edge_adding_modes directionality, input_numeration_modes numeration_mode, size_t edge_list_size,
                                           const std::string &char_source)
{
    std::stringstream stream;
    stream << char_source;

    return add_edges_from_list(directionality, numeration_mode, edge_list_size, stream);
}

template <class T>
template <
        class InputStream,
        typename std::enable_if<std::is_same<InputStream, std::istream>::value || std::is_same<InputStream, std::stringstream>::value, int>::type*
>
void graph_matrix<T>::update_from_edge_list (graph_matrix::edge_adding_modes directionality, input_numeration_modes numeration_mode, size_t edge_list_size,
                                             InputStream &input_stream)
{
    /// Reset all edges and add the ones from the list:
    fill_default();
    add_edges_from_list(directionality, numeration_mode, edge_list_size, input_stream);
}

template <class T>
void graph_matrix<T>::update_from_edge_list (graph_matrix::edge_adding_modes directionality, input_numeration_modes numeration_mode, size_t edge_list_size,
                                             const std::string &char_source)
{
    std::stringstream stream;
    stream << char_source;

    return update_from_edge_list(directionality, numeration_mode, edge_list_size, stream);
}

/// ********************************************* Common operations with graph matrices: *********************************************

template <class T>
void graph_matrix<T>::make_all_edges_bidirectional ()
{
    // static_assert(std::is_same_v<T, bool>, "the type should be bool");

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < i; ++j) {
            if constexpr (std::is_same<T, bool>::value) {
                bool has_some_edge = has_edge(i, j) || has_edge(j, i);
                get_edge(i, j) = has_some_edge;
                get_edge(j, i) = has_some_edge;
            }
            else {
                if (!has_edge(i, j)) get_edge(i, j) = get_edge(j, i);
                if (!has_edge(j, i)) get_edge(j, i) = get_edge(i, j);
            }
        }
    }
}


/// ————————————————————————————————————————	algorithms/bfs.h	————————————————————————————————————————


#include <queue>
#include <functional>


template<class Callable>
void bfs(const graph_matrix<bool>& graph, size_t initial_vertex_index, /*const std::function<void (size_t, size_t)>&*/ Callable callback)
{
    if (false) {
        size_t vertex_index = 0;
        size_t iteration = 0;
        size_t parent_index = 0;

        // Causes compiler error if your function has false format:
        callback(parent_index, vertex_index, iteration);
    }


    assert(graph.n != 0);
    assert(initial_vertex_index < graph.n);
    auto n = graph.n;

    /*
    std::unique_ptr<size_t> this_wave(new size_t [n]), last_wave(new size_t [n]);
    size_t this_size = 0, last_size = 0;

    auto pop_this = [&this_wave, &this_size](){ return this_wave[--this_size]; };
    auto pop_last = [&last_wave, &last_size](){ return last_wave[--last_size]; };

    auto add_this = [&this_wave, &this_size](size_t vert_index){ return this_wave[this_size++] = vert_index; };
    auto add_last = [&last_wave, &last_size](size_t vert_index){ return last_wave[last_size++] = vert_index; };
    */

    std::vector<size_t> last_wave, this_wave;
    last_wave.reserve(n);
    this_wave.reserve(n);

    std::vector<bool> viewed_vertexes(n, false);

    size_t iteration_number = 0;

    auto process_verified_vertex = [&](size_t parent_index, size_t vertex_index){
        this_wave.push_back(vertex_index);
        viewed_vertexes[vertex_index] = true;
        callback(parent_index, vertex_index, iteration_number);
    };

    auto exchange_waves = [&](){
        // Move this wave contents to the new one and find data:
        this_wave.swap(last_wave);
        this_wave.clear();
    };

//	last_wave.push_back(initial_vertex_index);
//	viewed_vertexes[initial_vertex_index] = true;
//	callback(initial_vertex_index, 0); // Call function for initial vertex

    process_verified_vertex(size_t(-1), initial_vertex_index);
    exchange_waves();

    ++iteration_number;
    while (!last_wave.empty()) {
        for (size_t prev_vert_index : last_wave) {
            std::vector<size_t> all_vertexes = graph.get_vertex_children(prev_vert_index);
            for (size_t new_vert : all_vertexes) {
                if (!viewed_vertexes[new_vert]) {
//					callback(new_vert, iteration_number);
//					viewed_vertexes[new_vert] = true;
//					this_wave.push_back(new_vert);
                    process_verified_vertex(prev_vert_index, new_vert);
                }
            }
        }

        exchange_waves();

        // DEBUG:
        // std::cout << "Capacity: this: " << this_wave.capacity() << "; last: " << last_wave.capacity() << std::endl;

        ++iteration_number;
    }
}

template<class Callable, class Graph_Element_T, typename std::enable_if<!std::is_same<Graph_Element_T, bool>::value, int>::type* = nullptr>
void bfs(const graph_matrix<Graph_Element_T>& graph, size_t initial_vertex_index, Callable callback) {
    using distance_type = maximize_type<Graph_Element_T>;

    constexpr distance_type infinity = std::numeric_limits<distance_type>::max();


    if (false) {
        size_t vertex_index = 0;
        size_t iteration = 0;
        auto path_length = distance_type{};

        // Causes compiler error if your function has false format:
        callback(vertex_index, iteration, path_length);
    }

    assert(graph.n != 0);
    assert(initial_vertex_index < graph.n);
    auto n = graph.n;

    std::vector<size_t> last_wave, this_wave;
    last_wave.reserve(n);
    this_wave.reserve(n);

    std::vector<distance_type> known_distances(n, infinity);

    auto process_reviewed_vertex = [&](size_t parent_vertex_index, size_t vertex_index){
        // known_distances[vertex_index] = known_distances[parent_vertex_index];
        // TODO
    };

    last_wave.push_back(initial_vertex_index);
    known_distances[initial_vertex_index] = 0;

    callback(initial_vertex_index, 0, 0);

    size_t iteration_number = 1;

    while (!last_wave.empty()) {
        for (size_t parent_vertex : last_wave) {
            for (size_t new_vertex : graph.get_vertex_children(parent_vertex)) {
                if (known_distances[new_vertex] == infinity) {
                    known_distances[new_vertex] = known_distances[parent_vertex] + graph.get_edge(parent_vertex, new_vertex);
                    this_wave.push_back(new_vertex);
                    callback(new_vertex, iteration_number, known_distances[new_vertex]);
                }
            }
        }

        // Move this wave contents to the new one and find data:
        this_wave.swap(last_wave);
        this_wave.clear();

        ++iteration_number;
    }
}

/**
 * @returns: pair of distance and parent pointer vector
 */
inline std::pair<std::vector<size_t>, std::vector<size_t>> bfs(const graph_matrix<bool>& graph, size_t initial_vertex_index) {
    std::vector<size_t> distances;
    distances.assign(graph.n, std::numeric_limits<size_t>::max());

    std::vector<size_t> parent_pointers(graph.n, size_t(-1));


    bfs(graph, initial_vertex_index, [&](size_t parent_index, size_t vertex_index, size_t iteration){
        distances[vertex_index] = iteration;
        parent_pointers[vertex_index] = parent_index;
    });

    return { distances, parent_pointers };
}



template<
        class Graph_Element_T,
        typename res_type = maximize_type<Graph_Element_T>, // = std::conditional<std::is_floating_point<Graph_Element_T>::value, double, long long>,
        typename std::enable_if<std::is_floating_point<Graph_Element_T>::value || (std::is_integral<Graph_Element_T>::value && !std::is_same<Graph_Element_T, bool>::value), int>::type* = nullptr
>
std::vector<Graph_Element_T> bfs(const graph_matrix<Graph_Element_T>& graph, size_t initial_vertex_index) {
    // TODO: test this!!!

    std::vector<res_type> res;
    res.assign(graph.n, std::numeric_limits<res_type>::max());

    bfs(graph, [&](size_t parent_index, size_t vertex_index, size_t iteration, size_t path_length){
        res[vertex_index] = res_type(iteration);
    });

    return res;
}

/// ————————————————————————————————————————	algorithms/path_restorer.h + algorithms/path_restorer.cpp	————————————————————————————————————————

#include <cstddef>
#include <vector>
#include <stdexcept>
#include <cassert>
#include <string>

using namespace std::string_literals;

class outer_pointer_exception : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class cycle_path_exception : public std::runtime_error {
    using std::runtime_error::runtime_error;
};


/**
 * Restores path from the starting vertex to the ending vertex using parent vector
 *
 * @throws Exceptions if there are cycles (=> cycle_path_exception)
 * or if the starting vertex is still not reached but the value points to something not from vector (=> outer_pointer_exception)
 *
 * @param parents: for each vertex in the path there should be the pointer t his parent which is the previous vertex in the path
 * @param path_start: the vertex where your path starts from
 * @param path_end: the vertex where your path ends with
 * @returns the path (not reversed, the true order)
 */
std::vector<size_t> restore_path(const std::vector<size_t>& parents, size_t path_start, size_t path_end);

inline std::vector<size_t> restore_path(const std::vector<size_t> &parents, size_t path_start, size_t path_end) {
    size_t n = parents.size();

    if (path_start >= n) {
        throw std::out_of_range("Start is out of range!");
    }
    if (path_end >= n) {
        throw std::out_of_range("End is out of range!");
    }

    std::vector<size_t> res_path;
    std::vector<bool> visited(n);

    size_t current_vertex = path_end;
    auto prev_child_vertex_index = size_t(-1);

    while (current_vertex != path_start) {
        if (current_vertex > n)
            throw outer_pointer_exception(
                    "Parent pointer for "s + std::to_string(prev_child_vertex_index) + " points out (to " + std::to_string(current_vertex)
            );

        if (visited[current_vertex]) throw cycle_path_exception(
                    "There was a cycle detected from point "s + std::to_string(current_vertex) + "!"
            );

        res_path.push_back(current_vertex);

        visited[current_vertex] = true;
        prev_child_vertex_index = current_vertex;
        current_vertex = parents[current_vertex];
    }

    // Add the initial vertex:
    res_path.push_back(path_start);

    // Make the path be from start to end but not the opposite direction:
    std::reverse(res_path.begin(), res_path.end());

    return res_path;
}

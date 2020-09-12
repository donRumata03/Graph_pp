//
// Created by vova on 12.09.2020.
//

#pragma once

#include "data_structures/graphs.h"
#include <stdexcept>
#include <utility>

/**
 * Returns the vector distances from the nearest of the initial points for each point
 * 			and parents for each one in the path from the nearest initial vertex
 */
template<class T, typename std::enable_if<!std::is_same<T, bool>::value, int>::type* = nullptr>
std::pair<std::vector<maximize_type<T>>, std::vector<size_t>> dijkstra(const graph_matrix<T>& graph, const std::vector<size_t>& initial_vertexes) {
    using distance_type = maximize_type<T>;
    constexpr distance_type infinity = std::numeric_limits<distance_type>::max();
    constexpr bool T_is_signed = std::is_signed<T>::value;
    auto n = graph.n;

    // Perform pre-checks:
    for (size_t vertex_index : initial_vertexes) if (vertex_index >= n) throw std::out_of_range("Vertex index "s + std::to_string(vertex_index) + " is out of range!");

    if constexpr (T_is_signed) {
	    for (size_t i = 0; i < n; ++i) {
		    for (size_t j = 0; j < n; ++j) {
			    // Asert that all the edges are >= than 0:
				if (graph[i][j] < 0) throw std::out_of_range("Graph edge weights for Dijkstra should`n be less than 0!");
		    }
	    }
    }

    std::vector<bool> has_known_distance(n, false);
    std::vector<distance_type> distance(n, infinity);
    std::vector<size_t> parents(n, size_t(-1));

    for (size_t vertex : initial_vertexes) distance[vertex] = 0;

    while (true) {
        size_t vertex_index;
        bool has_vertex = false;

        // Find the new vertex to add to known:
        for (int test_v = 0; test_v < n; ++test_v) {
            if (!has_known_distance[test_v] && (!has_vertex || (distance[test_v] < distance[vertex_index]))) {
                vertex_index = test_v;
                has_vertex = true;
            }
        }

        if (!has_vertex) break; // If all the vertexes are marked
        if (vertex_index == infinity) break; // If the connection component is over

        has_known_distance[vertex_index] = true;

        for (auto& next_vertex_index : graph.get_vertex_children(vertex_index)) {
            auto len = graph[vertex_index][next_vertex_index];

            if (distance[vertex_index] + len < distance[next_vertex_index]) {
                distance[next_vertex_index] = distance[vertex_index] + len;
                parents[next_vertex_index] = vertex_index;
            }
        }
    }

	return {
    	distance, parents
    };
}

template<class T, typename std::enable_if<!std::is_same<T, bool>::value, int>::type* = nullptr>
std::pair<std::vector<maximize_type<T>>, std::vector<size_t>> dijkstra(const graph_matrix<T>& graph, size_t initial_vertex)
{
	return dijkstra(graph, std::vector<size_t>{initial_vertex});
}

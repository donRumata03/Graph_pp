//
// Created by vova on 12.09.2020.
//

#pragma once

#include "data_structures/graphs.h"

/**
 * Returns the distances
 */
template<class T>
void dijkstra(const graph_matrix<T>& graph, const std::vector<size_t>& initial_vertexes) {
    using distance_type = maximize_type<T>;
    constexpr distance_type infinity = std::numeric_limits<distance_type>::max();
    auto n = graph.n;

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

    // TODO: return!!!
}

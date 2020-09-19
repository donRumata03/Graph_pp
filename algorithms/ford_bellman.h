//
// Created by Vova on 19.09.2020.
//

#pragma once

#include "data_structures/graphs.h"

template<class T>
std::pair<std::vector<T>, std::vector<size_t>> ford_bellman(const graph_matrix<T>& graph, size_t initial_vertex) {
	// TODO: Make also version for edge list graph representation (because it`s currently O(n^3), which isn`t ideal)

	using length_t = maximize_type<T>;
	constexpr auto infinity = graph.infinity;
	auto n = graph.n;

	std::vector<std::vector<length_t>> d(n); // d[v][i] = the shortest path from initial_vertex to v through i or less edges
	for (auto& vec : d) vec.resize(n);

	for (size_t v = 0; v < n; ++v) {
		d[v][0] = infinity;
	}
	d[initial_vertex][0] = 0;

	std::vector<size_t> parents(n);

	for (size_t iteration = 1; iteration < n; ++iteration) {
		for (size_t v = 0; v < n; ++v) {
			d[v][iteration] = d[v][iteration - 1];

			for (size_t parent_vertex : graph.get_vertex_parents(v)) {
				// If there are no infinite-length edges
				if (d[parent_vertex][iteration - 1] != infinity && graph.get_edge(parent_vertex, v) != infinity) {
					d[v][iteration] = min(d[v][iteration], d[parent_vertex][iteration - 1] + graph.get_edge(parent_vertex, v));
				}
			}
		}
	}

	std::vector<length_t> res(n);
	for (size_t v = 0; v < n; ++v) {
		res[v] = d[v][n - 1];
	}

	return {
		res,
		p
	};
}

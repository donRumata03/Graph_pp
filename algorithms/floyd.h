//
// Created by Vova on 19.09.2020.
//

#pragma once

#include "data_structures/graphs.h"


/**
 * @returns: std::pair of: [matrix of distances between each pair of vertexes, matrix with "parents" for each pair (the vertex in the middle)]
 * @complexity: Exactly O(n^3)
 */

template<class T>
std::pair<std::vector<std::vector<maximize_type<T>>>, std::vector<std::vector<size_t>>> floyd(const graph_matrix<T>& graph) {
	using length_t = maximize_type<T>;
	constexpr auto infinity = graph.infinity;
	auto n = graph.n;

	std::vector<std::vector<length_t>> d(n); // d[v1][v2] = the shortest path between v1 and v2
 	for (auto& vec : d) vec.resize(n);

	std::vector<std::vector<size_t>> p(n);
	for (auto& vec : p) vec.resize(n);

	// Copy graph weights to d:
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			d[i][j] = graph[i][j];
		}
	}
	// Ensure d[i][i] == 0 for each i:
	for (size_t v = 0; v < n; ++v)
		d[v][v] = 0;

	// For each k: ...:
	for (size_t k = 0; k < n; ++k) {
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < n; ++j) {
				if (d[i][k] != infinity && d[k][j] != infinity && d[i][k] + d[k][j] < d[i][j]) {
					d[i][j] = d[i][k] + d[k][j];
				}
			}
		}
	}


	return {
		d, p
	};
}

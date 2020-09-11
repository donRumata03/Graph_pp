//
// Created by Vova on 12.09.2020.
//

#pragma once

#include "single_include_headers/graph++_1.0.0.h"

inline void test_first_version_of_library() {
	size_t n;
	std::cin >> n;

	adj_matrix_graph graph(n);
	std::cin >> graph;

	size_t vertex_from, vertex_to;
	std::cin >> vertex_from >> vertex_to;

	vertex_from--;
	vertex_to--;

	auto [distances, parent_vector] = bfs(graph, vertex_from);
	size_t distance = distances[vertex_to];

	// If there is no path:
	if (distance == size_t(-1)) {
		std::cout << -1 << std::endl;
		return;
	}

	// Find path otherwise:
	std::vector<size_t> path = restore_path(parent_vector, vertex_from, vertex_to);

	// Output result:
	std::cout << distance << std::endl;

	if (distance) {
		// Output the path if it`s not zero:
		for (unsigned long long i : path) {
			std::cout << i + 1 << " ";
		}
	}
	std::cout << std::endl;
}


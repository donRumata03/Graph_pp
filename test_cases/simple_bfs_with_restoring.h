//
// Created by vova on 11.09.2020.
//

#pragma once

#include "data_structures/graphs.h"
#include "algorithms/path_restorer.h"
#include "algorithms/bfs.h"


/*
 * Test cases for this task:
1)  5
	0 1 0 0 1
	1 0 1 0 0
	0 1 0 0 0
	0 0 0 0 0
	1 0 0 0 0
	3 5

    ^^^ Output should be:
    3
    3 2 1 5
 *
 */
inline void simple_bfs_with_restoring() {
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




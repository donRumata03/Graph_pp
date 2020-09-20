//
// Created by vova on 12.09.2020.
//

#pragma once

#include "data_structures/graphs.h"

#include "algorithms/dijkstra.h"
#include "algorithms/path_restorer.h"

inline void basic_dijkstra_test() {
	max_int_weighted_matrix_graph graph(8);

	std::vector<std::tuple<size_t, size_t, intmax_t>> edges_to_add = {
			{ 0, 1, 10 },
			{ 0, 3, 100 },
			{ 0, 5, 1 },

			{ 1, 7, 3 },
			{ 1, 2, 7 },
			{ 1, 4, 5 },

			{ 2, 3, 6 },


			{ 4, 5, 54 },

			{ 6, 7, 1 },
	};

	// std::cout << sizeof(size_t) << " " << sizeof(long) << " " << sizeof(long long) << std::endl; // 8 8 8

	graph.add_edges_from_list(
			edge_adding_modes::bidirectional,
			input_numeration_modes::from_0,
			edges_to_add);

	std::cout << graph << std::endl;

	/// Preform Dijkstra:

	auto[distances, parents] = dijkstra(graph, 0);

	for (size_t i = 0; i < graph.n; ++i) {
		std::cout << i << " : " << distances[i] << " ; parent is: " << parents[i] << std::endl;
	}

}

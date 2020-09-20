//
// Created by Vova on 20.09.2020.
//

#pragma once


#include "data_structures/graphs.h"


auto get_sample_negative_graph() {
	max_int_weighted_matrix_graph graph(5);

	std::vector<std::tuple<size_t, size_t, intmax_t>> edges_to_add = {
			{ 0, 1, 11 },
			{ 0, 3, 1 },
			{ 0, 4, -7 },

			{ 1, 2, 10 },

			{ 2, 3, -100 },
	};

	// std::cout << sizeof(size_t) << " " << sizeof(long) << " " << sizeof(long long) << std::endl; // 8 8 8

	graph.add_edges_from_list(
			edge_adding_modes::bidirectional,
			input_numeration_modes::from_0,
			edges_to_add);

	return graph;
}

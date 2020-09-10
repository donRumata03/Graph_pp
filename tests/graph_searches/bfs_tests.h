//
// Created by Vova on 09.09.2020.
//

#pragma once

#include "algorithms/bfs.h"

inline void test_basic_bfs() {
	adj_matrix_graph test_graph(7);

	test_graph.add_edges_from_list(adj_matrix_graph::edge_adding_modes::bidirectional, std::vector<std::pair<long long, long long>>{
			{0, 4},
			{0, 5},
			{4, 3},
			{4, 2},
			{3, 1},
			{2, 1},
			{1, 6},
	});


	std::cout << test_graph << std::endl;

	std::vector<long long> bfs_res = bfs(test_graph, 0);

	size_t index = 0;
	for(auto& elem : bfs_res) {
		std::cout << index << " : " << elem << "\n";
		index++;
	} std::cout << std::endl;
}

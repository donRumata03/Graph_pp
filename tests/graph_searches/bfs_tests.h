//
// Created by Vova on 09.09.2020.
//

#pragma once

#include "algorithms/bfs.h"

inline void test_basic_bool_bfs() {
	adj_matrix_graph test_graph(7);

	test_graph.add_edges_from_list(adj_matrix_graph::edge_adding_modes::bidirectional, adj_matrix_graph::input_numeration_modes::from_0, std::vector<std::pair<long long, long long>>{
			{0, 4},
			{0, 5},
			{4, 3},
			{4, 2},
			{3, 1},
			{2, 1},
			{1, 6},
	});


	std::cout << test_graph << std::endl;

	/*std::vector<size_t>*/auto [bfs_res, parents] = bfs(test_graph, 0);

	size_t index = 0;
	for(auto& elem : bfs_res) {
		std::cout << index << " : dist=" << elem << "; parent=" << parents[index] << "\n";
		index++;
	} std::cout << std::endl;
}

inline void test_custom_graph_bfs () {
	using test_type = double;

	graph_matrix<test_type> test_graph(7);

	test_graph.add_edges_from_list(
			graph_matrix<test_type>::edge_adding_modes::bidirectional, graph_matrix<test_type>::input_numeration_modes::from_0, std::vector<std::tuple<long long, long long, test_type>>{
			{0, 4, 1},
			{0, 5, 1},
			{4, 3, 1},
			{4, 2, 1},
			{3, 1, 1},
			{2, 1, 1},
			{1, 6, 1},
	});

	bfs(test_graph, 0, [](size_t vertex_index, size_t iteration, size_t path_length){
		std::cout << vertex_index << " " << iteration << " " << path_length << std::endl;
	});

}


//
// Created by Vova on 04.09.2020.
//

#pragma once

#include <data_structures/graphs.h>
#include "data_structures/graph_base.h"

inline void graph_creation_test() {
	auto graph = adj_matrix_graph(5);

	graph.set_directed_edge(3, 2, true);

	graph.set_directed_edge(1, 3, true);

}

inline void graph_output_test() {
	auto graph = adj_matrix_graph(5);

	graph.set_directed_edge(3, 2, true);
	graph.set_directed_edge(4, 1, true);
	graph.set_directed_edge(2, 1, true);

	graph.set_bidirectional_edge(3, 1, true);

	std::cout << graph << std::endl;

	std::cout << "graph[1][3]: " << graph.get_edge(1, 3) << std::endl;

	///////////////////////////////// Big digits /////////////////////////////////
	auto big_graph = max_int_weighted_matrix_graph(5);
	big_graph.set_bidirectional_edge(1, 4, 239);
	big_graph.set_bidirectional_edge(2, 3, 566);

	std::cout << big_graph << std::endl;
}

inline void graph_operator_test () {
	auto graph = adj_matrix_graph(5);

	graph.set_directed_edge(3, 2, true);
	graph.set_directed_edge(4, 1, true);
	graph.set_directed_edge(2, 1, true);

	graph.set_bidirectional_edge(3, 1, true);

	std::cout << graph << std::endl;

	auto col = graph[3];
	std::cout << typeid(col).name() << std::endl;

	std::cout << "3rdColon: " <<
		col[0] << " " << col[1] << " " << col[2] << " " << col[3] << " " << col[4]
	<< std::endl;

	std::cout << graph[2][0] << " " << graph[2][1] << " " << graph[2][2] << std::endl;
}


inline void bool_graph_edge_adding_test() {
	adj_matrix_graph graph(3);

	std::vector<std::pair<long long, long long>> edges_to_add = {
			{1, 2},
			{2, 0},
	};

	graph.add_edges_from_list(adj_matrix_graph::edge_adding_modes::directed, edges_to_add);

	std::cout << graph << std::endl;

	graph.add_edges_from_list(adj_matrix_graph::edge_adding_modes::bidirectional, edges_to_add);

	std::cout << graph << std::endl;
}

inline void weighted_graph_edge_adding_test() {
	max_int_weighted_matrix_graph graph(3);

	std::vector<std::tuple<long long, long long, std::intmax_t>> edges_to_add = {
			{1, 2, 34},
			{2, 0, -12},
	};

	graph.add_edges_from_list(max_int_weighted_matrix_graph::edge_adding_modes::directed, edges_to_add);

	std::cout << graph << std::endl;

	graph.add_edges_from_list(max_int_weighted_matrix_graph::edge_adding_modes::bidirectional, edges_to_add);

	std::cout << graph << std::endl;
}

//////////////////////////////////// 	Graph input from streams and strings:  ////////////////////////////////////

inline void matrix_graph_input_test() {
	max_int_weighted_matrix_graph graph(3);

	// std::cin >> graph;

	graph.input_from_matrix(std::cin);
	std::cout << graph << std::endl;

	std::cout << "First colon: " << graph[0][0] << " " << graph[0][1] << " " << graph[0][2] << std::endl;
	std::cout << "First row: " << graph[0][0] << " " << graph[1][0] << " " << graph[2][0] << std::endl;

}


inline void test_graph_input_from_edge_list() {
	size_t n;
	std::cin >> n;
	adj_matrix_graph graph (n);

	std::size_t edges;
	std::cin >> edges;

	graph.update_from_edge_list(adj_matrix_graph::edge_adding_modes::directed, edges, std::cin);

	std::cout << graph << std::endl;
}



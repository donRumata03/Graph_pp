//
// Created by Vova on 04.09.2020.
//

#pragma once

#include "../data_structures/graph_base.h"

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
	auto big_graph = max_int_adj_matrix_graph(5);
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

void graph_input_test() {
	max_int_adj_matrix_graph graph(3);
	std::cin >> graph;

}

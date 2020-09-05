//
// Created by Vova on 04.09.2020.
//

#pragma once

// #include "../data_structures/graph_base.h"
#include "../data_structures/graphs.h"

template<class Callable, class Graph_Element_T>
void bfs(const graph_matrix<Graph_Element_T>& graph, Callable callback) {
	if (false) {
		size_t vertex_number = 0;
		size_t iteration = 0;

		// Causes compiler error if your function has false format:
		callback(vertex, iteration);
	}


}

template<class Graph_Element_T>
std::vector<long long> bfs(const graph_matrix<Graph_Element_T>& graph) {
	std::vector<long long> res;
	res.assign(graph.n, -1LL);

	bfs(graph, [&](size_t vertex_index, size_t iteration){
		res[vertex_index] = iteration;
	});

	return res;
}



//
// Created by Vova on 04.09.2020.
//

#pragma once

// #include "../data_structures/graph_base.h"
#include "../data_structures/graphs.h"
#include <queue>


template<class Callable, class Graph_Element_T>
void bfs(const graph_matrix<Graph_Element_T>& graph, size_t initial_vertex_index, Callable callback) {
	if (false) {
		size_t vertex_index = 0;
		size_t iteration = 0;
		Graph_Element_T path_length = Graph_Element_T{};

		// Causes compiler error if your function has false format:
		callback(vertex_index, iteration, path_length);
	}

	assert(initial_vertex_index < graph.n);

	// TODO!
}

template<class Callable>
void bfs(const graph_matrix<bool>& graph, size_t initial_vertex_index, Callable callback)
{
	if (false) {
		size_t vertex_index = 0;
		size_t iteration = 0;

		// Causes compiler error if your function has false format:
		callback(vertex_index, iteration);
	}
	assert(initial_vertex_index != 0);
	assert(initial_vertex_index < graph.n);
	auto n = graph.n;

	/*
	std::unique_ptr<size_t> this_wave(new size_t [n]), last_wave(new size_t [n]);
	size_t this_size = 0, last_size = 0;

	auto pop_this = [&this_wave, &this_size](){ return this_wave[--this_size]; };
	auto pop_last = [&last_wave, &last_size](){ return last_wave[--last_size]; };

	auto add_this = [&this_wave, &this_size](size_t vert_index){ return this_wave[this_size++] = vert_index; };
	auto add_last = [&last_wave, &last_size](size_t vert_index){ return last_wave[last_size++] = vert_index; };
	*/

	std::vector<size_t> last_wave, this_wave;

	std::vector<bool> viewed_vertexes(n, false);
	// add_last(initial_vertex_index);

	while (true) {

	}
}

template<class Graph_Element_T>
std::vector<long long> bfs(const graph_matrix<Graph_Element_T>& graph, size_t initial_vertex_index) {
	std::vector<long long> res;
	res.assign(graph.n, -1LL);

	bfs(graph, [&](size_t vertex_index, size_t iteration){
		res[vertex_index] = iteration;
	});

	return res;
}



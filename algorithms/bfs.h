//
// Created by Vova on 04.09.2020.
//

#pragma once

// #include "../data_structures/graph_base.h"
#include "../data_structures/graphs.h"
#include <queue>
#include <functional>


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
void bfs(const graph_matrix<bool>& graph, size_t initial_vertex_index, /*const std::function<void (size_t, size_t)>&*/ Callable callback)
{
	if (false) {
		size_t vertex_index = 0;
		size_t iteration = 0;

		// Causes compiler error if your function has false format:
		callback(vertex_index, iteration);
	}
	assert(graph.n != 0);
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
	last_wave.reserve(n);
	this_wave.reserve(n);

	std::vector<bool> viewed_vertexes(n, false);



	last_wave.push_back(initial_vertex_index);
	viewed_vertexes[initial_vertex_index] = true;
	callback(initial_vertex_index, 0); // Call function for initial vertex

	size_t iteration_number = 1;
	while (!last_wave.empty()) {
		for (size_t prev_vert_index : last_wave) {
			std::vector<size_t> all_vertexes = graph.get_vertex_children(prev_vert_index);
			for (size_t new_vert : all_vertexes) {
				if (!viewed_vertexes[new_vert]) {
					callback(new_vert, iteration_number);
					viewed_vertexes[new_vert] = true;
					this_wave.push_back(new_vert);
				}
			}
		}

		// Move this wave contents to the new one and find data:
		this_wave.swap(last_wave);
		this_wave.clear();

		// DEBUG:
		// std::cout << "Capacity: this: " << this_wave.capacity() << "; last: " << last_wave.capacity() << std::endl;

		++iteration_number;
	}
}

template<
		class Graph_Element_T,
		typename res_type = std::conditional<std::is_floating_point_v<Graph_Element_T>, double, long long>,
		typename std::enable_if<std::is_floating_point_v<Graph_Element_T> || (std::is_integral_v<Graph_Element_T> && !std::is_same_v<Graph_Element_T, bool>)>* = nullptr
        >
std::vector<Graph_Element_T> bfs(const graph_matrix<Graph_Element_T>& graph, size_t initial_vertex_index) {
	std::vector<res_type> res;
	res.assign(graph.n, std::numeric_limits<res_type>::max());

	bfs(graph, [&](size_t vertex_index, size_t iteration){
		res[vertex_index] = res_type(iteration);
	});

	return res;
}


inline std::vector<long long> bfs(const graph_matrix<bool>& graph, size_t initial_vertex_index) {
	std::vector<long long> res;
	res.assign(graph.n, std::numeric_limits<long long>::max());

	bfs(graph, initial_vertex_index, [&](size_t vertex_index, size_t iteration){
		res[vertex_index] = iteration;
	});

	return res;
}


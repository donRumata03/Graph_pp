//
// Created by Vova on 04.09.2020.
//

#pragma once

#include "data_structures/graph_base.h"
#include "data_structures/graphs.h"
#include <queue>
#include <functional>


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

	size_t iteration_number = 0;

	auto process_verified_vertex = [&](size_t vertex_index){
		this_wave.push_back(vertex_index);
		viewed_vertexes[vertex_index] = true;
		callback(vertex_index, iteration_number);
	};

	auto exchange_waves = [&](){
		// Move this wave contents to the new one and find data:
		this_wave.swap(last_wave);
		this_wave.clear();
	};

//	last_wave.push_back(initial_vertex_index);
//	viewed_vertexes[initial_vertex_index] = true;
//	callback(initial_vertex_index, 0); // Call function for initial vertex

	process_verified_vertex(initial_vertex_index);
	exchange_waves();

	++iteration_number;
	while (!last_wave.empty()) {
		for (size_t prev_vert_index : last_wave) {
			std::vector<size_t> all_vertexes = graph.get_vertex_children(prev_vert_index);
			for (size_t new_vert : all_vertexes) {
				if (!viewed_vertexes[new_vert]) {
//					callback(new_vert, iteration_number);
//					viewed_vertexes[new_vert] = true;
//					this_wave.push_back(new_vert);
					process_verified_vertex(new_vert);
				}
			}
		}

		exchange_waves();

		// DEBUG:
		// std::cout << "Capacity: this: " << this_wave.capacity() << "; last: " << last_wave.capacity() << std::endl;

		++iteration_number;
	}
}

template<class Callable, class Graph_Element_T, typename std::enable_if_t<!std::is_same_v<Graph_Element_T, bool>, int>* = nullptr>
void bfs(const graph_matrix<Graph_Element_T>& graph, size_t initial_vertex_index, Callable callback) {
	using distance_type = typename maximize_type<Graph_Element_T>;

	constexpr distance_type infinity = std::numeric_limits<distance_type>::max();


	if (false) {
		size_t vertex_index = 0;
		size_t iteration = 0;
		auto path_length = distance_type{};

		// Causes compiler error if your function has false format:
		callback(vertex_index, iteration, path_length);
	}

	assert(graph.n != 0);
	assert(initial_vertex_index < graph.n);
	auto n = graph.n;

	std::vector<size_t> last_wave, this_wave;
	last_wave.reserve(n);
	this_wave.reserve(n);

	std::vector<distance_type> known_distances(n, infinity);

	auto process_reviewed_vertex = [&](size_t parent_vertex_index, size_t vertex_index){
		// known_distances[vertex_index] = known_distances[parent_vertex_index];
		// TODO
	};

	last_wave.push_back(initial_vertex_index);
	known_distances[initial_vertex_index] = 0;

	callback(initial_vertex_index, 0, 0);

	size_t iteration_number = 1;

	while (!last_wave.empty()) {
		for (size_t parent_vertex : last_wave) {
			for (size_t new_vertex : graph.get_vertex_children(parent_vertex)) {
				if (known_distances[new_vertex] == infinity) {
					known_distances[new_vertex] = known_distances[parent_vertex] + graph.get_edge(parent_vertex, new_vertex);
					this_wave.push_back(new_vertex);
					callback(new_vertex, iteration_number, known_distances[new_vertex]);
				}
			}
		}

		// Move this wave contents to the new one and find data:
		this_wave.swap(last_wave);
		this_wave.clear();

		++iteration_number;
	}
}


inline std::vector<long long> bfs(const graph_matrix<bool>& graph, size_t initial_vertex_index) {
	std::vector<long long> res;
	res.assign(graph.n, std::numeric_limits<long long>::max());

	bfs(graph, initial_vertex_index, [&](size_t vertex_index, size_t iteration){
		res[vertex_index] = iteration;
	});

	return res;
}



template<
		class Graph_Element_T,
		typename res_type = maximize_type<Graph_Element_T>, // = std::conditional<std::is_floating_point_v<Graph_Element_T>, double, long long>,
		typename std::enable_if<std::is_floating_point_v<Graph_Element_T> || (std::is_integral_v<Graph_Element_T> && !std::is_same_v<Graph_Element_T, bool>)>* = nullptr
        >
std::vector<Graph_Element_T> bfs(const graph_matrix<Graph_Element_T>& graph, size_t initial_vertex_index) {
	// TODO: test this!!!

	std::vector<res_type> res;
	res.assign(graph.n, std::numeric_limits<res_type>::max());

	bfs(graph, [&](size_t vertex_index, size_t iteration, size_t path_length){
		res[vertex_index] = res_type(iteration);
	});

	return res;
}


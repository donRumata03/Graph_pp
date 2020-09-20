//
// Created by Vova on 20.09.2020.
//

#pragma once

#include "gs_test_base.h"

#include "algorithms/ford_bellman.h"

inline void ford_bellman_basic_test() {
	auto graph = get_sample_negative_graph();

	// Counting distances from vertex 0:
	auto [distances, parents] = ford_bellman(graph, 0);

	// Print result:

}


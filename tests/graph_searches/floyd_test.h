//
// Created by Vova on 20.09.2020.
//

#pragma once

#include "gs_test_base.h"

#include "algorithms/floyd.h"

inline void ford_bellman_basic_test() {
	auto graph = get_sample_negative_graph();

	// Counting distances from vertex 0:
	auto [distances, parents] = floyd(graph);

	// Print result:

}


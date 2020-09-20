//
// Created by Vova on 19.09.2020.
//

#pragma once

#include "graph_base.h"

struct edge {
	size_t from, to;
};

template<class weight_t>
struct weighted_edge {
	size_t from, to;
	weight_t weight;
};

template<>
struct weighted_edge<bool> {
	size_t from, to;
};

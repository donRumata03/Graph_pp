//
// Created by Vova on 11.09.2020.
//

#pragma once

#include "data_structures/graphs.h"

#include "algorithms/path_restorer.h"
#include "algorithms/bfs.h"

/*
 * Tests:

1) 3
 1 1
 3 3

 Correct output is:
	4
	1 1
	3 2
	1 3
	2 1
	3 3
2) 3
 1 1
 2 2

  Correct output is:
	ERROR!

 3) 5
	3 3
	5 1

  Correct output is:
	4
	3 3
	2 1
	1 3
	3 2
	5 1

 4) 5
	3 3
	3 3

  Correct output is:
	0
    3 3
 */

inline void chess_horse_bfs() {
	size_t n;
	std::cin >> n;

	std::pair<size_t, size_t> start, end;

	std::cin >> start.first >> start.second >> end.first >> end.second;

	start.first--;  start.second--;
	end.first--;    end.second--;

	// Numerate cells mainly from top to bottom and after that — from left to right:
	auto get_cell_index_from_coords = [n](size_t x, size_t y){
		return x + y * n;
	};

	auto get_coords_from_cell_index = [n](size_t cell_index) -> std::pair<size_t, size_t> {
		return {
			cell_index % n,
			cell_index / n
		};
	};

	auto horse_can_go = [](const std::pair<size_t, size_t>& from, const std::pair<size_t, size_t>& to) -> bool {
		long long x1 = from.first;
		long long y1 = from.second;

		long long x2 = to.first;
		long long y2 = to.second;

		return ((std::abs(x1 - x2) == 2) && (std::abs(y1 - y2) == 1))
		|| ((std::abs(y1 - y2) == 2) && (std::abs(x1 - x2) == 1));

		/*
		if ((std::abs(x1 - x2) == 2) && (std::abs(y1 - y2) == 1)) return true;
		if ((std::abs(y1 - y2) == 2) && (std::abs(x1 - x2) == 1)) return true;
		return false;
		*/
	};

/*

	std::cout << get_coords_from_cell_index(8).first << " " << get_coords_from_cell_index(8).second << std::endl;
	std::cout << get_cell_index_from_coords(1, 1) << std::endl;
	std::cout << horse_can_go({1, 0}, {2, 2});
	std::cout << " " << horse_can_go({1, 0}, {1, 2}) << std::endl;

*/

	// We have n*n cells, n*n graph_vertexes
	size_t nn = n * n;

	// We use graph bfs to determine the number of iterations for the horse to reach the given cell:
	adj_matrix_graph graph(nn);

	// Make the adj matrix so we can provide it to the graph:
	for (size_t cell_1 = 0; cell_1 < nn; ++cell_1) {
		for (size_t cell_2 = 0; cell_2 < nn; ++cell_2) {
			auto [x1, y1] = get_coords_from_cell_index(cell_1);
			auto [x2, y2] = get_coords_from_cell_index(cell_2);

			if (horse_can_go({x1, y1}, {x2, y2})) {
				graph.set_directed_edge(cell_1, cell_2, true);
			}
		}
	}

	// For Debug:
	// std::cout << graph << std::endl;

	auto[distances, parent_pointer_storage] = bfs(graph, get_cell_index_from_coords(start.first, start.second)/*, get_cell_index_from_coords(end.first, end.second)*/);

	size_t distance = distances[get_cell_index_from_coords(end.first, end.second)];

	 if (distance == size_t(-1)) {
	 	// Not expected at least for N >= 5
	 	std::cout << "Can`t find the path!" << std::endl;
	 	std::terminate();
	 }

	 // Restore path:
	 auto path = restore_path(parent_pointer_storage, get_cell_index_from_coords(start.first, start.second), get_cell_index_from_coords(end.first, end.second));

	 // Output the answer:
	 std::cout << distance << std::endl;

	for (auto& cell_index : path) {
		auto [x, y] = get_coords_from_cell_index(cell_index);
		std::cout << x + 1 << " " << y + 1 << std::endl;
	}
}

//
// Created by vova on 11.09.2020.
//

#include "path_restorer.h"

std::vector<size_t> restore_path(const std::vector<size_t> &parents, size_t path_start, size_t path_end) {
    size_t n = parents.size();

    if (path_start >= n) {
        throw std::out_of_range("Start is out of range!");
    }
    if (path_end >= n) {
        throw std::out_of_range("End is out of range!");
    }

    std::vector<size_t> res_path;
    std::vector<bool> visited(n);

    size_t current_vertex = path_end;
    auto prev_child_vertex_index = size_t(-1);

    while (current_vertex != path_start) {
        if (current_vertex > n)
            throw outer_pointer_exception(
                    "Parent pointer for "s + std::to_string(prev_child_vertex_index) + " points out (to " + std::to_string(current_vertex)
            );

        if (visited[current_vertex]) throw cycle_path_exception(
                    "There was a cycle detected from point "s + std::to_string(current_vertex) + "!"
            );

        res_path.push_back(current_vertex);

        visited[current_vertex] = true;
        prev_child_vertex_index = current_vertex;
        current_vertex = parents[current_vertex];
    }

    // Add the initial vertex:
    res_path.push_back(path_start);

    // Make the path be from start to end but not the opposite direction:
    std::reverse(res_path.begin(), res_path.end());

    return res_path;
}

std::vector<size_t>
restore_path (const std::vector<std::vector<size_t>> &parent_matrix, size_t path_start, size_t path_end)
{
	// TODO: implement restoring!
	return std::vector<size_t>();
}

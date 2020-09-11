//
// Created by vova on 11.09.2020.
//

#ifndef GRAPHS_PATH_RESTORER_H
#define GRAPHS_PATH_RESTORER_H

#include <cstddef>
#include <vector>
#include <stdexcept>
#include <cassert>
#include <string>

using namespace std::string_literals;

class outer_pointer_exception : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class cycle_path_exception : public std::runtime_error {
    using std::runtime_error::runtime_error;
};


/**
 * Restores path from the starting vertex to the ending vertex using parent vector
 *
 * @throws Exceptions if there are cycles (=> cycle_path_exception)
 * or if the starting vertex is still not reached but the value points to something not from vector (=> outer_pointer_exception)
 *
 * @param parents: for each vertex in the path there should be the pointer t his parent which is the previous vertex in the path
 * @param path_start: the vertex where your path starts from
 * @param path_end: the vertex where your path ends with
 * @returns the path (not reversed, the true order)
 */
std::vector<size_t> restore_path(const std::vector<size_t>& parents, size_t path_start, size_t path_end){
    size_t n = parents.size();

    if (path_start >= n) {
        throw std::out_of_range("Start is out of range!");
    }
    if (path_end) {
        throw std::out_of_range("End is out of range!");
    }
    assert(path_end < parents.size());

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

    // Make the path be from start to end but not the opposite direction:
    std::reverse(res_path.begin(), res_path.end());

    return res_path;
}

#endif //GRAPHS_PATH_RESTORER_H

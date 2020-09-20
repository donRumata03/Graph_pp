//
// Created by vova on 11.09.2020.
//

#pragma once

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
std::vector<size_t> restore_path(const std::vector<size_t>& parents, size_t path_start, size_t path_end);

/**
 * Restores path from the starting vertex to the ending vertex using parent MATRIX
 *
 * @throws Exceptions if there are cycles (=> cycle_path_exception)
 * or if the starting vertex is still not reached but the value points to something not from vector (=> outer_pointer_exception)
 *
 * @param parents: matrix (std::vector<std:vector<size_t>>) — for each vertex pair {i, j} (parent_matrix[i][j]) there is the vertex, through which
 * @param path_start: the vertex where your path starts from
 * @param path_end: the vertex where your path ends with
 * @returns the path (not reversed, the true order)
 */
std::vector<size_t> restore_path(const std::vector<std::vector<size_t>>& parent_matrix, size_t path_start, size_t path_end);





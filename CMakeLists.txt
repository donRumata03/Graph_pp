cmake_minimum_required(VERSION 3.16)
project(Graphs)

set(CMAKE_CXX_STANDARD 20)

include_directories(./)

add_executable(Graphs main.cpp data_structures/graph_base.cpp data_structures/graph_base.h algorithms/bfs.cpp algorithms/bfs.h tests/graph_matrix/basic_graph_matrix_tests.h data_structures/graph_matrix.cpp data_structures/graph_matrix.h data_structures/graphs.h data_structures/graph_matrix.inl.h tests/graph_matrix/memory_management_tests.h tests/random_tests.h)
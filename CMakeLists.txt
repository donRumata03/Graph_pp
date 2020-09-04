cmake_minimum_required(VERSION 3.16)
project(Graphs)

set(CMAKE_CXX_STANDARD 20)

include_directories(./)

add_executable(Graphs main.cpp data_structures/graph_base.cpp data_structures/graph_base.h algorithms/bfs.cpp algorithms/bfs.h tests/graph_tests.h)
cmake_minimum_required(VERSION 3.16)
project(Graphs)

set(CMAKE_CXX_STANDARD 20)

include_directories(./)

add_executable(Graphs
        main.cpp

        algorithms/bfs.h

        data_structures/graph_base.cpp
        data_structures/graph_base.h
        data_structures/graph_matrix.h
        data_structures/graphs.h
        data_structures/graph_matrix.inl.h

        # Tests:
        tests/graph_matrix/basic_graph_matrix_tests.h
        tests/graph_matrix/memory_management_tests.h
        tests/random_tests.h
        tests/graph_searches/bfs_tests.h

        algorithms/path_restorer.cpp algorithms/path_restorer.h test_cases/simple_bfs_with_restoring.h tests/util_tests/graph_restoring_test.h test_cases/chess_horse_bfs.h)
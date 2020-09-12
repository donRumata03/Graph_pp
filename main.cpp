/*
#define IS_SINGLE_INCLUDE_TEST

#ifndef IS_SINGLE_INCLUDE_TEST

*/


#include <test_cases/simple_bfs_with_restoring.h>
#include <test_cases/chess_horse_bfs.h>
#include <tests/graph_searches/djikstra_test.h>
#include "tests/random_tests.h"

#include "tests/graph_matrix/basic_graph_matrix_tests.h"
#include "tests/graph_matrix/memory_management_tests.h"

#include "tests/graph_searches/bfs_tests.h"
#include "tests/util_tests/graph_restoring_test.h"

int main ()
{
	switch_console_to_utf8();

	/// Random:
	// test_maxing_types();


	/// Basic matrix graph tests:
	// graph_static_consts_test();

	// graph_creation_test();
	// graph_operator_test();

	// graph_output_test();
	// matrix_graph_input_test();

	// bool_graph_edge_adding_test();
	// weighted_graph_edge_adding_test();

	// matrix_graph_input_test();
	// test_graph_input_from_edge_list();

	// test_graph_numeration_modes();

	/// Matrix graph memory management tests:

	// test_matrix_graph_copy_assignment_operator();
	// test_matrix_graph_move_assignment_operator();

	// test_matrix_graph_copy_constructor();
	// test_matrix_graph_move_constructor();

	// test_graph_self_assignments();


	/// BFS:

	// test_basic_bool_bfs();
	// test_custom_graph_bfs();

    /// Restoring:
    // test_simulated_restoring();

    /// Real test-cases:
	// simple_bfs_with_restoring();
	// chess_horse_bfs();

	/// Dijkstra:
	basic_dijkstra_test();

	// TODO: add Floyd algorithm
	// TODO: add new data structure <graph representation> through list of neighboring vertexes for each one

	return 0;
}

/*
#else

/// If it`s single include test:

#include <tests/header_collection_tests/1.0.0_test.h>

int main () {
	test_first_version_of_library();
}

#endif
 */



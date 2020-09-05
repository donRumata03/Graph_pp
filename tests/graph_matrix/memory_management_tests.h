//
// Created by Vova on 05.09.2020.
//

#pragma once

#include "../../data_structures/graphs.h"


inline void test_matrix_graph_copy_assignment_operator() {
	adj_matrix_graph initial_graph(3);

	initial_graph[2][1] = true;
	initial_graph[0][1] = true;
	initial_graph[1][2] = true;

	/* ^^^
	0 0 0
	1 0 1
	0 1 0
	*/


	adj_matrix_graph new_graph(1212);
	new_graph = initial_graph; // Perform deep copy

	// Modify initial graph:
	initial_graph[1][0] = true;

	std::cout << "Initial graph: " << std::endl << initial_graph << std::endl;
	std::cout << "New graph: " << std::endl << new_graph << std::endl;

	// Auto Destructors:
	// ...
}

inline void test_matrix_graph_move_assignment_operator() {
	adj_matrix_graph initial_graph(3);

	initial_graph[2][1] = true;
	initial_graph[0][1] = true;
	initial_graph[1][2] = true;

	/* ^^^
	0 0 0
	1 0 1
	0 1 0
	*/


	adj_matrix_graph new_graph(1034);
	new_graph = std::move(initial_graph); // Perform move

	std::cout << "Initial graph: " << std::endl << initial_graph << std::endl; // Should give zero-size graph matrix
	std::cout << "New graph: " << std::endl << new_graph << std::endl;

	// Auto Destructors:
	// ...
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////				 Constructors:				////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


inline void test_matrix_graph_copy_constructor() {
	adj_matrix_graph initial_graph(3);

	initial_graph[2][1] = true;
	initial_graph[0][1] = true;
	initial_graph[1][2] = true;

	/* ^^^
	0 0 0
	1 0 1
	0 1 0
	*/


	adj_matrix_graph new_graph (initial_graph); // Perform deep copy

	// Modify initial graph:
	initial_graph[1][0] = true;

	std::cout << "Initial graph: " << std::endl << initial_graph << std::endl;
	std::cout << "New graph: " << std::endl << new_graph << std::endl;

	// Auto Destructors:
	// ...
}

inline void test_matrix_graph_move_constructor() {
	adj_matrix_graph initial_graph(3);

	initial_graph[2][1] = true;
	initial_graph[0][1] = true;
	initial_graph[1][2] = true;

	/* ^^^
	0 0 0
	1 0 1
	0 1 0
	*/


	adj_matrix_graph new_graph(std::move(initial_graph)); // Perform move

	std::cout << "Initial graph: " << std::endl << initial_graph << std::endl; // Should give zero-size graph matrix
	std::cout << "New graph: " << std::endl << new_graph << std::endl;

	// Auto Destructors:
	// ...
}

// TODO: test self assignment!

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

// Adds a new edge to a graph. Assumes that the graph has both the to and from nodes within it
void add_edge(Graph* self, int first_router, int second_router, int speed) {
	EdgeNodePtr new_edge_node; // The new edge node

	// Create new node and assign values
	new_edge_node = malloc(sizeof * new_edge_node);
	new_edge_node->edge.to_router = second_router;
	new_edge_node->edge.speed = speed;
	new_edge_node->next = self->edges[first_router].head;

	self->edges[first_router].head = new_edge_node;
}

Graph* build_graph_from_file(String filepath) {
	FILE* file = fopen(filepath, "r");			// The file to read from
	Graph* new_graph = malloc(sizeof(Graph));   // The new graph
	int first_router;  // The first router from the currently iterated row of the file
	int second_router; // The second router from the currently iterated row of the file
	int speed; // The speed of the connection between the routers from the currently iterated row of the file

	// Stop function if file is not valid
	if (file == NULL) {
		printf("Error opening file!\n");
		return NULL;
	}

	fscanf_s(file, "%d", &new_graph->vertices);

	// Initilise each edge and allocate memory
	new_graph->edges = malloc((sizeof * new_graph->edges) * new_graph->vertices);
	for (int i = 0; i < new_graph->vertices; i++)
	{
		new_graph->edges[i].head = NULL;
	}

	// Add all edges from file
	while (fscanf_s(file, "%d,%d,%d", &first_router, &second_router, &speed) == 3) {
		add_edge(new_graph, first_router, second_router, speed);
	}

	return new_graph;
}

// Tests all functions in this file
void test_graph() {
	String test_file_path = "test_graph.txt"; // The path of the file containing the test graph
	FILE* file = fopen(test_file_path, "r");  // The file containing the test graph
	Graph* testing_graph;	// The graph used for testing this file
	Graph empty_graph;		// An initially empty graph

	EdgeNodePtr current_edge_node; // The edge node currently being iterated over

	// Testing graph values
	int first_router;
	int second_router;
	int speed;


	// Tests
	// ----------------------------------------------------------------------------------------------------------------
	// 1 - Test add_edge()
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n1. add_edge() test\n----------------\n");

	// 1.1 - Test whether the function can add an edge to the graph. This function has no alternative execution paths
	//       and therefore only requires this test

	empty_graph.vertices = 2;

	empty_graph.edges = malloc((sizeof * empty_graph.edges) * empty_graph.vertices);
	for (int i = 0; i < empty_graph.vertices; i++)
	{
		empty_graph.edges[i].head = NULL;
	}

	add_edge(&empty_graph, 0, 1, 2);

	printf("1.1 - Expected Result: Edge from router 0 to router 1 with speed 2\n1.1 - Actual Result: ");
	printf("Edge from router 0 to router %d with speed %d", empty_graph.edges[0].head->edge.to_router, empty_graph.edges[0].head->edge.speed);

	// ----------------------------------------------------------------------------------------------------------------
	// 2 - Test build_graph_from_file()
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n2. build_graph_from_file() test\n----------------\n");

	// 2.1 - Test when the filepath is invalid. This will trigger the first if statement
	printf("2.1 - Expected Result: Error opening file!\n");
	printf("2.1 - Actual Result: ");
	build_graph_from_file("meow.txt");

	// 2.2 - Test when the filepath is valid. This builds the graph and returns it. This function has no other execution paths
	//       so this test is sufficient to test it.
	testing_graph = build_graph_from_file(test_file_path);

	// Skip the first line
	fscanf_s(file, "%d", &speed);

	printf("2.2 - Expected Result:\n");
	while (fscanf_s(file, "%d,%d,%d", &first_router, &second_router, &speed) == 3) {
		printf("Edge between %d and %d with weight of %d\n", first_router, second_router, speed);

	}
	printf("end of graph\n");

	printf("2.2 - Actual Result:\n");
	for (int i = 0; i < testing_graph->vertices; i++) {
		current_edge_node = testing_graph->edges[i].head;
		while (current_edge_node != NULL) {
			printf("Edge between %d and %d with weight of %d\n", i, current_edge_node->edge.to_router, current_edge_node->edge.speed);
			current_edge_node = current_edge_node->next;
		}
	}
	printf("end of graph\n");

}

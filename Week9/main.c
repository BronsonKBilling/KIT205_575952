#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

// Adds a new edge to a graph. Assumes that the graph has both the to and from nodes within it
void add_edge(Graph* self, int from, int to, int w) {
	EdgeNodePtr new_edge_node; // The new edge node

	// Create new node and assign values
	new_edge_node = malloc(sizeof * new_edge_node);
	new_edge_node->edge.to_vertex = to;
	new_edge_node->edge.weight = w;
	new_edge_node->next = self->edges[from].head;

	self->edges[from].head = new_edge_node;
}

// Retrieves the in degrees of each vertex in a graph. Returns an array of ints with the index of each int corresponding
// to the vertex number. The ints stored within the array represent the in degrees of the vertex with the corresponding 
// index number.
int* get_in_degrees(Graph* self) {
	int* in_degrees;		// The in degrees of each vertex
	EdgeNodePtr current;	// The node currently being iterated over

	// Initialise array of in-degrees
	in_degrees = malloc((sizeof * in_degrees) * self->V);
	for (int i = 0; i < self->V; i++)
	{
		in_degrees[i] = 0;
	}

	// Must be a separate for loop to prior one as in degrees must all be initialised prior to looping
	for (int i = 0; i < self->V; i++) {
		current = self->edges[i].head;
		while (current != NULL) {
			in_degrees[current->edge.to_vertex]++;
			current = current->next;
		}
	}

	return in_degrees;
}

// Tests all functions (aside from main) in this file
void test_graph() {
	Graph testing_graph; // The graph used for testing this file
	FILE* file = fopen("testgraph.txt", "r"); // The file contianing the test graph
	int* in_degrees;	   // The in-degree values calculated by the get_in_degrees function
	int* true_in_degrees;  // The actual in-degree values for the testing graph
	EdgeNodePtr current;   // The node currently being iterated over
	int from, to, weight;  // From, to, and weight values extracted from file

	if (file == NULL) {
		printf("Error opening file!\n");
		return;
	}

	// Get vertices
	fscanf_s(file, "%d", &testing_graph.V);

	// Initilise each edge and allocate memory
	testing_graph.edges = malloc((sizeof * testing_graph.edges) * testing_graph.V);
	for (int i = 0; i < testing_graph.V; i++)
	{
		testing_graph.edges[i].head = NULL;
	}

	// Tests
	// ----------------------------------------------------------------------------------------------------------------
	// 1 - Test add_edge()
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n1. add_edge() test\n----------------\n");

	// 1.1 - Test deletion of element that is not the head or tail of the list, this avoids both if statements at the beginning of the
	//		 function, but executes the two if statements under the 'reorder list' comment. This tests deleting from the middle
	add_edge(&testing_graph, 0, 1, 2);

	printf("1.1 - Expected Result: Edge from vertex 0 to vertex 1 with weight 2\n1.1 - Actual Result: ");
	printf("Edge from vertex 0 to vertex %d with weight %d", testing_graph.edges[0].head->edge.to_vertex, testing_graph.edges[0].head->edge.weight);

	// ----------------------------------------------------------------------------------------------------------------
	// 2 - Test get_in_degrees()
	// ----------------------------------------------------------------------------------------------------------------
	printf("\n----------------\n2. get_in_degrees() test\n----------------\n");

	// 2.1 - Test finding the in degrees of each vertex 

	// Configure the true in-degrees
	true_in_degrees = malloc((sizeof * true_in_degrees) * testing_graph.V);
	for (int i = 0; i < testing_graph.V; i++)
	{
		true_in_degrees[i] = 0;
	}

	// Configure the testing graph and get in-degrees
	free(testing_graph.edges[0].head);
	testing_graph.edges[0].head = NULL;

	// Set true in-degrees and add graph edges
	while (fscanf_s(file, "%d,%d,%d", &from, &to, &weight) == 3) {
		true_in_degrees[to] = true_in_degrees[to] + 1;
		add_edge(&testing_graph, from, to, weight);
	}

	in_degrees = get_in_degrees(&testing_graph);

	printf("2.1 - Expected Result:\n");
	for (int i = 0; i < testing_graph.V; i++)
	{
		printf("%d\n", true_in_degrees[i]);
	}
	printf("2.1 - Actual result:\n");
	for (int i = 0; i < testing_graph.V; i++)
	{
		printf("%d\n", in_degrees[i]);
	}

}

int main() {
	Graph G;
	//FILE* file = fopen("graph.txt", "r");  (The original graph.txt file)
	FILE* file = fopen("large_graph.txt", "r");
	int from;
	int to;
	int weight;
	int* in_degrees;
	EdgeNodePtr current;

	// Stop function if file is not valid
	if (file == NULL) {
		printf("Error opening file!\n");
		return;
	}

	fscanf_s(file, "%d", &G.V);

	// Initilise each edge and allocate memory
	G.edges = malloc((sizeof * G.edges) * G.V);
	for (int i = 0; i < G.V; i++)
	{
		G.edges[i].head = NULL;
	}

	// Add all edges from file
	//while (fscanf_s(file, "%d,%d,%d", &from, &to, &weight) == 3) { (For the inital graph.txt file that included edges)
	while (fscanf_s(file, "%d,%d", &from, &to) == 2) {
		add_edge(&G, from, to, 0);
	}

	in_degrees = get_in_degrees(&G);

	printf("In degrees of vertex 5567: %d\n", in_degrees[5567]);
	fclose(file);

	// De-allocate memory
	for (int i = 0; i < G.V; i++)
	{
		G.edges[i].head = NULL;
	}

	test_graph();
	return 0;
}
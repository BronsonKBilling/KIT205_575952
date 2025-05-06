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

// Retrieves the out degrees of each vertex in a graph. Returns an array of ints with the index of each int corresponding
// to the vertex number. The ints stored within the array represent the out degrees of the vertex with the corresponding 
// index number.
int* get_out_degrees(Graph* self) {
	int* out_degrees;	  // The out degrees of each vertex
	EdgeNodePtr current;  // The node currently being iterated over

	// Initialise array of in-degrees
	out_degrees = malloc((sizeof * out_degrees) * self->V);

	for (int i = 0; i < self->V; i++)
	{
		out_degrees[i] = 0;
		current = self->edges[i].head;
		while (current != NULL) {
			out_degrees[i]++;
			current = current->next;
		}
	}

	return out_degrees;
}

// Gets the PageRanks of each vertex in a given graph. Iterates over the algorithm a given number of times and returns
// the results as an array of floats where each index of the array corresponds to a vertex number.
float* get_pageranks(Graph* self, int iterations) {
	const float DAMPENING_FACTOR = 0.5; // The dampening factor of the PageRank function

	float* sums;  // Array of sums
	int* L;		  // Array of out degrees
	float* PR;	  // Array of page ranks
	EdgeNodePtr current; // The currently iterated node

	sums = malloc((sizeof * sums) * self->V);
	PR = malloc((sizeof * PR) * self->V);

	L = get_out_degrees(self);

	// Initialise PageRanks to 1
	for (int i = 0; i < self->V; i++)
	{
		PR[i] = 1;
	}

	for (int iteration = 0; iteration < iterations; iteration++) {
		// 1. Loop through all vertices and set sums to 0
		for (int i = 0; i < self->V; i++)
		{
			sums[i] = 0;
		}

		// 2. Loop through all edges adding the sigma bit to vertex i where i is the to vertex and j is the from vertex
		for (int i = 0; i < self->V; i++) {
			for (int j = 0; j < self->V; j++)
			{
				current = self->edges[j].head;
				while (current != NULL) {
					if (current->edge.to_vertex == i) {
						sums[i] += PR[j] / L[j];
					}
					current = current->next;
				}
			}
		}

		// 3. Loop through all vertices and calculate the new PageRanks by applying the dampening factor
		for (int i = 0; i < self->V; i++) {
			//printf("sum %d: %f\n", i, sums[i]);
			PR[i] = (1 - DAMPENING_FACTOR) + DAMPENING_FACTOR * sums[i];
		}
	}
	
	return PR;
}

// Tests all functions (aside from main) in this file
void test_graph() {
	const float PAGE_RANK_RESULTS[] = { 0.675564, 0.5, 0.578776, 1.459201, 0.972222, 0.625434, 0.748264 }; // Page rank results
	//	gotten from executing the PageRank algorithm on the testing file graph in Excel, in the same way demonstrated in the week 10 tutorial

	Graph testing_graph;	  // The graph used for testing this file
	FILE* file = fopen("testgraph.txt", "r"); // The file contianing the test graph
	int* in_degrees;	   // The in-degree values calculated by the get_in_degrees function
	int* out_degrees;	   // The out-degree values calculated by the get_out_degrees function
	int* true_in_degrees;  // The actual in-degree values for the testing graph
	int* true_out_degrees; // The actual out-degree values for the testing graph
	EdgeNodePtr current;   // The node currently being iterated over
	int from, to, weight;  // From, to, and weight values extracted from file
	float* page_ranks;	   // The page ranks of the week 10 testing graph

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

	// ----------------------------------------------------------------------------------------------------------------
	// 3 - Test get_out_degrees()
	// ----------------------------------------------------------------------------------------------------------------
	printf("\n----------------\n3. get_out_degrees() test\n----------------\n");

	// 3.1 - Test finding the out degrees of each vertex 

	// Configure the true out-degrees
	true_out_degrees = malloc((sizeof * true_out_degrees) * testing_graph.V);
	for (int i = 0; i < testing_graph.V; i++)
	{
		true_out_degrees[i] = 0;
	}

	rewind(file);
	// Skip vertex line
	fscanf_s(file, "%d", &testing_graph.V);

	// Set true out-degrees
	while (fscanf_s(file, "%d,%d,%d", &from, &to, &weight) == 3) {
		true_out_degrees[from] = true_out_degrees[from] + 1;
	}

	out_degrees = get_out_degrees(&testing_graph);

	printf("3.1 - Expected Result:\n");
	for (int i = 0; i < testing_graph.V; i++)
	{
		printf("%d\n", true_out_degrees[i]);
	}
	printf("3.1 - Actual result:\n");
	for (int i = 0; i < testing_graph.V; i++)
	{
		printf("%d\n", out_degrees[i]);

	}

	// ----------------------------------------------------------------------------------------------------------------
	// 4 - Test get_pageranks()
	// ----------------------------------------------------------------------------------------------------------------
	printf("\n----------------\n4. get_pageranks() test\n----------------\n");
	
	// 4.1 - Test finding the pageranks of all vertices in a graph. This graph has a vertex with no in degrees (vertex 1) so this
	//		 tests the case when a vertex has no in degrees. 
	page_ranks = get_pageranks(&testing_graph, 4);

	printf("4.1 - Expected Result:\n");
	for (int i = 0; i < testing_graph.V; i++)
	{
		printf("%f\n", PAGE_RANK_RESULTS[i]);
	}
	printf("4.1 - Actual result:\n");
	for (int i = 0; i < testing_graph.V; i++)
	{
		printf("%f\n", page_ranks[i]);

	}
}

int main() {
	Graph G;
	//FILE* file = fopen("graph.txt", "r");  (The original graph.txt file)
	//FILE* file = fopen("testgraph.txt", "r");
	FILE* file = fopen("week10_test_graph.txt", "r");
	int from;
	int to;
	int weight;
	int* in_degrees;
	int* out_degrees;
	EdgeNodePtr current;
	float* page_ranks;

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
	//while (fscanf_s(file, "%d,%d,%d", &from, &to, &weight) == 3) { //(For the inital graph.txt file that included edges)
	while (fscanf_s(file, "%d,%d", &from, &to) == 2) {
		add_edge(&G, from, to, 0);
	}

	page_ranks = get_pageranks(&G, 5);

	for (int i = 0; i < G.V; i++)
	{
		printf("%f\n", page_ranks[i]);
	}
	fclose(file);

	test_graph();
	return 0;
}
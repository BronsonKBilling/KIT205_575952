#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>

#include "network.h"

// Adds a new link to a network. Assumes that the network has both the to and from nodes within it
void add_link(Network* self, int first_device, int second_device, int speed) {
	LinkNodePtr new_link_node; // The new link node
	LinkNodePtr opposite_link_node; // The new link node to add to the second device

	// Create new node and assign values
	new_link_node = malloc(sizeof * new_link_node);
	new_link_node->link.to_device = second_device;
	new_link_node->link.speed = speed;
	new_link_node->next = self->devices[first_device].links.head;

	// Add the link the other way
	opposite_link_node = malloc(sizeof * opposite_link_node);
	opposite_link_node->link.to_device = first_device;
	opposite_link_node->link.speed = speed;
	opposite_link_node->next = self->devices[second_device].links.head;


	self->devices[first_device].links.head = new_link_node;
	self->devices[second_device].links.head = opposite_link_node;
}

// Builds and returns network from given file
Network* build_network_from_file(String filepath) {
	FILE* file = fopen(filepath, "r");			// The file to read from
	Network* new_network = malloc(sizeof(Network));   // The new network
	int first_device;  // The first device from the currently iterated row of the file
	int second_device; // The second device from the currently iterated row of the file
	int speed; // The speed of the connection between the devices from the currently iterated row of the file

	// Stop function if file is not valid
	if (file == NULL) {
		printf("Error opening file!\n");
		return NULL;
	}

	fscanf_s(file, "%d", &new_network->vertices);

	// Initilise each device and allocate memory
	new_network->devices = malloc((sizeof * new_network->devices) * new_network->vertices);
	for (int i = 0; i < new_network->vertices; i++)
	{
		new_network->devices[i].links.head = NULL;

		new_network->devices[i].routes = malloc((sizeof * new_network->devices[i].routes) * new_network->vertices);
		for (int j = 0; j < new_network->vertices; j++) {			
			new_network->devices[i].routes[j].next_hop = -1;
			new_network->devices[i].routes[j].cost = -1;
		}
		
	}

	// Add all links from file
	while (fscanf_s(file, "%d,%d,%d", &first_device, &second_device, &speed) == 3) {
		add_link(new_network, first_device, second_device, speed);
	}

	return new_network;
}

bool all_devices_known(bool* known_array, int length) {
	bool all_known = true;

	for (int i = 0; i < length; i++)
	{
		if (!known_array[i]) {
			all_known = false;
		}
	}

	return all_known;
}

void build_routing_table_from_distances(Network* self, int* previous, int* distances, int device_index) {
	int current_device; // The currently assessed device

	for (int i = 0; i < self->vertices; i++) {
		if (i == device_index) {
			continue;
		}

		current_device = i;
		
		// Backtrack to just before the parameter device. -1 check is necessary to see if device is unreachable
		while (current_device != -1 && previous[current_device] != device_index) {
			// If there is no valid path
			if (previous[current_device] == -1) {
				current_device = -1;
			}
			else {
				current_device = previous[current_device];
			}
		}

		// If device can be reached
		if (current_device != -1) {
			self->devices[device_index].routes[i].next_hop = current_device;
			self->devices[device_index].routes[i].cost = distances[i];
		}
		else {
			self->devices[device_index].routes[i].next_hop = -1;
			self->devices[device_index].routes[i].cost = -1;
		}
	}
}

// Creates a routing table for each node in the network using Dijkstra's shortest path algorithm
// Assumes that the graph has no negative weights and that it has more than 1 vertex
// Some errors were found and troubleshooted with help from ChatGPT for this function
void find_shortest_paths_dijkstra(Network* self, int device_index) {
	int* distances = malloc((sizeof(int)) * self->vertices); // An array of distances
	bool* known = malloc((sizeof(bool)) * self->vertices); // An array of visitations
	int* previous = malloc((sizeof(int)) * self->vertices); // An array storing the previous hop of each device
	int current_device; // The currently assessed device
	LinkNodePtr current_link;

	// Initialise distances
	for (int i = 0; i < self->vertices; i++)
	{	
		distances[i] = INT_MAX; // Closest we can get to infinity with an INT
		previous[i] = -1;
		known[i] = false;
	}

	distances[device_index] = 0;

	while (!all_devices_known(known, self->vertices)) {
		current_device = -1; // Reset current device

		// Find device with shortest distance that is still unknown and make it the current_device
		for (int i = 0; i < self->vertices; i++)
		{
			// Condition makes it so that if two unknown devices have the same distance, the one that comes first in the
			// array is assessed first
			if (!known[i] && (current_device == -1 || distances[i] < distances[current_device])) {
				current_device = i;
			}
		}

		current_link = self->devices[current_device].links.head;

		// Traverse linked devices and overwrite paths if needed
		while (current_link != NULL) {
			if (
				!known[current_link->link.to_device] && 
				distances[current_device] + current_link->link.speed < distances[current_link->link.to_device]
			) {
				distances[current_link->link.to_device] = distances[current_device] + current_link->link.speed;
				previous[current_link->link.to_device] = current_device;
			}

			current_link = current_link->next;
		}

		known[current_device] = true;
	}

	build_routing_table_from_distances(self, previous, distances, device_index);

	// Free dynamically allocated memory
	free(distances);
	free(known);
	free(previous);
}

// Creates a routing table for each node in the network using the Bellman-Ford shortest path algorithm
// ChatGPT gave basic pseudocode to explain how Bellman-Ford works and was used for debugging.
void find_shortest_paths_bellman_ford(Network* self, int device_index) {
	typedef struct bellmanFordLinkListNode { // A link (edge) in the edge list
		int to_device;
		int from_device;
		int speed;
		struct bellmanFordLinkListNode* next;
	} *BellmanFordLinkListPtr;

	int* distances = malloc((sizeof(int)) * self->vertices); // An array of distances
	int* previous = malloc((sizeof(int)) * self->vertices); // An array storing the previous hop of each device
	LinkNodePtr link_to_add; // The link to add to the link list
	BellmanFordLinkListPtr current_link = NULL; // The link that was most recently created
	BellmanFordLinkListPtr first_link = NULL; // The first link in the edge/link list
	BellmanFordLinkListPtr previous_link = NULL; // The link that was assessed previously
	BellmanFordLinkListPtr new_node; // A new node to add to the list
	
	// Initialise distances
	for (int i = 0; i < self->vertices; i++)
	{
		distances[i] = INT_MAX;
		previous[i] = -1;
	}

	// Build list of edges
	for (int i = 0; i < self->vertices; i++) 
	{
		// Check if device has any links
		if (self->devices[i].links.head != NULL) {
			link_to_add = self->devices[i].links.head;
		} 
		else {
			link_to_add = NULL;
		}

		while (link_to_add != NULL) {
			// Create the new node
			new_node = malloc(sizeof * new_node);
			new_node->from_device = i;
			new_node->to_device = link_to_add->link.to_device;
			new_node->speed = link_to_add->link.speed;
			new_node->next = NULL;

			// If the first link has not been used
			if (current_link == NULL) {
				first_link = new_node;
				current_link = first_link;
			}
			else {
				current_link->next = new_node;
				current_link = current_link->next;
			}

			link_to_add = link_to_add->next;
		}
	}

	distances[device_index] = 0;

	// Run relaxations
	for (int i = 0; i < self->vertices - 1; i++)
	{
		current_link = first_link;
		while (current_link != NULL) {
			if (
				distances[current_link->from_device] != INT_MAX && 
				distances[current_link->from_device] + current_link->speed < distances[current_link->to_device]
			) {
				distances[current_link->to_device] = distances[current_link->from_device] + current_link->speed;
				previous[current_link->to_device] = current_link->from_device;
			}

			current_link = current_link->next;
		}
	}

	// Make unreachable distances equal -1
	for (int i = 0; i < self->vertices; i++) {
		if (distances[i] == INT_MAX) {
			distances[i] = -1;
		}
	}

	build_routing_table_from_distances(self, previous, distances, device_index);

	// Free memory
	current_link = first_link;
	while (current_link != NULL) {
		previous_link = current_link;
		current_link = current_link->next;
		free(previous_link);
	}
	free(distances);
	free(previous);
}

// Builds a routing table for each node in the network using a specified algorithm. 0 is for Dijkstra and 1 is for Bellman-Ford
void build_routing_tables(Network* self, int algorithm) {
	for (int i = 0; i < self->vertices; i++)
	{
		if (algorithm == 0) { // 0 = Dijkstra
			find_shortest_paths_dijkstra(self, i);
		}
		else if (algorithm == 1) { // 1 = Bellman-Ford
			find_shortest_paths_bellman_ford(self, i);
		} 
		else {
			printf("Error: Algorithm is not supported!");
		}
	}
}

// Prints the routing table for a single device, or the routing table for all devices in the network if device_table_to_print = -1
void print_routes(Network* self, int device_table_to_print) {
	int next_hop; // The next hop for the device to print the table of to get to the currently iterated device
	int cost; // The cost to get from the currently iterated device to the device with the table to print

	if (device_table_to_print == -1) {
		for (int i = 0; i < self->vertices; i++)
		{
			print_routes(self, i);
		}
	}
	else {
		for (int i = 0; i < self->vertices; i++)
		{
			next_hop = self->devices[device_table_to_print].routes[i].next_hop;
			cost = self->devices[device_table_to_print].routes[i].cost;
			printf("From device %d to device %d with a cost of %d and a next hop of %d\n", device_table_to_print, i, cost, next_hop);
		}
	}
}

void print_routes_from_test_file(FILE* table_file, int network_devices) {
	int first_device;   // The first device in the route
	int second_device;  // The second device in the route
	int cost;		    // The cost of the route
	int next_hop;		// The next hop of the route

	for (int i = 0; i < network_devices; i++)
	{
		fscanf_s(table_file, "%d,%d,%d,%d", &first_device, &second_device, &next_hop, &cost);
		printf("From device %d to device %d with a cost of %d and a next hop of %d\n", first_device, second_device, cost, next_hop);
	}
}


// Tests all functions in this file
void test_network() {
	const int KNOWN_ARRAY_SIZE = 10; // The size of the 'known' array
	const String TEST_FILE_PATH = "test_graph.txt"; // The path of the file containing the test network
	const String ROUTING_TABLE_FILE_PATH = "test_graph_routing_table.txt"; // The path of the file containing a roruting
																		   // table that corresponds to the testing network
	FILE* file = fopen(TEST_FILE_PATH, "r");  // The file containing the test network
	FILE* routing_table_file = fopen(ROUTING_TABLE_FILE_PATH, "r"); // The file containing the routing table that corresponds to the test network
	Network* testing_network;	// The network used for testing this file
	Network empty_network;		// An initially empty network
	bool* known = malloc((sizeof(bool)) * KNOWN_ARRAY_SIZE); // An array of visitations
	LinkNodePtr current_link_node; // The link node currently being iterated over


	// Testing network values
	int first_device;
	int second_device;
	int speed;

	// Testing network routing table values
	int next_hop;
	int cost;
	String file_header;

	// build_routing_table_from_distances testing variables
	Network* distance_table_network;
	int* distances;
	int* previous;



	// Tests
	// ----------------------------------------------------------------------------------------------------------------
	// 1 - Test add_link()
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n1. add_link() test\n----------------\n");

	// 1.1 - Test whether the function can add an link to the network, and that the link is added in reverse. This function 
	//		 has no alternative execution paths and therefore only requires this test

	empty_network.vertices = 3;

	empty_network.devices = malloc((sizeof * empty_network.devices) * empty_network.vertices);

	empty_network.devices[0].links.head = NULL;
	empty_network.devices[1].links.head = NULL;

	add_link(&empty_network, 0, 1, 2);

	printf("1.1 - Expected Result: Link from device 0 to device 1 with speed 2\n1.1 - Actual Result: ");
	printf("Link from device 0 to device %d with speed %d", empty_network.devices[0].links.head->link.to_device, empty_network.devices[0].links.head->link.speed);
	printf("\n1.1 - Expected Result: Link from device 1 to device 0 with speed 2\n1.1 - Actual Result: ");
	printf("Link from device 1 to device %d with speed %d", empty_network.devices[1].links.head->link.to_device, empty_network.devices[1].links.head->link.speed);

	// ----------------------------------------------------------------------------------------------------------------
	// 2 - Test build_network_from_file()
	// ----------------------------------------------------------------------------------------------------------------
	printf("\n----------------\n2. build_network_from_file() test\n----------------\n");

	// 2.1 - Test when the filepath is invalid. This will trigger the first if statement
	printf("2.1 - Expected Result: Error opening file!\n");
	printf("2.1 - Actual Result: ");
	build_network_from_file("meow.txt");

	// 2.2 - Test when the filepath is valid. This builds the network and returns it. This function has no other execution paths
	//       so this test is sufficient to test it.
	testing_network = build_network_from_file(TEST_FILE_PATH);

	// Skip the first line
	fscanf_s(file, "%d", &speed);

	printf("2.2 - Expected Result:\n");
	while (fscanf_s(file, "%d,%d,%d", &first_device, &second_device, &speed) == 3) {
		printf("Link between %d and %d with weight of %d\n", first_device, second_device, speed);

	}
	printf("end of network\n");

	printf("2.2 - Actual Result:\n");
	for (int i = 0; i < testing_network->vertices; i++) {
		current_link_node = testing_network->devices[i].links.head;
		while (current_link_node != NULL) {
			printf("Link between %d and %d with weight of %d\n", i, current_link_node->link.to_device, current_link_node->link.speed);
			current_link_node = current_link_node->next;
		}
	}
	printf("end of network\n");

	// ----------------------------------------------------------------------------------------------------------------
	// 3 - Test all_devices_known()
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n3. all_devices_known() test\n----------------\n");

	// 3.1 - Test when all values in the array are false. If all values in the array are false then all devices are unknown
	//       meaning the return should also be false
	for (int i = 0; i < KNOWN_ARRAY_SIZE; i++)
	{
		known[i] = false;
	}

	printf("3.1 - Expected Result: false\n");
	printf("%s\n", all_devices_known(known, KNOWN_ARRAY_SIZE) ? "true" : "false");

	// 3.2 - Test when all values in the array are true. If all values in the array are true then all devices are known
	//       meaning the return should also be true
	for (int i = 0; i < KNOWN_ARRAY_SIZE; i++)
	{
		known[i] = true;
	}

	printf("3.2 - Expected Result: true\n");
	printf("%s\n", all_devices_known(known, KNOWN_ARRAY_SIZE) ? "true" : "false");

	// 3.3 - Test the array has a mix of values. If any value in the array is false then all devices are cannot be discovered
	//       meaning the return should be false
	for (int i = 0; i < KNOWN_ARRAY_SIZE; i++)
	{
		if (i % 2 == 0) {
			known[i] = false;
		}
	}

	printf("3.3 - Expected Result: false\n");
	printf("%s\n", all_devices_known(known, KNOWN_ARRAY_SIZE) ? "true" : "false");

	// ----------------------------------------------------------------------------------------------------------------
	// 4 - Test build_routing_table_from_distances()
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n4. build_routing_table_from_distances() test\n----------------\n");

	// 4.1 - Test whether the function can generate a routing table for the network when there are no unreachable nodes.
	//		 This tests all loops, the first else statement and the second if statement within the inner while loop. 
	distance_table_network = build_network_from_file(TEST_FILE_PATH);
	distances = malloc((sizeof(int)) * distance_table_network->vertices);
	previous = malloc((sizeof(int)) * distance_table_network->vertices);

	// Build distances from 0 to other devices and the previous hops of each device
	distances[0] = -1;
	previous[0] = -1;
	distances[1] = 6;
	previous[1] = 2;
	distances[2] = 2;
	previous[2] = 3;
	distances[3] = 1;
	previous[3] = 0;
	distances[4] = 4;
	previous[4] = 2;

	build_routing_table_from_distances(distance_table_network, previous, distances, 0);

	printf("4.1 - Expected Result:\n");
	print_routes_from_test_file(routing_table_file, distance_table_network->vertices);

	printf("4.1 - Actual Result:\n");
	print_routes(distance_table_network, 0);

	// 4.2 - Test whether the function can generate a routing table for the network when there are unreachable nodes.
	//		 This tests all loops, the first if statement and the second else statement within the inner while loop. 

	// Remove link from 1 to 2, making it unreachable
	distances[1] = -1;
	previous[1] = -1;

	build_routing_table_from_distances(distance_table_network, previous, distances, 0);

	printf("4.2 - Expected Result:\n");
	printf("From device 0 to device 1 with a cost of -1 and a next hop of -1");

	printf("\n4.2 - Actual Result:\n");
	printf("From device 0 to device 1 with a cost of %d and a next hop of %d\n", testing_network->devices[0].routes[1].cost, testing_network->devices[0].routes[1].next_hop);


	// ----------------------------------------------------------------------------------------------------------------
	// 5 - Test find_shortest_paths_dijkstra()
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n5. find_shortest_paths_dijkstra() test\n----------------\n");
	// Note: functionality tests for this function were suggested by ChatGPT. Implementation and code path related tests
	//		 were not done by ChatGPT

	// 5.1 - Test whether the function can correctly create a routing table from a network. The testing graph is basic and
	//       does not have multiple paths to the same device. It also does not have any unreachable nodes. This tests all
	//       loops in the function and all 'if' statements aside from the one that checks for invalid paths within the final
	//		 while statement.
	find_shortest_paths_dijkstra(testing_network, 1);

	printf("5.1 - Expected Result:\n");
	print_routes_from_test_file(routing_table_file, testing_network->vertices);

	printf("5.1 - Actual Result:\n");
	print_routes(testing_network, 1);

	// 5.2 - Test when the graph has an unreachable node. This triggers the if statement within the final while statement 
	//       that checks if the device has no path to it. Also triggers the final else statement within the final while statement.
	//       This test and the test above together execute all code paths
	free(testing_network->devices[0].links.head);
	testing_network->devices[0].links.head = NULL;

	// first link node is 3's connection to 2, the second and last is 3's connection to 0
	free(testing_network->devices[3].links.head->next);
	testing_network->devices[3].links.head->next = NULL;

	find_shortest_paths_dijkstra(testing_network, 0);
	find_shortest_paths_dijkstra(testing_network, 3);

	printf("5.2 - Expected Result:\n");
	printf("From device 0 to device 3 with a cost of -1 and a next hop of -1\n");
	printf("From device 3 to device 0 with a cost of -1 and a next hop of -1\n");

	printf("5.2 - Actual Result:\n");
	printf(
		"From device 0 to device 3 with a cost of %d and a next hop of %d\n",
		testing_network->devices[0].routes[3].cost,
		testing_network->devices[0].routes[3].next_hop
	);
	printf(
		"From device 3 to device 0 with a cost of %d and a next hop of %d\n",
		testing_network->devices[3].routes[0].cost,
		testing_network->devices[3].routes[0].next_hop
	);

	// 5.3 - Test when two paths exist to the same device. The algorithm should choose the shortest path.
	//		 1 has a link with 2 with a speed of 4, 2 has a link to 4 with a speed of 2.
	add_link(testing_network, 1, 4, 8);

	find_shortest_paths_dijkstra(testing_network, 1);

	// 1->4 directly costs 8, 1->2->4 costs 6
	printf("5.3 - Expected Result: From device 1 to device 4 with a cost of 6 and a next hop of 2\n");
	printf(
		"5.3 - Actual Result: From device 1 to device 4 with a cost of %d and a next hop of %d\n",
		testing_network->devices[1].routes[4].cost,
		testing_network->devices[1].routes[4].next_hop
	);

	// 5.4 - Test when the network has a cycle in it with each link/edge in the cycle having the same weight. A cycle 
	//		 should not break Dijkstra's algorithm as it can handle them and it should not cause in infinite loop in 
	//       the code.
	empty_network.devices[2].links.head = NULL;
	empty_network.devices[0].routes = malloc((sizeof * empty_network.devices[0].routes) * empty_network.vertices);
	empty_network.devices[1].routes = malloc((sizeof * empty_network.devices[1].routes) * empty_network.vertices);
	empty_network.devices[2].routes = malloc((sizeof * empty_network.devices[2].routes) * empty_network.vertices);
	for (int i = 0; i < empty_network.vertices; i++)
	{
		for (int j = 0; j < empty_network.vertices; j++)
		{
			empty_network.devices[i].routes[j].cost = -1;
			empty_network.devices[i].routes[j].next_hop = -1;
		}
	}

	add_link(&empty_network, 0, 2, 2);
	add_link(&empty_network, 1, 2, 2);
	find_shortest_paths_dijkstra(&empty_network, 0);
	find_shortest_paths_dijkstra(&empty_network, 1);
	find_shortest_paths_dijkstra(&empty_network, 2);

	printf("5.4 - Expected Result:\n");
	for (int i = 0; i < empty_network.vertices; i++)
	{
		for (int j = 0; j < empty_network.vertices; j++)
		{
			// Since each link has a direct path to each node, the next hop should always just be the destination node
			if (i == j) {
				cost = -1;
				next_hop = -1;
			}
			else {
				cost = 2;
				next_hop = j;
			}
			printf("From device %d to device %d with a cost of %d and a next hop of %d\n", i, j, cost, next_hop);	
		}
	}

	printf("5.4 - Actual Result:\n");
	print_routes(&empty_network, -1);

	// ----------------------------------------------------------------------------------------------------------------
	// 6 - Test find_shortest_paths_bellman_ford()
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n6. find_shortest_paths_bellman_ford() test\n----------------\n");

	// 6.1 - Test whether the function can correctly create a routing table from a network. The testing graph is basic and
	//       does not have multiple paths to the same device. It also does not have any unreachable nodes. This tests all
	//       loops in the function and all if and else statements aside from the first else statement in the function

	add_link(testing_network, 0, 3, 1);
	
	find_shortest_paths_bellman_ford(testing_network, 2);
	printf("6.1 - Expected Result:\n");
	// Starts at 1 because the routing table file does not have an entry for a device to itself
	print_routes_from_test_file(routing_table_file, testing_network->vertices);

	printf("6.1 - Actual Result:\n");
	print_routes(testing_network, 2);

	// 6.2 - Test when the graph has an unreachable node. This triggers the first else statment of the function. 
	//		  This test and the test above together execute all code paths

	free(testing_network->devices[0].links.head);
	testing_network->devices[0].links.head = NULL;

	// first link node is 3's connection to 0, this is different to last time as this connection was added most recently
	free(testing_network->devices[3].links.head);
	testing_network->devices[3].links.head = NULL;

	find_shortest_paths_bellman_ford(testing_network, 0);
	find_shortest_paths_bellman_ford(testing_network, 3);

	printf("6.2 - Expected Result:\n");
	printf("From device 0 to device 3 with a cost of -1 and a next hop of -1\n");
	printf("From device 3 to device 0 with a cost of -1 and a next hop of -1\n");

	printf("6.2 - Actual Result:\n");
	printf(
		"From device 0 to device 3 with a cost of %d and a next hop of %d\n",
		testing_network->devices[0].routes[3].cost,
		testing_network->devices[0].routes[3].next_hop
	);
	printf(
		"From device 3 to device 0 with a cost of %d and a next hop of %d\n",
		testing_network->devices[3].routes[0].cost,
		testing_network->devices[3].routes[0].next_hop
	);

	// 6.3 - Test when two paths exist to the same device. The algorithm should choose the shortest path.
	//		 1 has a link with 2 with a speed of 4, 2 has a link to 4 with a speed of 2. in 5.3, 1 was given a
	//		 direct link to 4 with a cost of 8. The algorithm should choose the path of 1->2->4 instead of 1->4

	find_shortest_paths_bellman_ford(testing_network, 1);

	printf("6.3 - Expected Result: From device 1 to device 4 with a cost of 6 and a next hop of 2\n");
	printf(
		"6.3 - Actual Result: From device 1 to device 4 with a cost of %d and a next hop of %d\n",
		testing_network->devices[1].routes[4].cost,
		testing_network->devices[1].routes[4].next_hop
	);

	// 6.4 - Test when the network has a cycle in it with each link/edge in the cycle having the same weight. A cycle 
	//		 should not break the algorithm as it can handle them and it should not cause in infinite loop in 
	//       the code.

	// Empty network gained a cycle in 5.4

	find_shortest_paths_bellman_ford(&empty_network, 0);
	find_shortest_paths_bellman_ford(&empty_network, 1);
	find_shortest_paths_bellman_ford(&empty_network, 2);

	printf("6.4 - Expected Result:\n");
	for (int i = 0; i < empty_network.vertices; i++)
	{
		for (int j = 0; j < empty_network.vertices; j++)
		{
			// Since each link has a direct path to each node, the next hop should always just be the destination node
			if (i == j) {
				cost = -1;
				next_hop = -1;
			}
			else {
				cost = 2;
				next_hop = j;
			}
			printf("From device %d to device %d with a cost of %d and a next hop of %d\n", i, j, cost, next_hop);
		}
	}

	printf("6.4 - Actual Result:\n");
	print_routes(&empty_network, -1);
}


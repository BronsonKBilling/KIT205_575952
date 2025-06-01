#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "network.h"
#include <stdbool.h>

// Adds a new link to a network. Assumes that the network has both the to and from nodes within it
void add_link(Network* self, int first_device, int second_device, int speed) {
	LinkNodePtr new_link_node; // The new link node

	// Create new node and assign values
	new_link_node = malloc(sizeof * new_link_node);
	new_link_node->link.to_device = second_device;
	new_link_node->link.speed = speed;
	new_link_node->next = self->devices[first_device].links.head;

	self->devices[first_device].links.head = new_link_node;
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
			new_network->devices[i].routes[j].next_hop = NULL;
			new_network->devices[i].routes[j].cost = NULL;
		}
		
	}

	// Add all links from file
	while (fscanf_s(file, "%d,%d,%d", &first_device, &second_device, &speed) == 3) {
		add_link(new_network, first_device, second_device, speed);
	}

	return new_network;
}

bool all_devices_known(int* known_array, int length) {
	bool all_known = true;

	for (int i = 0; i < length; i++)
	{
		if (!known_array[i]) {
			all_known = false;
		}
	}

	return all_known;
}

// Creates a routing table for each node in the network using Dijkstra's shortest path algorithm
void find_shortest_paths_dijkstra(Network* self, int device_index) {
	int* distances = malloc((sizeof(int)) * self->vertices); // An array of distances
	int* known = malloc((sizeof(bool)) * self->vertices); // An array of visitations
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

	// Build routing table from previous[]
	for (int i = 0; i < self->vertices; i++) {
		if (i == device_index) {
			continue;
		}

		current_device = i;

		while (current_device != -1 && previous[current_device] != device_index) {
			// If there is no valid path
			if (previous[current_device] == -1) {
				current_device = -1;
			}
			else {
				current_device = previous[current_device];
			}
			
		}

		if (current_device != -1) {
			self->devices[device_index].routes[i].next_hop = current_device;
			self->devices[device_index].routes[i].cost = distances[i];
		}
	}

	// Free dynamically allocated memory
	free(distances);
	free(known);
	free(previous);
}

// Creates a routing table for each node in the network using the Bellman-Ford shortest path algorithm
//void find_shortest_paths_bellman_ford(Network* self, int device_index) {
//
//}

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


// Tests all functions in this file
void test_network() {
	const int KNOWN_ARRAY_SIZE = 10; // The size of the 'known' array

	String test_file_path = "test_graph.txt"; // The path of the file containing the test network
	FILE* file = fopen(test_file_path, "r");  // The file containing the test network
	Network* testing_network;	// The network used for testing this file
	Network empty_network;		// An initially empty network
	int* known = malloc((sizeof(bool)) * KNOWN_ARRAY_SIZE); // An array of visitations
	LinkNodePtr current_link_node; // The link node currently being iterated over

	// Testing network values
	int first_device;
	int second_device;
	int speed;


	// Tests
	// ----------------------------------------------------------------------------------------------------------------
	// 1 - Test add_link()
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n1. add_link() test\n----------------\n");

	// 1.1 - Test whether the function can add an link to the network. This function has no alternative execution paths
	//       and therefore only requires this test

	empty_network.vertices = 2;

	empty_network.devices = malloc((sizeof * empty_network.devices) * empty_network.vertices);

	empty_network.devices[0].links.head = NULL;

	add_link(&empty_network, 0, 1, 2);

	printf("1.1 - Expected Result: Link from device 0 to device 1 with speed 2\n1.1 - Actual Result: ");
	printf("Link from device 0 to device %d with speed %d", empty_network.devices[0].links.head->link.to_device, empty_network.devices[0].links.head->link.speed);

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
	testing_network = build_network_from_file(test_file_path);

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


}


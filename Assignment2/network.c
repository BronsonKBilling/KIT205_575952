#include <stdio.h>
#include <stdlib.h>
#include "network.h"

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

// Creates a routing table for each node in the network
void build_routing_tables(Network* self) {

}


// Tests all functions in this file
void test_network() {
	String test_file_path = "test_graph.txt"; // The path of the file containing the test network
	FILE* file = fopen(test_file_path, "r");  // The file containing the test network
	Network* testing_network;	// The network used for testing this file
	Network empty_network;		// An initially empty network

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

#pragma once

// Code is derived from week 9/10 tutorials

typedef char* String;

/**
 * @struct link
 * @brief Represents a link that a device has within the adjacency list. 
 *
 * Contains the index of the device the link goes to and the speed of the link
 */
typedef struct link {
	int to_device;
	int speed;
} Link;

/**
 * @struct linkNode
 * @brief Represents a node that represents a link within the adjacency list
 *
 * Contains the link and a pointer to the next link
 */
typedef struct linkNode {
	Link link;
	struct linkNode* next;
} *LinkNodePtr;

/**
 * @struct linkList
 * @brief Represents a list of all the links that a device has.
 *
 * Contains the head of the list of links
 */
typedef struct linkList {
	LinkNodePtr head;
} LinkList;

/**
 * @struct route
 * @brief Represents a route that a device has to another device
 *
 * Contains the index of the device the link goes to and the speed of the link
 */
typedef struct route {
	int next_hop;
	int cost;
} Route;

/**
 * @struct device
 * @brief Represents a device connected to the network
 *
 * Contains lists of a device's links and its routes
 */
typedef struct device {
	LinkList links;
	Route* routes;
} Device;

/**
 * @struct network
 * @brief Represents a TCP/IP network
 *
 * Contains the number of devices the network has and a list of the links that each device has
 */
typedef struct network {
	int vertices;
	Device* devices;
} Network;

/**
 * @brief Adds a link to the network that goes from a given device to another with a given speed
 *
 * @param self Network to add the device to
 * @param first_device One of the devices (nodes) that the link includes 
 * @param second_device The other device (node) that the link includes 
 * @param weight The speed of the connection (weight)
 *
 */
void add_link(Network* self, int first_device, int second_device, int speed);

/**
 * @brief Builds and allocates memory for a network from a file of links. Returns the network.
 *
 * @param filepath The path of the file to use
 *
 * @return Pointer to the new network created from the file
 */
Network* build_network_from_file(String filepath);

/**
 * @brief Tests all of the functions within this file
 */
void test_network();
#pragma once

// Code is derived from week 9/10 tutorials

typedef char* String;

/**
 * @struct edge
 * @brief Represents an edge that a vertex has within the adjacency list. 
 *
 * Contains a the index of the router the edge goes to and the weight of the edge
 */
typedef struct edge {
	int to_router;
	int speed;
} Edge;

/**
 * @struct edgeNode
 * @brief Represents a node that represents an edge within the adjacency list
 *
 * Contains the edge and a pointer to the next edge
 */
typedef struct edgeNode {
	Edge edge;
	struct edgeNode* next;
} *EdgeNodePtr;

/**
 * @struct edgeList
 * @brief Represents a list of all the edges that a node has.
 *
 * Contains the head of the edge list
 */
typedef struct edgeList {
	EdgeNodePtr head;
} EdgeList;

/**
 * @struct graph
 * @brief Represents a graph
 *
 * Contains the number of vertices the graph has and a list of the edges that each vertice has
 */
typedef struct graph {
	int vertices;
	EdgeList* edges;
} Graph;

/**
 * @brief Adds an edge to the graph that goes from a given vertex to another with a given weight
 *
 * @param self Graph to add the edge to
 * @param first_router One of the routers (nodes) that the edge includes 
 * @param second_router The other router (node) that the edge includes 
 * @param weight The speed of the connection (weight)
 *
 */
void add_edge(Graph* self, int first_router, int second_router, int speed);

/**
 * @brief Builds and allocates memory for a graph from a file of edges. Returns the graph
 *
 * @param filepath The path of the file to use
 *
 * @return Pointer to the new graph created from the file
 */
Graph* build_graph_from_file(String filepath);

/**
 * @brief Tests all of the functions within this file
 */
void test_graph();
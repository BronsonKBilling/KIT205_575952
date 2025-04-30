// linked_list.h
#pragma once

#include "record.h"

// Code is derivative of Week 6 tutorial

typedef char* String;

/**
 * @struct listNode
 * @brief Represents a single node in a linked list
 *
 * Contains a record data item and pointers to the previous and next nodes
 */
typedef struct listNode {
	Record* data; 
	struct listNode* next;
	struct listNode* prev;
} *ListNodePtr;

/**
 * @struct list
 * @brief Represents a doubly linked list
 *
 * Contains pointers to the head and tail of the list
 */
typedef struct list {
	ListNodePtr head;
	ListNodePtr tail;
} List;

/**
 * @brief Creates a new, empty list
 *
 * @return The new list
 */
List create_list();

/**
 * @brief Inserts a record into the list
 *
 * @param self The list to insert the record into
 * @param data The record to insert into the list
 */
void insert_list(List* self, Record* data);

/**
 * @brief Deletes a node from the list
 *
 * @param self The list to delete from
 * @param node_to_delete The node to delete form the list
 */
void delete_node(List* self, ListNodePtr node_to_delete);

/**
 * @brief Prints the identifiers of all records held in the list
 *
 * @param self The list to print
 */
void print_list(List* self);

/**
 * @brief Finds a record by its identifier within the list and returns a pointer to the node containing it
 *
 * @param self The list to search
 * @param identifier The identifier item to search for
 * @param type The type of the identifier
 *
 * @return Pointer to the node with the given record, or NULL if it is not found
 */
ListNodePtr find_list(List* self, void* identifier, IdentifierType type);

/**
 * @brief Tests all functions within the file
 */
void test_list();
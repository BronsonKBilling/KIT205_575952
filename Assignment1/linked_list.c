// linked_list.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record.h"
#include "linked_list.h"


// Creates and returns a new list
List create_list() {
	List new_list;
	new_list.head = NULL;
	new_list.tail = NULL;
	return new_list;
}

// Prints every element of a list in order
void print_list(List* self) {
	ListNodePtr current = self->head; // The current node being iterated over

	while (current != NULL) {
		print_record(current->data);
		current = current->next;
		printf("\n");
	}
}

// Inserts a record at the head of the list
void insert_at_front(List* self, Record* data) {
	ListNodePtr new_node = malloc(sizeof * new_node);

	new_node->data = data;

	new_node->next = self->head;
	self->head = new_node;
}

// Inserts a record at the tail of the list
void insert_at_rear(List* self, Record* data) {
	ListNodePtr new_node = malloc(sizeof * new_node);

	new_node->data = data;

	new_node->prev = self->tail;
	self->tail = new_node;
}

// Inserts a record in the middle of the list, starting traversal at the front
void insert_at_middle_from_front(List* self, Record* data) {
	ListNodePtr new_node = malloc(sizeof * new_node);
	ListNodePtr current = self->head;
	ListNodePtr previous;

	while (compare_records(current->data, data) != -1) {
		previous = current;
		current = current->next;
	}

	new_node->next = current;
	new_node->prev = previous;

	previous->next = new_node;
	current->prev = new_node;

}

// Inserts a record in the middle of the list, starting traversal at the rear
void insert_at_middle_from_rear(List* self, Record* data) {
	ListNodePtr new_node = malloc(sizeof * new_node);
	ListNodePtr current = self->tail;
	ListNodePtr next;

	while (compare_records(current->data, data) != 1) {
		next = current;
		current = current->next;
	}

	new_node->next = next;
	new_node->prev = current;

	current->next = new_node;
	next->prev = new_node;

}

// Inserts a new record into the list. Assumes that the record to be inserted is not a duplicate of any other record
// in self. Duplicates within a nested list are fine. Comparisons only factor in record identifiers, data is handled
// by the database. This function also assumes that the data types are the same between the List and the new record.
// If types are different then this issue will be handled downstream in the 'compare_records() function
void insert(List* self, Record* data) {
	if (compare_records(self->head->data, data) == -1) {
		insert_at_front(self, data);
	}
	else if (compare_records(self->tail->data, data) == 1) {
		insert_at_rear(self, data);
	}
	else if (self->tail->data->identifier_type == IT_INT && *(int*)self->tail->data->identifier / 2 < data->identifier) {
		insert_at_middle_from_rear(self, data);
	}
	else {
		insert_at_middle_from_front(self, data);
	}
}

// Tests all above functions
void test_list() {
	List test = create_list();

	// 1 - Test 'create_list()'
	printf("----------------\n1. create_list() test\n----------------\n");

}

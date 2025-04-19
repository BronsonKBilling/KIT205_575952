// linked_list.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "record.h"

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
		print_record(&current->data);
		current = current->next;
		printf("\n");
	}
}

// Prints every record of a list that is associated with the given record
void print_one_to_many(List* self, Record* match) {

}

// Inserts a new record into the list
void insert(List* self, Record* data) {

}

// Tests all above functions
void test_list() {

}

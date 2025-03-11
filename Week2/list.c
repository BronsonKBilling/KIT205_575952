#include <stdio.h>
#include <stdlib.h>
#include "list.h"


// Function to create and return a new empty list.
List new_list() {
	List temp;
	temp.head = NULL;
	return temp;
}

// Function to print the contents of a list.
void print_list(List* self) {
	ListNodePtr current = self->head;
	while (current != NULL) {
		printf("%d", current->data);
		current = current->next;

		if (current != NULL)
			printf(", ");
	}
	printf("\n");
}

// Function to insert a new node with given data at the front of the list.
void insert_at_front(List* self, int data) {
	ListNodePtr new_node = malloc(sizeof * new_node);

	new_node->data = data;
	new_node->next = self->head;
	self->head = new_node;
}

// Function to insert a new node with given data in the list in ascending order.
void insert_in_order(List* self, int data) {
	ListNodePtr current = self->head;
	ListNodePtr prev = NULL;

	ListNodePtr new_node = malloc(sizeof * new_node);
	new_node->data = data;
	new_node->next = NULL;

	while (current != NULL && current->data < data) {
		prev = current;
		current = current->next;
	}

	if (current == self->head) { // at front
		new_node->next = current;
		self->head = new_node;
	}
	else {                     // middle
		new_node->next = current;
		prev->next = new_node;
	}
}

// Function to delete the first occurrence of a node with given data from the list.
void delete_list(List* self, int data) {
	ListNodePtr current = self->head;
	ListNodePtr prev = NULL;

	while (current != NULL) {
		if (current->data == data) {
			if (prev == NULL) {        // front of list
				self->head = current->next;
				free(current);
				current = self->head;
			}
			else {                    // middle of list
				prev->next = current->next;
				free(current);
				current = prev->next;
			}
		}
		else {
			prev = current;
			current = current->next;
		}
	}
}

// Function to free the memory allocated to a list and all its nodes.
void destroy_list(List* self) {
	ListNodePtr current = self->head;
	while (current != NULL) {
		ListNodePtr to_free = current;
		current = current->next;
		free(to_free);
	}
	self->head = NULL;
}

// Reverses the order of a list. Algorithm based off of: https://www.geeksforgeeks.org/reverse-a-linked-list/
void reverse(List* self) {
	ListNodePtr prev = NULL;
	ListNodePtr current = self->head;
	ListNodePtr next = NULL;

	while (current != NULL) {
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}

	self->head = prev;
}

// Merges two ordered lists. Modifies the first list to become the merged list (the hard way, not working)
//void merge(List* self, List* list_to_merge) {
//	List merged_list = new_list();
//	ListNodePtr current_merged = merged_list.head;
//	ListNodePtr current_self = self->head;
//	ListNodePtr current_to_merge = list_to_merge->head;
//	ListNodePtr new_node;
//	int data_to_add;
//
//	while (current_self != NULL && current_to_merge != NULL) {
//		// Find which list to add from and advance the list
//		if (current_self == NULL || current_self->data > current_to_merge->data)
//		{
//			data_to_add = current_to_merge->data;
//			current_to_merge = current_to_merge->next;
//		}
//		else {
//			data_to_add = current_self->data;
//			current_self = current_self->next;
//		}
//		printf("meow");
//		// Add to the new list
//		new_node = malloc(sizeof * new_node);
//		new_node->data = data_to_add;
//
//		if (merged_list.head == NULL) {
//			merged_list.head = new_node;
//			current_merged = merged_list.head;
//		}
//		else {
//			current_merged->next = new_node;
//		}
//	}
//	print_list(&merged_list);
//	self->head = merged_list.head;
//}

// Merges two ordered lists. Modifies the first list to become the merged list
void merge(List* self, List* list_to_merge) {
	List merged_list = new_list();
	ListNodePtr current_self = self->head;
	ListNodePtr current_to_merge = list_to_merge->head;
	
	while (current_self != NULL) {
		insert_in_order(&merged_list, current_self->data);
		current_self = current_self->next;
	}

	while (current_to_merge != NULL) {
		insert_in_order(&merged_list, current_to_merge->data);
		current_to_merge = current_to_merge->next;
	}

	self->head = merged_list.head;
}

// Function to test the list's functions
void list_test() {
	List test_list = new_list();

	printf("Testing insert_at_front... \n");

	insert_at_front(&test_list, 5);
	insert_at_front(&test_list, 3);
	insert_at_front(&test_list, 7);
	insert_at_front(&test_list, 2);
	insert_at_front(&test_list, 0);

	printf("Expected: 0, 2, 7, 3, 5\n");
	printf("Result: ");

	print_list(&test_list);
}

// Tests the 'insert_at_front()' function
void option_insert(List* self) {
	int int_to_add;

	printf("Please enter a number to add to the list: ");
	scanf_s("%d", &int_to_add);

	insert_at_front(self, int_to_add);
}

// Tests the 'delete_list()' function
void option_delete(List* self) {
	int int_to_delete;

	printf("Please enter a number to delete from the list: ");
	scanf_s("%d", &int_to_delete);

	delete_list(self, int_to_delete);
}

// Tests the 'print_list()' function
void option_print(List* self) {
	print_list(self);
}

void option_reverse(List* self) {
	reverse(self);
}

void option_merge(List* self) {
	// code to test the merge function
	List test_list = new_list();
	insert_in_order(&test_list, 1);
	insert_in_order(&test_list, 4);
	insert_in_order(&test_list, 9);
	insert_in_order(&test_list, 2);
	insert_in_order(&test_list, 3);

	merge(self, &test_list);
}

// Tests inserting in order into a List, and destroying the list
//void ad_hoc_test() {
//	List test_list = new_list();
//	int int_to_add;
//
//	scanf_s("%d", &int_to_add);
//
//	while (int_to_add != 0) {
//		insert_in_order(&test_list, int_to_add);
//		scanf_s("%d", &int_to_add);
//	}
//
//	print_list(&test_list);
//
//	destroy_list(&test_list);
//}

// Tests inserting in order into a List, and destroying the list
void ad_hoc_test() {
	List my_list = new_list();
	int quit = 0;

	while (!quit) {
		int option;

		printf("Please select a function to test:\n1. Insert\n2. Delete\n3. Print\n4. Reverse\n5. Merge\n0. Quit\n");
		scanf_s("%d", &option);

		switch (option)
		{
			case 0:
				quit = 1;
				break;
			case 1:
				option_insert(&my_list);
				break;
			case 2:
				option_delete(&my_list);
				break;
			case 3:
				option_print(&my_list);
				break;
			case 4:
				option_reverse(&my_list);
				break;
			case 5:
				option_merge(&my_list);
		}
	}

	destroy_list(&my_list);
}

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

// Creates and returns a new list node pointer
ListNodePtr create_node() {
	ListNodePtr new_node = malloc(sizeof * new_node);
	new_node->data = NULL;
	new_node->next = NULL;
	new_node->prev = NULL;

	return new_node;
}

// Inserts a record at the head of the list
void insert_at_front(List* self, Record* data) {
	ListNodePtr new_node = create_node();
	Record* new_record = clone_record(data);

	new_node->data = new_record;

	// If the list is empty then the tail and head should both be equal to the new node
	if (self->head != NULL) {
		self->head->prev = new_node;
	}
	else {
		self->tail = new_node;
	}

	new_node->next = self->head;
	self->head = new_node;

}

// Inserts a record at the tail of the list
void insert_at_rear(List* self, Record* data) {
	ListNodePtr new_node = create_node();
	Record* new_record = clone_record(data);

	new_node->data = new_record;

	if (self->tail != NULL) {
		self->tail->next = new_node;
	}
	else {
		self->head = new_node;
	}

	new_node->prev = self->tail;
	self->tail = new_node;
	
}

// Inserts a record in the middle of the list, starting traversal at the front. This function assumes that the list 
// has at least 2 records within it. This function will only ever be called when the list has at least 2 elements.
// The function also assumes that items are sorted in ascending order.
void insert_at_middle_from_front(List* self, Record* data) {
	ListNodePtr new_node = create_node();
	Record* new_record = clone_record(data);
	ListNodePtr current = self->head;
	ListNodePtr previous = NULL;

	new_node->data = new_record;

	while (compare_records(current->data, data) != 1) {
		previous = current;
		current = current->next;
	}

	new_node->next = current;
	new_node->prev = previous;

	previous->next = new_node;
	current->prev = new_node;

}

// Inserts a record in the middle of the list, starting traversal at the rear. This function assumes that the list 
// has at least 2 records within it. This function will only ever be called when the list has at least 2 elements.
// The function also assumes that items are sorted in ascending order.
void insert_at_middle_from_rear(List* self, Record* data) {
	ListNodePtr new_node = create_node();
	Record* new_record = clone_record(data);
	ListNodePtr current = self->tail;
	ListNodePtr next = NULL;

	new_node->data = new_record;

	while (compare_records(data, current->data) != 1) {
		next = current;
		current = current->prev;
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

	if (self->head == NULL || compare_records(self->head->data, data) == 1) {
		insert_at_front(self, data);
	}
	else if (compare_records(self->tail->data, data) == -1) {
		insert_at_rear(self, data);
	}
	else if (self->tail->data->identifier_type == IT_INT && *(int*)self->tail->data->identifier / 2 < data->identifier) {
		insert_at_middle_from_rear(self, data);
	}
	else {
		insert_at_middle_from_front(self, data);
	}
}

// Deletes a node within the list, reshuffles the list and changes head and tail if necessary
void delete_node(List* self, ListNodePtr node_to_delete) {
	// Fix head & tail
	if (self->head == node_to_delete) {
		self->head = self->head->next;
	}
	if (self->tail == node_to_delete) {
		self->tail = self->tail->prev;
	}

	// Reorder list
	if (node_to_delete->prev != NULL) {
		node_to_delete->prev->next = node_to_delete->next;
	}
	if (node_to_delete->next != NULL) {
		node_to_delete->next->prev = node_to_delete->prev;
	}

	destroy_record(node_to_delete->data);
	free(node_to_delete);
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

// Tests all above functions
void test_list() {
	List testing_list = create_list();
	Record testing_record = create_record();

	// Note about testing insertion: There is only 1 public insert function, the rest are helper functions. The
	// public insert function cannot by itself insert anything without the help of the private helper functions.
	// To avoid confudion, the helper functions will be tested directly rather than being called through the
	// public function. The public function will also be tested. 

	// Note about testing order: While the functions are mostly tested in the order that they appear in the file,
	// some functions need to be executed to accurately test others. This means that for some functions, it is
	// more efficient to test them out of the order that they appear in the file.

	// Note about testing print_list: Testing this function would simply involve printing a list with no elements,
	// then printing a list with elements, which is doen multiple times in the below tests. Therefore further
	// tests of this function are not necessary

	// Note about tests that return true or false: For a test to pass (exhibit intended behaviour) the result must be
	// true. Any false values are considered a failed test.

	// 1 - Test 'create_list()'
	printf("----------------\n1. create_list() test\n----------------\n");

	// 1.1 - Tests if the function can create and return a linked list without throwing any errors
	testing_list = create_list();
	printf("1.1 - Test passed! List successfully created!\n");

	// 2 - Test insert_at_front()
	printf("----------------\n2. insert_at_front() test\n----------------\n");

	// 2.1 - Tests insertion at front when there are no elements in the list, tests the 'else' statement
	change_int_identifier(&testing_record, 315);
	testing_record.identifier_type = IT_INT;
	insert_at_front(&testing_list, &testing_record);

	printf("2.1 - Expected Result: 315\n2.1 - Actual Result: ");
	print_list(&testing_list);
	
	// As there is only 1 element, the head and tail should point to the same node
	printf("2.1 - head == tail? ");
	printf("%s\n", testing_list.head == testing_list.tail ? "true" : "false");

	// 2.2 - Tests insertion at front when there elements in the list (self->head != NULL). This tests the 'if' statement
	change_int_identifier(&testing_record, 316);
	insert_at_front(&testing_list, &testing_record);

	printf("2.2 - Expected Result:\n316\n315\n2.2 - Actual Result:\n");
	print_list(&testing_list);

	// As there are now 2 elements, the head and tail should point to different nodes
	printf("2.2 - head == tail? ");
	printf("%s\n", testing_list.head == testing_list.tail ? "true" : "false");

	// 3 - Test delete_node()
	printf("----------------\n3. delete_node() test\n----------------\n");

	// 3.1 - Test deletion of element that is not the head or tail of the list, this avoids both if statements at the beginning of the
	//		 function, but executes the two if statements under the 'reorder list' comment. This tests deleting from the middle
	change_int_identifier(&testing_record, 317);
	insert_at_front(&testing_list, &testing_record);
	delete_node(&testing_list, testing_list.head->next);

	printf("3.1 - Expected Result:\n317\n315\n3.1 - Actual Result:\n");
	print_list(&testing_list);

	// 3.2 - Test deletion of element at the head of the list. This will test the first if statement of the funciton and
	//       the second if statement under the 'reorder list' comment. It will trigger no others. The head should now be 
	//		 equal to the tail.
	delete_node(&testing_list, testing_list.head);

	printf("3.2 - Expected Result: 315\n3.2 - Actual Result: ");
	print_list(&testing_list);
	printf("3.2 - Head and tail are equal? ");
	printf("%s\n", testing_list.head == testing_list.tail ? "true" : "false");

	// 3.3 - Test deletion of element at the tail of the list, when there are more than just 1 element. This will test the
	//		 second if statement within th efunction, and the first if statement under the 'reorder list' heading. The head
	//		 should be equal to the tail after the deletion.
	insert_at_front(&testing_list, &testing_record);
	delete_node(&testing_list, testing_list.tail);

	printf("3.3 - Expected Result: 317\n3.3 - Actual Result: ");
	print_list(&testing_list);
	printf("3.3 - Head and tail are equal? ");
	printf("%s\n", testing_list.head == testing_list.tail ? "true" : "false");

	// 3.4 - Test deletion of an element when it is alone in the list. This should trigger both if statements at the beginning
	//		 of the function, causing the tail and head to == NULL. Both if statements under the 'reorder list' heading will
	//		 not be triggered. The head and tail should be equal and should both equal NULL.

	// It does not matter whether the head or tail is deleted
	delete_node(&testing_list, testing_list.head);

	printf("3.4 - Expected Result: \n3.4 - Actual Result: ");
	print_list(&testing_list);
	printf("\n3.4 - Head and tail are equal? ");
	printf("%s\n", testing_list.head == testing_list.tail ? "true" : "false");
	printf("3.4 - Head and tail are equal to NULL? ");
	printf("%s\n", testing_list.head == NULL && testing_list.tail == NULL ? "true" : "false");

	// 4 - Test insert_at_rear()
	printf("----------------\n4. insert_at_rear() test\n----------------\n");

	// 4.1 - Tests insertion at rear when there are no elements in the list. Tests the else statement
	//delete_node(testing_list.head);
	//testing_list.head = NULL;
	//delete_node(testing_list.tail);
	testing_list = create_list();

	insert_at_rear(&testing_list, &testing_record);

	printf("4.1 - Expected Result: 317\n4.1 - Actual Result: ");
	print_list(&testing_list);

	// As there is only 1 element, the head and tail should point to the same node
	printf("4.1 - head == tail? ");
	printf("%s\n", testing_list.head == testing_list.tail ? "true" : "false");

	// 4.2 - Tests insertion at rear when there are elements in the list (self->tail != NULL). Tests the if statement
	change_int_identifier(&testing_record, 318);
	insert_at_rear(&testing_list, &testing_record);

	printf("4.2 - Expected Result:\n317\n318\n4.2 - Actual Result:\n");
	print_list(&testing_list);

	// As there are now 2 elements, the head and tail should point to different nodes
	printf("4.2 - head == tail? ");
	printf("%s\n", testing_list.head == testing_list.tail ? "true" : "false");

	// 5 - Test insert_at_middle_from_front()
	printf("----------------\n5. insert_at_middle_from_front() test\n----------------\n");

	// 5.1 - Test inserting a node. Testing insertion when the list is empty or has less than 2 elements is not necessary
	//		 as the function assumes that the list has at least 2 elements. The function is private and will never be called
	//		 unless this is the case. This means that this is the only necessary test for this function
	delete_node(&testing_list, testing_list.tail);
	delete_node(&testing_list, testing_list.tail);

	change_int_identifier(&testing_record, 4);
	insert_at_front(&testing_list, &testing_record);
	change_int_identifier(&testing_record, 1);
	insert_at_front(&testing_list, &testing_record);
	change_int_identifier(&testing_record, 2);
	insert_at_middle_from_front(&testing_list, &testing_record);

	printf("5.1 - Expected Result:\n1\n2\n4\n5.1 - Actual Result:\n");
	print_list(&testing_list);

	// 6 - Test insert_at_middle_from_front()
	printf("----------------\n6. insert_at_middle_from_rear() test\n----------------\n");

	// 6.1 - Test inserting a node. Testing insertion when the list is empty or has less than 2 elements is not necessary
	//		 as the function assumes that the list has at least 2 elements. The function is private and will never be called
	//		 unless this is the case. This means that this is the only necessary test for this function
	change_int_identifier(&testing_record, 3);
	insert_at_middle_from_rear(&testing_list, &testing_record);

	printf("6.1 - Expected Result:\n1\n2\n3\n4\n6.1 - Actual Result:\n");
	print_list(&testing_list);

	// 7 - Test insert()
	printf("----------------\n7. insert() test\n----------------\n");

	// 7.1 - Test inserting when the list is empty. This tests the first if statement as self->head == NULL

	delete_node(&testing_list, testing_list.head);
	delete_node(&testing_list, testing_list.head);
	delete_node(&testing_list, testing_list.head);
	delete_node(&testing_list, testing_list.head);
	change_int_identifier(&testing_record, 2);
	insert(&testing_list, &testing_record);

	printf("7.1 - Expected Result: 2\n7.1 - Actual Result: ");
	print_list(&testing_list);

	// 7.2 - Test when the record to insert comes before the head of the list. This also triggers the first if statement.
	change_int_identifier(&testing_record, 1);
	insert(&testing_list, &testing_record);

	printf("7.2 - Expected Result:\n1\n2\n7.2 - Actual Result:\n");
	print_list(&testing_list);

	// 7.3 - Test when the record to insert comes after the tail of the list. This triggers the first else if statement.
	change_int_identifier(&testing_record, 10);
	insert(&testing_list, &testing_record);

	printf("7.3 - Expected Result:\n1\n2\n10\n7.3 - Actual Result:\n");
	print_list(&testing_list);

	// 7.4 - Test when the record to insert is larger than half of the tail. This triggers the second else if statement.
	change_int_identifier(&testing_record, 6);
	insert(&testing_list, &testing_record);

	printf("7.4 - Expected Result:\n1\n2\n6\n10\n7.4 - Actual Result:\n");
	print_list(&testing_list);

	// 7.5 - Test when the record to insert is smaller than half of the tail. This triggers the final else statement.
	change_int_identifier(&testing_record, 3);
	insert(&testing_list, &testing_record);

	printf("7.5 - Expected Result:\n1\n2\n3\n6\n10\n7.5 - Actual Result:\n");
	print_list(&testing_list);

}

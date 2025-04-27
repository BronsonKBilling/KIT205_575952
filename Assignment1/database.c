#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "record.h"
#include "linked_list.h"
#include "avl_tree.h"
#include "database.h"

// NOTE: as the two prototypes differ in just the internal datastructure, not the outer one, the different prototypes
// will be refered to by their internal type. So for prototype 1 that is a linked list of a linked list, that will
// just be referred to as a 'list'. For prototype 2, which is a linked list containing avl trees, this will be
// referred to as 'avl'

void print_events(List* database) {
	print_list(database);
}

void print_fighters_in_event(List* database, int event_code, Prototype prototype) {
	ListNodePtr target_event_node;				// The event which should have all fighters printed

	target_event_node = find_list(database, &event_code, IT_INT);

	if (target_event_node->data->data == NULL) {
		printf("This event has no fighters listed yet!");
	}
	else if (prototype == PROTOTYPE_UNSET) {
		printf("ERROR! No prototype set");
		//ERR
	}
	else if (prototype == LINKED_LIST_LINKED_LIST) {
		print_list(target_event_node->data->data);
	}
	else {
		print_avl_without_tree(target_event_node->data->data);
	}
}

void add_fighter_list(List* database, int event_code, String fighter_name) {
	ListNodePtr target_event_node;				// The event to insert the fighter into
	Record* fighter = malloc(sizeof(Record));   // The new fighter record to be placed in the list
	List* new_list;								// Variable that holds the new list to be put into the event's data field, if the event's data is NULL

	// Create the fighter record
	*fighter = create_record();
	fighter->identifier_type = IT_STRING;
	change_string_identifier(fighter, fighter_name);

	target_event_node = find_list(database, &event_code, IT_INT);

	if (target_event_node == NULL) {
		//ERR
	}
	else {
		if (target_event_node->data->data == NULL) {
			new_list = malloc(sizeof(List));
			*new_list = create_list();
			target_event_node->data->data = new_list;
		}
		insert_list(target_event_node->data->data, fighter);
	}
}

void add_fighter_avl(List* database, int event_code, String fighter_name) {
	ListNodePtr target_event_node;				// The event to insert the fighter into
	Record* fighter = malloc(sizeof(Record));   // The new fighter record to be placed in the list
	AVL* new_avl;								// Variable that holds the new avl to be put into the event's data field, if the event's data is NULL

	// Create the fighter record
	*fighter = create_record();
	fighter->identifier_type = IT_STRING;
	change_string_identifier(fighter, fighter_name);

	target_event_node = find_list(database, &event_code, IT_INT);

	if (target_event_node == NULL) {
		//ERR
	}
	else {
		if (target_event_node->data->data == NULL) {
			new_avl = malloc(sizeof(AVL));
			*new_avl = create_avl();
			target_event_node->data->data = new_avl;
		}
		insert_avl(target_event_node->data->data, fighter);
	}
}

// Adds a fighter to an event
void add_fighter_to_event(List* database, int event_code, String fighter_name, Prototype prototype) {
	if (prototype == PROTOTYPE_UNSET) {
		// ERR
	}
	else if (prototype == LINKED_LIST_LINKED_LIST) {
		add_fighter_list(database, event_code, fighter_name);
	}
	else {
		add_fighter_avl(database, event_code, fighter_name);
	}
}

void add_event(List* database, int event_code) {
	Record* new_event = malloc(sizeof(Record));
	*new_event = create_record();
	new_event->identifier_type = IT_INT;
	change_int_identifier(new_event, event_code);

	insert_list(database, new_event);
}

void print_events_of_fighter_list(List* database, String fighter_name) {
	ListNodePtr current = database->head;

	while (current != NULL) {
		if (current->data->data != NULL && find_list(current->data->data, fighter_name, IT_STRING) != NULL) {
			print_record(current->data);
		}
		current = current->next;
	}
}

void print_events_of_fighter_avl(List* database, String fighter_name) {
	ListNodePtr current = database->head;

	while (current != NULL) {
		if (current->data->data != NULL && find_avl(current->data->data, fighter_name) != NULL) {
			print_record(current->data);
		}
		current = current->next;
	}
}

void print_events_of_fighter(List* database, String fighter_name, Prototype prototype) {
	if (prototype == LINKED_LIST_LINKED_LIST) {
		print_events_of_fighter_list(database, fighter_name);
	}
	else {
		print_events_of_fighter_avl(database, fighter_name);
	}
}

void test_database() {
	List database;
	List avl_database;

	database = create_list();
	avl_database = create_list();

	add_event(&database, 1);
	add_event(&database, 2);
	add_event(&database, 3);
	add_event(&database, 4);

	print_events(&database);

	add_fighter_to_event(&database, 1, "Fighter1", LINKED_LIST_LINKED_LIST);
	add_fighter_to_event(&database, 1, "Fighter3", LINKED_LIST_LINKED_LIST);
	add_fighter_to_event(&database, 1, "Fighter2", LINKED_LIST_LINKED_LIST);

	add_fighter_to_event(&database, 2, "Fighter1", LINKED_LIST_LINKED_LIST);
	add_fighter_to_event(&database, 4, "Fighter1", LINKED_LIST_LINKED_LIST);
	print_fighters_in_event(&database, 1, LINKED_LIST_LINKED_LIST);
	print_events_of_fighter(&database, "Fighter1", LINKED_LIST_LINKED_LIST);

	add_event(&avl_database, 1);
	add_event(&avl_database, 2);
	add_event(&avl_database, 3);
	add_event(&avl_database, 4);

	print_events(&avl_database);

	add_fighter_to_event(&avl_database, 1, "Fighter1", LINKED_LIST_AVL_TREE);
	add_fighter_to_event(&avl_database, 1, "Fighter3", LINKED_LIST_AVL_TREE);
	add_fighter_to_event(&avl_database, 1, "Fighter2", LINKED_LIST_AVL_TREE);

	add_fighter_to_event(&avl_database, 2, "Fighter1", LINKED_LIST_AVL_TREE);
	add_fighter_to_event(&avl_database, 4, "Fighter1", LINKED_LIST_AVL_TREE);
	print_fighters_in_event(&avl_database, 1, LINKED_LIST_AVL_TREE);
	print_events_of_fighter(&avl_database, "Fighter1", LINKED_LIST_AVL_TREE);

}
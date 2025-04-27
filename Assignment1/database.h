#pragma once

typedef enum {
	LINKED_LIST_LINKED_LIST,
	LINKED_LIST_AVL_TREE,
	PROTOTYPE_UNSET
} Prototype;

void print_events(List* database);

void print_fighters_in_event(List* database, int event_code, Prototype prototype);

void add_fighter_to_event(List* database, int event_code, String fighter_name, Prototype prototype);

void add_event(List* database, int event_code);

void test_database();


#pragma once

typedef enum {
	LINKED_LIST_LINKED_LIST,	// Prototype 1
	LINKED_LIST_AVL_TREE,		// Prototype 2
	PROTOTYPE_UNSET			    // Unset prototype
} Prototype;

/**
 * @brief Prints all events within a given database
 *
 * @param database The database to print the events of
 */
void print_events(List* database);

/**
 * @brief Prints fighters in a given event
 *
 * @param database The database to retrieve fighters and events from
 * @param event_code The code of the event whose fighters will be printed
 * @param prototype The prototype of the database
 * 
 */
void print_fighters_in_event(List* database, int event_code, Prototype prototype);

/**
 * @brief Adds a fighter to a given event
 *
 * @param database The database to add the fighter to
 * @param event_code The event to add the fighter to
 * @param fighter_name The name of the fighter to add
 * @param prototype The prototype of the database
 *
 */
void add_fighter_to_event(List* database, int event_code, String fighter_name, Prototype prototype);

/**
 * @brief Adds an event to the database
 *
 * @param database The database to add the event to
 * @param event_code The event to add to the database
 *
 */
void add_event(List* database, int event_code);


/**
 * @brief Tests all functions within the file aside from evaluate_database
 *
 */
void test_database();

/**
 * @brief Evaluates the performance of the two prototypes and prints the time it takes for them to complete the most
 * time complex operation of the database
 *
 */
void evaluate_database();


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "record.h"
#include "linked_list.h"
#include "avl_tree.h"
#include "database.h"

// NOTE: as the two prototypes differ in just the internal datastructure, not the outer one, the different prototypes
// will be refered to by their internal type. So for prototype 1 that is a linked list of a linked list, that will
// just be referred to as a 'list'. For prototype 2, which is a linked list containing avl trees, this will be
// referred to as 'avl'

// Adds an event to the a database
void add_event(List* database, int event_code) {
	Record* new_event = malloc(sizeof(Record)); // The new event to add to the database

	// Create event
	*new_event = create_record();
	new_event->identifier_type = IT_INT;
	change_int_identifier(new_event, event_code);

	insert_list(database, new_event);
}

// Adds a fighter to an event in a prototype 1 database
void add_fighter_list(List* database, Record* event, Record* fighter) {
	List* new_list;	// Variable that holds the new list to be put into the event's data field, if the event's data is NULL

	// If there are no other fighters in the list
	if (event->data == NULL) {
		new_list = malloc(sizeof(List));
		*new_list = create_list();
		event->data = new_list;
	}

	insert_list(event->data, fighter);
}

// Adds a fighter to an event in a prototype 2 database
void add_fighter_avl(List* database, Record* event, Record* fighter) {
	AVL* new_avl; // Variable that holds the new avl to be put into the event's data field, if the event's data is NULL

	// If there are no other fighters in the tree
	if (event->data == NULL) {
		new_avl = malloc(sizeof * new_avl);
		*new_avl = create_avl();
		event->data = new_avl;
	}

	insert_avl(event->data, fighter);
}

// Adds a fighter to an event. Function assumes that all parameters are valid
void add_fighter_to_event(List* database, int event_code, String fighter_name, Prototype prototype) {
	ListNodePtr target_event_node;				// The event to insert the fighter into
	Record* fighter = malloc(sizeof(Record));   // The new fighter record to be placed in the list

	// Create the fighter record
	*fighter = create_record();
	fighter->identifier_type = IT_STRING;
	change_string_identifier(fighter, fighter_name);

	target_event_node = find_list(database, &event_code, IT_INT);

	if (target_event_node == NULL) {
		printf("ERR4: Event %d does not exist!", event_code);
	}
	else if (prototype == LINKED_LIST_LINKED_LIST) {
		add_fighter_list(database, target_event_node->data, fighter);
	}
	else {
		add_fighter_avl(database, target_event_node->data, fighter);
	}
}

// Prints all events in ascending order
void print_events(List* database) {
	if (database->head == NULL) {
		printf("ERR5: Database is empty!");
	}
	else {
		print_list(database);
	}
	
}

// Prints all fighters that competed in a given event
void print_fighters_in_event(List* database, int event_code, Prototype prototype) {
	ListNodePtr target_event_node;	// The event which should have all fighters printed

	target_event_node = find_list(database, &event_code, IT_INT);

	if (target_event_node->data->data == NULL) {
		printf("ERR6: This event has no fighters listed yet!");
	}
	else if (prototype == LINKED_LIST_LINKED_LIST) {
		print_list(target_event_node->data->data);
	}
	else {
		print_avl_without_tree(target_event_node->data->data);
	}
}

// Prints all of the events that a fighter has fought in, when the fighters are stored in an linked list (prototype 1)
// This function assumes that the database has at least 1 event
void print_events_of_fighter_list(List* database, String fighter_name) {
	ListNodePtr current = database->head; // The node currently being iterated over

	while (current != NULL) {
		if (current->data->data != NULL && find_list(current->data->data, fighter_name, IT_STRING) != NULL) {
			print_record(current->data);
			printf("\n");
		}
		current = current->next;
	}
}

// Prints all of the events that a fighter has fought in, when the fighters are stored in an AVL (prototype 2)
// This function assumes that the database has at least 1 event
void print_events_of_fighter_avl(List* database, String fighter_name) {
	ListNodePtr current = database->head; // The node currently being iterated over
	 
	while (current != NULL) {
		if (current->data->data != NULL && find_avl(current->data->data, fighter_name) != NULL) {
			print_record(current->data);
			printf("\n");
		}
		current = current->next;
	}
}

// Prints all events that a given fighter has fought in
void print_events_of_fighter(List* database, String fighter_name, Prototype prototype) {
	if (database->head == NULL) {
		printf("ERR5: Database is empty!");
	}
	else if (prototype == LINKED_LIST_LINKED_LIST) {
		print_events_of_fighter_list(database, fighter_name);
	}
	else {
		print_events_of_fighter_avl(database, fighter_name);
	}
}

// Reads a file and places data of fighters and events into a given database
// File reading code is derivative of week 9 tutorial
// This funciton assumes that the file lines being read have not been added to the database yet
void read_data(List* database, int start, int end, Prototype prototype) {
	FILE* file = fopen("ufc_dataset_10k.csv", "r"); // The file containing all records
	int event;				   // The event currently being iterated over in the file
	int most_recent_event = 0; // The event of the fighter that was added most recently. Defaults to 0 to enable comparisons before needing to assign
	char line[15];		       // Temporarily stores line of text, maximum length is 13 characters + terminator and \n
	char fighter[11];	       // Temporarily stores name of fighter, maximum length is 9 characters + terminator and \n

	if (file == NULL) {
		return;
	}

	// Skip lines up to 'start'
	// This for loop was made by ChatGPT
	for (int i = 0; i < start; i++) {
		if (!fgets(line, sizeof(line), file)) {
			return;  // EOF or error early
		}
	}

	if (database->head != NULL) {
		most_recent_event = *(int*)database->head->data->identifier;
	}

	for (int i = start; i < end; i++)
	{
		fgets(line, sizeof(line), file);
		// Next five lines were made by ChatGPT
		char* comma = strchr(line, ',');
		*comma = '\0';
		event = atoi(line);
		strncpy_s(fighter, sizeof(fighter), comma + 1, _TRUNCATE);
		fighter[strcspn(fighter, "\n")] = 0; // Remove newline

		if (event != most_recent_event || database->head == NULL) {
			add_event(database, event);
		}
		most_recent_event = event;
		add_fighter_to_event(database, event, fighter, prototype);

	}
}

// Times the many to many function for a given database and returns the time in milliseconds. Function is derived
// from week 4 tutorial
long time_many_to_many(List* database, Prototype prototype) {
	clock_t start;		// The starting time
	clock_t difference; // The time between the start and the end
	long milliseconds;  // How long in milliseconds the difference is

	start = clock();
	print_events_of_fighter(database, "AA AA", prototype);
	difference = clock() - start;
	milliseconds = difference * 1000 / CLOCKS_PER_SEC;

	return milliseconds;
}


// Tests this files functions
void test_database() {

	// Note about testing add_fighter_to_event and private helper functions: The helper functions (add_fighter_avl & 
	// add_fighter_list) will be tested by calling their public parent function add_fighter_to_event. This is because
	// if the helper functions were to be tested on their own without the parent, then the utility of the public function
	// would need to be replicated prior to every test anyway. This makes testing far more succinct while still being able
	// to actually test the functions and all of their execution paths. Considering this, add_fighter_to_event will be tested
	// directly first.

	// Note about testing read_data: The first if statement cannot be tested well as it will never be triggered in this 
	// environment. It can be triggered if the file is not present in the directory, but this will do exactly what is 
	// expected and just exit the function. This is why further testing of this execution path is not necessary

	List database = create_list();
	List empty_database = create_list();
	List avl_database = create_list();
	ListNodePtr node_to_delete;

	printf("\n------------------------------------------------------\n                  *database.c tests*\n");

	// ----------------------------------------------------------------------------------------------------------------
	// 1 - Test 'add_event()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n1. add_event() test\n----------------\n");

	// 1.1 - Tests if an event can be successfully created and placed into an empty database
	add_event(&database, 1);
	
	printf("1.1 - Expected value: 1\n1.1 - Actual value: ");
	print_list(&database);

	// 1.2 - Tests if an event can be successfully created and placed into an database with items in it
	add_event(&database, 2);

	printf("1.2 - Expected value: \n1\n2\n1.2 - Actual value: \n");
	print_list(&database);

	// ----------------------------------------------------------------------------------------------------------------
	// 2 - Test 'add_fighter_to_event()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n2. add_fighter_to_event() test\n----------------");

	// 2.1 - Tests error detection when attempting to add a fighter to an event that does not exist. Triggers the first
	//		 if statement
	printf("\n2.1 - Expected value: ERR4\n2.1 - Actual value: ");
	add_fighter_to_event(&database, 5, "John Fighter", LINKED_LIST_LINKED_LIST);

	// 2.2 - Tests insertion when the prototype is LINKED_LIST_LINKED_LIST (prototype 1). Tests the first if else statement
	add_fighter_to_event(&database, 1, "John Fighter", LINKED_LIST_LINKED_LIST);

	printf("\n2.2 - Expected value: John Fighter\n2.2 - Actual value: ");
	print_list(database.head->data->data);

	// 2.3 - Tests insertion when the prototype is LINKED_LIST_AVL_TREE (prototype 2). Tests the else statement
	add_event(&avl_database, 1);
	add_fighter_to_event(&avl_database, 1, "John Fighter", LINKED_LIST_AVL_TREE);

	printf("2.3 - Expected value: John Fighter\n2.3 - Actual value: ");
	print_avl_without_tree(avl_database.head->data->data);

	// ----------------------------------------------------------------------------------------------------------------
	// 3 - Test 'add_fighter_list()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n3. add_fighter_list() test\n----------------\n");

	// 3.1 - Test insertion when there are no fighters in the list. This tests the if statement
	add_fighter_to_event(&database, 2, "Jane Fighter", LINKED_LIST_LINKED_LIST);

	printf("3.1 - Expected value: Jane Fighter\n3.1 - Actual value: ");
	// This accesses record 2
	print_list(database.head->next->data->data);

	// 3.2 - Test insertion when there are fighters in the list. This bypasses the if statement
	add_fighter_to_event(&database, 2, "John Fighter", LINKED_LIST_LINKED_LIST);

	printf("3.2 - Expected value:\nJane Fighter\nJohn Fighter\n3.2 - Actual value:\n");
	// This accesses record 2
	print_list(database.head->next->data->data);

	// ----------------------------------------------------------------------------------------------------------------
	// 4 - Test 'add_fighter_avl()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n4. add_fighter_avl() test\n----------------\n");

	// 4.1 - Test insertion when there are no fighters in the tree. This tests the if statement
	add_event(&avl_database, 2);
	add_fighter_to_event(&avl_database, 2, "Jane Fighter", LINKED_LIST_AVL_TREE);

	printf("4.1 - Expected value: Jane Fighter\n3.1 - Actual value: ");
	// This accesses record 2
	print_avl_without_tree(avl_database.head->next->data->data);

	// 4.2 - Test insertion when there are fighters in the tree. This bypasses the if statement
	add_fighter_to_event(&avl_database, 2, "John Fighter", LINKED_LIST_AVL_TREE);

	printf("4.2 - Expected value:\nJane Fighter\nJohn Fighter\n3.2 - Actual value:\n");
	// This accesses record 2
	print_avl_without_tree(avl_database.head->next->data->data);

	// ----------------------------------------------------------------------------------------------------------------
	// 5 - Test 'print_events()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n5. print_events() test\n----------------\n");

	// 5.1 - Test error detection when database has no elements
	
	printf("5.1 - Expected value: ERR5\n5.1 - Actual value: ");
	print_events(&empty_database);

	// 5.2 - Test if function can successfully print all events in a database when the database has elements in it

	printf("\n5.2 - Expected value:\n1\n2\n5.2 - Actual value:\n");
	print_events(&database);

	// ----------------------------------------------------------------------------------------------------------------
	// 6 - Test 'print_fighters_in_event()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n6. print_fighters_in_event() test\n----------------\n");

	// 6.1 - Test when the event has no fighters. This triggers the first if statement
	add_event(&database, 3);

	printf("6.1 - Expected value: ERR6\n6.1 - Actual value: ");
	print_fighters_in_event(&database, 3, LINKED_LIST_LINKED_LIST);

	// 6.2 - Test when the database is of prototype 1 (LINKED_LIST_LINKED_LIST). This triggers the if else statement
	printf("\n6.2 - Expected value: John Fighter\n6.2 - Actual value: ");
	print_fighters_in_event(&database, 1, LINKED_LIST_LINKED_LIST);

	// 6.3 - Test when the database is of prototype 2 (LINKED_LIST_AVL_TREE). This triggers the else statement
	printf("6.3 - Expected value: John Fighter\n6.3 - Actual value: ");
	print_fighters_in_event(&avl_database, 1, LINKED_LIST_AVL_TREE);

	// ----------------------------------------------------------------------------------------------------------------
	// 7 - Test 'print_events_of_fighter_list()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n7. print_events_of_fighter_list() test\n----------------\n");

	// 7.1 - Test when the fighter is in no events
	printf("7.1 - Expected value: \n7.1 - Actual value: ");
	print_events_of_fighter_list(&database, "Jimmy Fighter");

	// 7.2 - Test when the fighter has fought in events
	//add_fighter_to_event(&database, 2, "John Fighter", LINKED_LIST_LINKED_LIST);

	printf("\n7.2 - Expected value:\n1\n2\n7.2 - Actual value:\n");
	print_events_of_fighter_list(&database, "John Fighter");

	// ----------------------------------------------------------------------------------------------------------------
	// 8 - Test 'print_events_of_fighter_avl()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n8. print_events_of_fighter_avl() test\n----------------\n");

	// 8.1 - Test when the fighter is in no events
	printf("8.1 - Expected value: \n8.1 - Actual value: ");
	print_events_of_fighter_avl(&avl_database, "Jimmy Fighter");

	// 8.2 - Test when the fighter has fought in events

	printf("\n8.2 - Expected value:\n1\n2\n8.2 - Actual value:\n");
	print_events_of_fighter_avl(&avl_database, "John Fighter");

	// ----------------------------------------------------------------------------------------------------------------
	// 9 - Test 'print_events_of_fighter()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n9. print_events_of_fighter() test\n----------------\n");

	// 9.1 - Test when the database is empty. Tests if statement
	printf("9.1 - Expected value: ERR5\n9.1 - Actual value: ");
	print_events_of_fighter(&empty_database, "John Fighter", LINKED_LIST_LINKED_LIST);

	// 9.2 - Test when the database is of prototype 1 (LINKED_LIST_LINKED_LIST). Tests if else statement
	printf("\n9.2 - Expected value: \n1\n2\n9.2 - Actual value:\n");
	print_events_of_fighter(&database, "John Fighter", LINKED_LIST_LINKED_LIST);

	// 9.3 - Test when the database is of prototype 2 (LINKED_LIST_LINKED_LIST). Tests else statement
	printf("9.3 - Expected value:\n1\n2\n9.3 - Actual value:\n");
	print_events_of_fighter(&avl_database, "John Fighter", LINKED_LIST_AVL_TREE);

	// ----------------------------------------------------------------------------------------------------------------
	// 10 - Test 'read_data()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n10. read_data() test\n----------------\n");

	// 10.1 - Test adding data to an empty database. This will trigger the 'if' statement within the final for loop of the 
	//	      function add an event that currently does not exist in the database. Test is successful if the data is successfully
	//		  added
	read_data(&empty_database, 0, 1, LINKED_LIST_LINKED_LIST);

	printf("10.1 - Expected value: 250\n10.1 - Actual value: ");
	print_events(&empty_database);
	printf("10.1 - Expected value: A B\n10.1 - Actual value: ");
	print_fighters_in_event(&empty_database, 250, LINKED_LIST_LINKED_LIST);

	// 10.2 - Test adding data to a database with the same event and with a start value > 0. This tests the for loop that
	//		  skips fgets lines and the second non nested if statement.
	read_data(&empty_database, 1, 2, LINKED_LIST_LINKED_LIST);

	printf("10.2 - Expected value: 250\n10.2 - Actual value: ");
	print_events(&empty_database);
	printf("10.2 - Expected value: \nA B\nC D\n10.2 - Actual value:\n");
	print_fighters_in_event(&empty_database, 250, LINKED_LIST_LINKED_LIST);

	// 10.3 - Test adding data to a database where the event number changes during the function's execution. This further 
	//		  tests the if statement within the final for loop
	read_data(&empty_database, 9999, 10002, LINKED_LIST_LINKED_LIST);

	printf("10.3 - Expected value:\n249\n250\n10.3 - Actual value:\n");
	print_events(&empty_database);
	printf("10.3 - Expected value: \nA B\nC D\n10.3 - Actual value:\n");
	print_fighters_in_event(&empty_database, 249, LINKED_LIST_LINKED_LIST);

	// ----------------------------------------------------------------------------------------------------------------
	// 11 - Test 'time_many_to_many()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n11. time_many_to_many() test\n----------------\n");

	// 11.1 - Tests the function and whether it can time the execution of print_events_of_fighter(). This function has
	//		  a single execution path so no other tests are needed
	printf("11.1 - Expected value: [time]\n11.1 - Actual value: ");
	printf("%d", time_many_to_many(&database, LINKED_LIST_LINKED_LIST));

}

// Evaluates the most time sensitive function of this database file (print_events_of_fighter) for both prototypes using
// testing data.
void evaluate_database() {
	List database = create_list();
	List avl_database = create_list();
	long result;

	// The two values of note are m (the number of events) and n (the number of fighters within an event) each test will 
	// deal with increasing amounts of both

	printf("\n------------------------------------------------------\n             *Database evaluation tests*");
	printf("\n------------------------------------------------------\n");

	// 1. Test time to print all events of a fighter when 10 records have been added (when m=1 and n1 = 10)
	printf("--------------------10 records--------------------\n\n");

	read_data(&database, 0, 10, LINKED_LIST_LINKED_LIST);
	read_data(&avl_database, 0, 10, LINKED_LIST_AVL_TREE);

	result = time_many_to_many(&database, LINKED_LIST_LINKED_LIST);
	printf("PROTOTYPE 1 - Time when m=1 and n1=10 = %d milliseconds\n\n", result);

	result = time_many_to_many(&avl_database, LINKED_LIST_AVL_TREE);
	printf("PROTOTYPE 2 - Time when m=1 and n1=10 = %d milliseconds\n\n", result);

	// 2. Test time to print all events of a fighter when 50 records have been added (when m=1 and n1 = 50)
	printf("--------------------50 records--------------------\n\n");

	read_data(&database, 10, 50, LINKED_LIST_LINKED_LIST);
	read_data(&avl_database, 10, 50, LINKED_LIST_AVL_TREE);

	result = time_many_to_many(&database, LINKED_LIST_LINKED_LIST);
	printf("PROTOTYPE 1 - Time when m=1 and n1=50 = %d milliseconds\n\n", result);

	result = time_many_to_many(&avl_database, LINKED_LIST_AVL_TREE);
	printf("PROTOTYPE 2 - Time when m=1 and n1=50 = %d milliseconds\n\n", result);

	// 3. Test time to print all events of a fighter when 100 records have been added (when m=1 and n1 = 100)
	printf("--------------------100 records--------------------\n\n");

	read_data(&database, 50, 100, LINKED_LIST_LINKED_LIST);
	read_data(&avl_database, 50, 100, LINKED_LIST_AVL_TREE);

	result = time_many_to_many(&database, LINKED_LIST_LINKED_LIST);
	printf("PROTOTYPE 1 - Time when m=1 and n1=100 = %d milliseconds\n\n", result);

	result = time_many_to_many(&avl_database, LINKED_LIST_AVL_TREE);
	printf("PROTOTYPE 2 - Time when m=1 and n1=100 = %d milliseconds\n\n", result);

	// 4. Test time to print all events of a fighter when 250 records have been added (when m=1 and n1 = 250)
	printf("--------------------250 records--------------------\n\n");

	read_data(&database, 100, 250, LINKED_LIST_LINKED_LIST);
	read_data(&avl_database, 100, 250, LINKED_LIST_AVL_TREE);

	result = time_many_to_many(&database, LINKED_LIST_LINKED_LIST);
	printf("PROTOTYPE 1 - Time when m=1 and n1=250 = %d milliseconds\n\n", result);

	result = time_many_to_many(&avl_database, LINKED_LIST_AVL_TREE);
	printf("PROTOTYPE 2 - Time when m=1 and n1=250 = %d milliseconds\n\n", result);

	// 5. Test time to print all events of a fighter when 500 records have been added (when m=1 and n1 = 500)
	printf("--------------------500 records--------------------\n\n");

	read_data(&database, 250, 500, LINKED_LIST_LINKED_LIST);
	read_data(&avl_database, 250, 500, LINKED_LIST_AVL_TREE);

	result = time_many_to_many(&database, LINKED_LIST_LINKED_LIST);
	printf("PROTOTYPE 1 - Time when m=1 and n1=500 = %d milliseconds\n\n", result);

	result = time_many_to_many(&avl_database, LINKED_LIST_AVL_TREE);
	printf("PROTOTYPE 2 - Time when m=1 and n1=500 = %d milliseconds\n\n", result);

	// 6. Test time to print all events of a fighter when 5000 records have been added (when m=1 and n1= 5,000)
	printf("--------------------5,000 records--------------------\n\n");

	read_data(&database, 500, 5000, LINKED_LIST_LINKED_LIST);
	read_data(&avl_database, 500, 5000, LINKED_LIST_AVL_TREE);

	result = time_many_to_many(&database, LINKED_LIST_LINKED_LIST);
	printf("PROTOTYPE 1 - Time when m=1 and n1=5,000 = %d milliseconds\n\n", result);

	result = time_many_to_many(&avl_database, LINKED_LIST_AVL_TREE);
	printf("PROTOTYPE 2 - Time when m=1 and n1=5,000 = %d milliseconds\n\n", result);

	// 7. Test time to print all events of a fighter when 10,000 records have been added (when m=1 and n1= 10,000)
	printf("--------------------10,000 records--------------------\n\n");

	read_data(&database, 5000, 10000, LINKED_LIST_LINKED_LIST);
	read_data(&avl_database, 5000, 10000, LINKED_LIST_AVL_TREE);

	result = time_many_to_many(&database, LINKED_LIST_LINKED_LIST);
	printf("PROTOTYPE 1 - Time when m=1 and n1=10,000 = %d milliseconds\n\n", result);

	result = time_many_to_many(&avl_database, LINKED_LIST_AVL_TREE);
	printf("PROTOTYPE 2 - Time when m=1 and n1=10,000 = %d milliseconds\n\n", result);

	// 8. Test time to print all events of a fighter when 20,000 records have been added (when m=2 and n1+n2= 20,000)
	printf("--------------------20,000 records--------------------\n\n");

	read_data(&database, 10000, 20000, LINKED_LIST_LINKED_LIST);
	read_data(&avl_database, 10000, 20000, LINKED_LIST_AVL_TREE);

	result = time_many_to_many(&database, LINKED_LIST_LINKED_LIST);
	printf("PROTOTYPE 1 - Time when m=2 and n1+n2=20,000 = %d milliseconds\n\n", result);

	result = time_many_to_many(&avl_database, LINKED_LIST_AVL_TREE);
	printf("PROTOTYPE 2 - Time when m=2 and n1+n2=20,000 = %d milliseconds\n\n", result);

	// 8. Test time to print all events of a fighter when 50,000 records have been added (when m=5 and n1+n2+n...= 50,000)
	printf("--------------------50,000 records--------------------\n\n");

	read_data(&database, 20000, 50000, LINKED_LIST_LINKED_LIST);
	read_data(&avl_database, 20000, 50000, LINKED_LIST_AVL_TREE);

	result = time_many_to_many(&database, LINKED_LIST_LINKED_LIST);
	printf("PROTOTYPE 1 - Time when m=5 and n1+n2+n...=50,000 = %d milliseconds\n\n", result);

	result = time_many_to_many(&avl_database, LINKED_LIST_AVL_TREE);
	printf("PROTOTYPE 2 - Time when m=5 and n1+n2+n...=50,000 = %d milliseconds\n\n", result);	

	// 7. Test time to print all events of a fighter when 125,000 records have been added (when m=125 and n1+n2+n...= 125,000)
	printf("--------------------125,000 records--------------------\n\n");

	read_data(&database, 50000, 125000, LINKED_LIST_LINKED_LIST);
	read_data(&avl_database, 50000, 125000, LINKED_LIST_AVL_TREE);

	result = time_many_to_many(&database, LINKED_LIST_LINKED_LIST);
	printf("PROTOTYPE 1 - Time when m=12 and n1+n2+n...=125,000 = %d milliseconds\n\n", result);

	result = time_many_to_many(&avl_database, LINKED_LIST_AVL_TREE);
	printf("PROTOTYPE 2 - Time when m=12 and n1+n2+n...=125,000 = %d milliseconds\n\n", result);

	// 8. Test time to print all events of a fighter when 250,000 records have been added (when m=25 and n1+n2+n...= 250,000)
	printf("--------------------250,000 records--------------------\n\n");

	read_data(&database, 125000, 250000, LINKED_LIST_LINKED_LIST);
	read_data(&avl_database, 125000, 250000, LINKED_LIST_AVL_TREE);

	result = time_many_to_many(&database, LINKED_LIST_LINKED_LIST);
	printf("PROTOTYPE 1 - Time when m=25 and n1+n2+n...=250,000 = %d milliseconds\n\n", result);

	result = time_many_to_many(&avl_database, LINKED_LIST_AVL_TREE);
	printf("PROTOTYPE 2 - Time when m=25 and n1+n2+n...=250,000 = %d milliseconds\n\n", result);


	// 9. Test time to print all events of a fighter when 500,000 records have been added (when m=50 and n1+n2+n...= 500,000)
	printf("--------------------500,000 records--------------------\n\n");

	read_data(&database, 250000, 500000, LINKED_LIST_LINKED_LIST);
	read_data(&avl_database, 250000, 500000, LINKED_LIST_AVL_TREE);

	result = time_many_to_many(&database, LINKED_LIST_LINKED_LIST);
	printf("PROTOTYPE 1 - Time when m=50 and n1+n2+n...=500,000 = %d milliseconds\n\n", result);

	result = time_many_to_many(&avl_database, LINKED_LIST_AVL_TREE);
	printf("PROTOTYPE 2 - Time when m=50 and n1+n2+n...=500,000 = %d milliseconds\n\n", result);

	// 10. Test time to print all events of a fighter when 1,000,000 records have been added (when m=100 and n1+n2+n...= 1,000,000)
	printf("--------------------1,000,000 records--------------------\n\n");

	read_data(&database, 500000, 1000000, LINKED_LIST_LINKED_LIST);
	read_data(&avl_database, 500000, 1000000, LINKED_LIST_AVL_TREE);

	result = time_many_to_many(&database, LINKED_LIST_LINKED_LIST);
	printf("PROTOTYPE 1 - Time when m=100 and n1+n2+n...=1,000,000 = %d milliseconds\n\n", result);

	result = time_many_to_many(&avl_database, LINKED_LIST_AVL_TREE);
	printf("PROTOTYPE 2 - Time when m=100 and n1+n2+n...=1,000,000 = %d milliseconds\n\n", result);

	// 11. Test time to print all events of a fighter when 2,500,000 records have been added (when m=250 and n1+n2+n...= 2,500,000)
	printf("--------------------2,500,000 records--------------------\n\n");

	read_data(&database, 1000000, 2500000, LINKED_LIST_LINKED_LIST);
	read_data(&avl_database, 1000000, 2500000, LINKED_LIST_AVL_TREE);

	result = time_many_to_many(&database, LINKED_LIST_LINKED_LIST);
	printf("PROTOTYPE 1 - Time when m=250 and n1+n2+n...=2,500,000 = %d milliseconds\n\n", result);

	result = time_many_to_many(&avl_database, LINKED_LIST_AVL_TREE);
	printf("PROTOTYPE 2 - Time when m=250 and n1+n2+n...=2,500,000 = %d milliseconds\n\n", result);

}
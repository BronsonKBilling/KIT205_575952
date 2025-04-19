// record.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "record.h"
#include "linked_list.h"

// Creates and returns a new record
Record create_record() {
	Record new_record; // The new record to return

	new_record.identifier = NULL;
	new_record.identifier_type = IT_UNSET;
	new_record.data = NULL;
	new_record.data_type = DT_UNSET;

	return new_record;
}

// Compares two record identifiers. Returns 1 if the first comes after the second, 0 if they are the same, and -1 if the
// first comes before the second 
// This function assumes that the identifier types match the identifiers.
int compare_records(Record* self, Record* other) {
	int comparison_result;

	if (self->identifier_type != other->identifier_type)
	{
		printf("ER1: ERROR! You are comparing two identifiers with different data types.");
		return;	// This is temporary and is only here to simulate exiting the program during testing
		// exit(1);
	}  
	// Only checks one identifier type as both identifiers must be the same, or else the program would have already exited
	else if (self->identifier_type == IT_UNSET) {
		printf("ER2: ERROR! The identifier type is unset.");
		return;	// This is temporary and is only here to simulate exiting the program during testing
		// exit(2);
	}	

	if (self->identifier == NULL || other->identifier == NULL) {
		printf("ER3: ERROR! One or both identifiers is unset.");
		return;	// This is temporary and is only here to simulate exiting the program during testing
	}

	if (self->identifier_type == IT_STRING) {
		comparison_result = strcmp(self->identifier, other->identifier);
		// Normalise the result to match the format specified by this function
		if (comparison_result > 0) {
			comparison_result = 1;
		} 
		else if (comparison_result < 0) {
			comparison_result = -1;
		}
	}
	// Only other identifier type is IT_INT as previous checks make progressing with IT_UNSET impossible
	else {
		if (self->identifier > other->identifier) {
			comparison_result = 1;
		}
		else if (self->identifier < other->identifier) {
			comparison_result = -1;
		}
		else {
			comparison_result = 0;
		}
	}

	return comparison_result;
}

// Prints the identifier of a record
// This function assumes that the identifier types match the identifiers.
void print_record(Record* self) {
	String format; // The format of the printf function

	if (self->identifier_type == IT_UNSET)
	{
		printf("ER2: ERROR! The identifier type is unset.");
		return;	// This is temporary and is only here to simulate exiting the program during testing
		// exit(2);
	} 

	if (self->identifier == NULL) {
		printf("ER3: ERROR! The identifier is unset.");
		return;	// This is temporary and is only here to simulate exiting the program during testing
	}

	if (self->identifier_type == IT_STRING) {
		format = "%s";
	} 
	else {
		format = "%d";
	}

	printf(format, self->identifier);
}

// Tests all of the above functions
void test_record() {
	Record testing_record;
	Record other_record;
	int result;

	// 1 - Test 'create_record()'
	printf("----------------\n1. create_record() test\n----------------\n");

	// 1.1 - Tests if the function can create and return a record without throwing any errors
	testing_record = create_record();
	printf("1.1 - Test passed! Record successfully created!\n");

	// 2 - Test 'compare_records()'
	printf("----------------\n2. compare_records() test\n----------------\n");

	// 2.1 - Test behaviour when the identifier types do not match. This should exit the program with an error message
	// The exit functionality is currently commented in all instances to ensure that all tests can run. Instead, only 
	// the error message is printed.
	other_record = create_record();
	other_record.identifier_type = IT_STRING;

	printf("2.1 - Expected Result: [ER1]\n2.1 - Actual Result: ");
	compare_records(&testing_record, &other_record);

	// 2.2 - Test when both identifier types are set to 'IT_UNSET'	
	other_record.identifier_type = IT_UNSET;
	printf("\n2.2 - Expected Result: [ER2]\n2.2 - Actual Result: ");
	compare_records(&testing_record, &other_record);

	// 2.3 - Test when just one identifier type is set to 'IT_UNSET'		
	other_record.identifier_type = IT_STRING;
	printf("\n2.3 - Expected Result: [ER1]\n2.3 - Actual Result: ");
	compare_records(&testing_record, &other_record);

	// 2.4 - Tests when an identifier is NULL
	testing_record.identifier_type = IT_STRING;
	printf("\n2.4 - Expected Result: [ER3]\n2.4 - Actual Result: ");
	compare_records(&testing_record, &other_record);

	// 2.5 - Test when the identifier types are both Strings, and when the first record comes after the second
	testing_record.identifier = "AAAAB";
	other_record.identifier = "AAAAA";

	result = compare_records(&testing_record, &other_record);
	printf("\n2.5 - Expected Result: 1\n2.5 - Actual Result: ");
	printf("%d", result);
	
	// 2.6 - Tests when the identifiers are both strings, and when the first record comes before the second
	other_record.identifier = "AAAAC";

	result = compare_records(&testing_record, &other_record);
	printf("\n2.6 - Expected Result: -1\n2.6 - Actual Result: ");
	printf("%d", result);

	// 2.7 - Tests when the identifiers are both strings, and when the records are the same
	other_record.identifier = "AAAAB";

	result = compare_records(&testing_record, &other_record);
	printf("\n2.7 - Expected Result: 0\n2.7 - Actual Result: ");
	printf("%d", result);

	// 2.8 - Tests when the identifiers are ints, and the first is larger than the second
	testing_record.identifier_type = IT_INT;
	other_record.identifier_type = IT_INT;
	testing_record.identifier = 313;
	other_record.identifier = 312;

	result = compare_records(&testing_record, &other_record);
	printf("\n2.8 - Expected Result: 1\n2.8 - Actual Result: ");
	printf("%d", result);

	// 2.9 - Tests when the identifiers are ints, and the first is smaller than the second
	other_record.identifier = 314;

	result = compare_records(&testing_record, &other_record);
	printf("\n2.9 - Expected Result: -1\n2.9 - Actual Result: ");
	printf("%d", result);

	// 2.10 - Tests when the identifiers are ints, and the first is the same as the second
	testing_record.identifier = 314;

	result = compare_records(&testing_record, &other_record);
	printf("\n2.10 - Expected Result: 0\n2.10 - Actual Result: ");
	printf("%d", result);

	// 3 - Test print_record()
	printf("\n----------------\n3. print_record() test\n----------------\n");

	// 3.1 - Tests when the identifier type is unset. Should print error message and exit program. It does not exit the
	//		 program currently for testing reasons
	testing_record.identifier_type = IT_UNSET;
	printf("3.1 - Expected Result: [ER2]\n3.1 - Actual Result: ");
	print_record(&testing_record);

	// 3.2 - Tests when the identifier is NULL
	testing_record.identifier_type = IT_STRING;
	testing_record.identifier = NULL;
	printf("\n3.2 - Expected Result: [ER3]\n3.2 - Actual Result: ");
	print_record(&testing_record);

	// 3.3 - Tests when the identifier type is a string and has a value
	testing_record.identifier = "John";
	printf("\n3.3 - Expected Result: John\n3.3 - Actual Result: ");
	print_record(&testing_record);

	// 3.4 - Tests when the identifier is an int and has a value
	testing_record.identifier_type = IT_INT;
	testing_record.identifier = 40;
	printf("\n3.4 - Expected Result: 40\n3.4 - Actual Result: ");
	print_record(&testing_record);

}
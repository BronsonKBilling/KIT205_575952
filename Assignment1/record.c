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

	return new_record;
}

// Compares two record identifiers. Returns 1 if the first comes after the second, 0 if they are the same, and -1 if the
// first comes before the second. This function assumes that the identifier types match the identifiers.
int compare_records(Record* self, Record* other) {
	int comparison_result; // The result of the comparison. Conforms to values above

	// Do error checks
	if (self->identifier_type != other->identifier_type)
	{
		printf("ER1: ERROR! You are comparing two identifiers with different data types.");
		return 1;	// This is temporary and is only here to simulate exiting the program during testing
		// exit(1);
	}  
	// Only checks one identifier type as both identifiers must be the same, or else the program would have already exited
	else if (self->identifier_type == IT_UNSET) {
		printf("ER2: ERROR! The identifier type is unset.");
		return 2;	// This is temporary and is only here to simulate exiting the program during testing
		// exit(2);
	}	
	if (self->identifier == NULL || other->identifier == NULL) {
		printf("ER3: ERROR! One or both identifiers is unset.");
		return 3;	// This is temporary and is only here to simulate exiting the program during testing
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
		if (*(int*)self->identifier > *(int*)other->identifier) {
			comparison_result = 1;
		}
		else if (*(int*)self->identifier < *(int*)other->identifier) {
			comparison_result = -1;
		}
		else {
			comparison_result = 0;
		}
	}

	return comparison_result;
}

// A wrapper function to compare records by inputting a identifier type and identifier, rather than a record
int compare_record_by_identifier(Record* self, void* identifier, IdentifierType type) {
	Record dummy_record; // A record used to search for the given identifier

	dummy_record.identifier_type = type;
	dummy_record.identifier = identifier;

	return compare_records(self, &dummy_record);
}

// Prints the identifier of a record. This function assumes that the identifier types match the identifiers.
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
		printf("%s", self->identifier);
	} 
	else {
		format = "%d";
		printf("%d", *(int*)self->identifier);
	}
}

// Clones a record and returns a pointer to the clone. This function assumes that records with a set identifier type also
// have a valid identifier of the same type. Cloning the data of a record is not necessary for the database so is not implemented
// in this function
Record* clone_record(Record* self) {
	Record* clone = malloc(sizeof(Record));  // The new record that is a clone of self

	*clone = create_record();

	// Directly assigning is fine here as this field is not a pointer, so the compiler will make a copy
	clone->identifier_type = self->identifier_type;

	if (self->identifier != NULL) {
		if (self->identifier_type == IT_INT) {
			clone->identifier = malloc(sizeof(int));
			*(int*)clone->identifier = *(int*)self->identifier;
		} 
		else { // IT_STRING is the only other option as IT_UNSET cannot be the type if the identifier is NULL
			// malloc is necessary to avoid making a shallow copy
			clone->identifier = malloc(strlen(self->identifier) + 1);
			strcpy_s(clone->identifier, strlen(self->identifier) + 1, self->identifier);
		} 
	} 

	return clone;
}

// Destroys a record and frees memory. Only used for testing purposes to free up variables and not in the database implementation
void destroy_record(Record* self) {
	if (self->identifier != NULL && self->identifier_type != IT_STRING) {
		free(self->identifier);
	}
	
	free(self);
}

// Changes the identifier of a record that has an identifier type of IT_INT. This is necessary as 'identifier' is of
// type 'void*', meaning that directly assigning an integer value to it will make 'identifier' equal the memory address that
// correlates to the decimal value of the assigned int. This function assumes that the identifier is either NULL or an int.
// String literals cannot be freed.
void change_int_identifier(Record* self, int num) {
	if (self->identifier != NULL ) {
		free(self->identifier);
	}

	int* new_int = malloc(sizeof(int));
	*new_int = num;
	self->identifier = new_int;
}

// Changes the identifier of a record that has an identifier type of IT_STRING. This is necessary as 'identifier' is of
// type 'void*'. While the string will not equal the memory address as in the case of an int, the clone_record function 
// requires strings to be malloc'd when copied, to avoid them from becoming shallow copies. This funciton is necessary
// as if some records used direct assignment of string literals, and others used pointers to strings, the database would
// be wildly inconsistent.
void change_string_identifier(Record* self, String str) {
	if (self->identifier != NULL) {
		free(self->identifier);
	}

	String new_string = malloc(strlen(str) + 1);
	strcpy_s(new_string, strlen(str) + 1, str);
	self->identifier = new_string;
}

// Tests all of the above functions
void test_record() {
	Record testing_record;
	Record other_record;
	Record* cloning_test_record;
	int result;

	// Note: The Record variables will go from storing Strings to storing Ints, this is not behaviour that will be seen 
	// in the actual database program, as records will maintian only a single IT_TYPE. IT_TYPEs change in this function
	// for efficiency, simplicity, and to limit the number of variables being used.

	// Note about testing destroy_record(): This function is diffucult to test as testing it would involve accessing 
	// already freed memory. It would also be impossible to actually see if the identifier within a record is being
	// freed, as that would of course require accessing the already freed identifier within an already freed record.
	// Considering this, and considering the fact that destroy_record is called multiple times during testing successfully,
	// this function does not require further testing. This function is also only used for testing and is not used for the 
	// database implementation, which explains why it does not clear data, as this is not necessary for testing purposes.

	// Note about tests that return true or false: For a test to pass (exhibit intended behaviour) the result must be
	// true. Any false values are considered a failed test.

	printf("\n------------------------------------------------------\n                  *record.c tests*\n");

	// ----------------------------------------------------------------------------------------------------------------
	// 1 - Test 'create_record()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n1. create_record() test\n----------------\n");

	// 1.1 - Tests if the function can create and return a record without throwing any errors
	testing_record = create_record();
	printf("1.1 - Test passed! Record successfully created!\n");

	// ----------------------------------------------------------------------------------------------------------------
	// 2 - Test 'compare_records()'
	// ----------------------------------------------------------------------------------------------------------------
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
	change_string_identifier(&testing_record, "AAAAB");
	change_string_identifier(&other_record, "AAAAA");

	result = compare_records(&testing_record, &other_record);
	printf("\n2.5 - Expected Result: 1\n2.5 - Actual Result: ");
	printf("%d", result);
	
	// 2.6 - Tests when the identifiers are both strings, and when the first record comes before the second
	change_string_identifier(&other_record, "AAAAC");

	result = compare_records(&testing_record, &other_record);
	printf("\n2.6 - Expected Result: -1\n2.6 - Actual Result: ");
	printf("%d", result);

	// 2.7 - Tests when the identifiers are both strings, and when the records are the same
	change_string_identifier(&other_record, "AAAAB");

	result = compare_records(&testing_record, &other_record);
	printf("\n2.7 - Expected Result: 0\n2.7 - Actual Result: ");
	printf("%d", result);

	// 2.8 - Tests when the identifiers are ints, and the first is larger than the second
	testing_record.identifier_type = IT_INT;
	other_record.identifier_type = IT_INT;
	testing_record.identifier = NULL;
	other_record.identifier = NULL;
	change_int_identifier(&testing_record, 313);
	change_int_identifier(&other_record, 312);

	result = compare_records(&testing_record, &other_record);
	printf("\n2.8 - Expected Result: 1\n2.8 - Actual Result: ");
	printf("%d", result);

	// 2.9 - Tests when the identifiers are ints, and the first is smaller than the second
	change_int_identifier(&other_record, 314);

	result = compare_records(&testing_record, &other_record);
	printf("\n2.9 - Expected Result: -1\n2.9 - Actual Result: ");
	printf("%d", result);

	// 2.10 - Tests when the identifiers are ints, and the first is the same as the second
	change_int_identifier(&testing_record, 314);

	result = compare_records(&testing_record, &other_record);
	printf("\n2.10 - Expected Result: 0\n2.10 - Actual Result: ");
	printf("%d", result);

	// ----------------------------------------------------------------------------------------------------------------
	// 3 - Test print_record()
	// ----------------------------------------------------------------------------------------------------------------
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
	change_string_identifier(&testing_record, "John");
	printf("\n3.3 - Expected Result: John\n3.3 - Actual Result: ");
	print_record(&testing_record);

	// 3.4 - Tests when the identifier is an int and has a value
	testing_record.identifier_type = IT_INT;
	testing_record.identifier = NULL;
	change_int_identifier(&testing_record, 40);
	printf("\n3.4 - Expected Result: 40\n3.4 - Actual Result: ");
	print_record(&testing_record);

	// ----------------------------------------------------------------------------------------------------------------
	// 4 - Test clone_record()
	// ----------------------------------------------------------------------------------------------------------------
	printf("\n----------------\n4. clone_record() test\n----------------\n");

	// 4.1 - Test cloning an default record
	testing_record.identifier = NULL;
	testing_record.identifier_type = IT_UNSET;
	cloning_test_record = clone_record(&testing_record);

	// Identifiers should be equal (both NULL) and not share memory addresses. The two variables should also occupy 
	// different memory addresses
	printf("4.1 - Records have different memory addresses? ");
	printf("%s\n", &testing_record != cloning_test_record ? "true" : "false");
	printf("4.1 - Identifiers have different memory addresses? ");
	printf("%s\n", &testing_record.identifier != cloning_test_record->identifier ? "true" : "false");
	printf("4.1 - Identifiers are both NULL? ");
	printf("%s\n", testing_record.identifier == NULL && cloning_test_record->identifier == NULL ? "true" : "false");

	// 4.2 - Test cloning a record with an identifier type of int, but with a null identifier. This should not trigger the 
	//		 outer if statement that copies identifiers. This tests the outer if statement
	destroy_record(cloning_test_record);
	testing_record.identifier_type = IT_INT;
	cloning_test_record = clone_record(&testing_record);

	// The two variables should occupy different memory addresses and have the same identifier_type
	printf("4.2 - Records have different memory addresses? ");
	printf("%s\n", &testing_record != cloning_test_record ? "true" : "false");
	printf("4.2 - Identifiers have the same identifier_type? ");
	printf("%s\n", testing_record.identifier_type == cloning_test_record->identifier_type ? "true" : "false");
	printf("4.2 - Identifiers are both still NULL? ");
	printf("%s\n", testing_record.identifier == NULL && cloning_test_record->identifier == NULL ? "true" : "false");

	// 4.3 - Test cloning a record with a set int identifier. This tests the first outer if statement and the first
	//	     if statement within it
	destroy_record(cloning_test_record);
	change_int_identifier(&testing_record, 2000);
	cloning_test_record = clone_record(&testing_record);

	// The two variables should have the same identifier and the identifier of both should be in different locations.
	printf("4.3 - Identifiers are the same? ");
	printf("%s\n", compare_records(&testing_record, cloning_test_record) == 0 ? "true" : "false");
	printf("4.3 - Identifiers are in different memory locations? ");
	printf("%s\n", testing_record.identifier != cloning_test_record->identifier ? "true" : "false");

	// 4.4 - Test cloning a record with a set string identifier. A similar test to 4.2, where the identifier is not 
	//		 set and the identifier_type is set is pointless, as the outer if statement has already been tested.
	//       This tests the else statement within the first if statement.
	destroy_record(cloning_test_record);
	free(testing_record.identifier);
	testing_record.identifier = NULL;
	testing_record.identifier_type = IT_STRING;
	change_string_identifier(&testing_record, "MEOW");
	cloning_test_record = clone_record(&testing_record);

	// The two variables should have the same identifier.
	printf("4.4 - Identifiers are the same? ");
	printf("%s\n", compare_records(&testing_record, cloning_test_record) == 0 ? "true" : "false");
	printf("4.4 - Identifiers are in different memory locations? ");
	printf("%s\n", testing_record.identifier != cloning_test_record->identifier ? "true" : "false");


	// Further tests will be developed for this function once the functions it will need to rely on are implemented.
	// Logic based on the 'data' field of a record cannot be tested completely until prototype 2 data structures have
	// been implemented

	// ----------------------------------------------------------------------------------------------------------------
	// 5 - Test change_int_identifier()
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n5. change_int_identifier() test\n----------------\n");

	// 5.1 - Test changing the identifier when the identifier is currently NULL (bypassing the 'if' statement)

	testing_record.identifier_type = IT_INT;
	free(testing_record.identifier);
	testing_record.identifier = NULL;
	change_int_identifier(&testing_record, 2);

	printf("5.1 - Expected Result: 2\n5.1 - Actual Result: ");
	print_record(&testing_record);
	// If '2' was directly assigned to testing_record.identifier, the memory location would equal 2, rather than it
	// pointing to an int that equals 2.
	printf("\n5.1 - Identifier memory location does not equal 2? "); 
	printf("%s", testing_record.identifier != 2 ? "true" : "false");

	// 5.2 - Test changing the identifier when the identifier is already set (enters the if statement)
	change_int_identifier(&testing_record, 3);

	printf("\n5.2 - Expected Result: 3\n5.2 - Actual Result: ");
	print_record(&testing_record);

	// ----------------------------------------------------------------------------------------------------------------
	// 6 - Test change_string_identifier()
	// ----------------------------------------------------------------------------------------------------------------
	printf("\n----------------\n6. change_string_identifier() test\n----------------\n");

	// 6.1 - Test changing the identifier when the identifier is currently NULL (bypassing the 'if' statement)

	testing_record.identifier_type = IT_STRING;
	free(testing_record.identifier);
	testing_record.identifier = NULL;
	change_string_identifier(&testing_record, "hello");

	printf("6.1 - Expected Result: hello\n6.1 - Actual Result: ");
	print_record(&testing_record);
	// If 'hello' was directly assigned to testing_record.identifier, the memory location would store a string literal
	// rather than it pointing to a string.
	printf("\n6.1 - Identifier points to a string? ");
	printf("%s", testing_record.identifier != "hello" ? "true" : "false");

	// 6.2 - Test changing the identifier when the identifier is already set (enters the if statement)
	change_string_identifier(&testing_record, "hello there");

	printf("\n6.2 - Expected Result: hello there\n6.2 - Actual Result: ");
	print_record(&testing_record);

}
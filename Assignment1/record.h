// record.h
#pragma once

typedef char* String;

typedef enum {
	DT_LIST,
	DT_TREE,
	DT_UNSET
} DataType;

typedef enum {
	IT_STRING,
	IT_INT,
	IT_UNSET
} IdentifierType;

/**
 * @struct record
 * @brief Represents a record within the database
 *
 * Contains an identifier for the record, the type of that identifier, and the data of the record which is in all cases
 * either a list or AVL tree of fighters
 */
typedef struct record {
	void* identifier;
	IdentifierType identifier_type;
	void* data;
} Record;

/**
 * @brief Creates a new, empty record
 *
 * @return The new record
 */
Record create_record();

/**
 * @brief Compares the identifiers of two records
 *
 * @param self The first record to compare
 * @param other The second record to compare
 * 
 * @return An int representing which record comes before or after the other. -1 if the first comes before the second, 
 * 0 if they are the same, and 1 if the first comes after the second
 */
int compare_records(Record* self, Record* other);

/**
 * @brief Compares a record to an identifier
 *
 * @param self The record to compare
 * @param identifier The identifier to compare to the record
 * @param type The type of the identifier to compare to the record
 *
 * @return An int representing which record comes before or after the other. -1 if the first comes before the second,
 * 0 if they are the same, and 1 if the first comes after the second
 */
int compare_record_by_identifier(Record* self, void* identifier, IdentifierType type);

/**
 * @brief Prints the identifier of a record
 *
 * @param self The record to print
 */
void print_record(Record* self);

/**
 * @brief Clones a record and returns the clone
 *
 * @param self The record to clone
 *
 * @return Pointer to the new clone record
 */
Record* clone_record(Record* self);

/**
 * @brief Changes the identifier of a record when the identifier type is int
 *
 * @param self The record with the identifier to change
 * @param num The int that will become the new identifier
 */
void change_int_identifier(Record* self, int num);

/**
 * @brief Changes the identifier of a record when the identifier type is String
 *
 * @param self The record with the identifier to change
 * @param str The String that will become the new identifier
 */
void change_string_identifier(Record* self, String str);

/**
 * @brief Tests all functions within the file
 */
void test_record();

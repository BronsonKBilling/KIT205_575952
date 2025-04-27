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

typedef struct record {
	void* identifier;
	IdentifierType identifier_type;
	void* data;
	DataType data_type;
} Record;

Record create_record();

int compare_records(Record* self, Record* other);

int compare_record_by_identifier(Record* self, void* identifier, IdentifierType type);

void print_record(Record* self);

Record* clone_record(Record* record);

void destroy_record(Record* self);

void change_int_identifier(Record* self, int num);

void change_string_identifier(Record* self, String str);

void test_record();

// record.h
#pragma once

typedef enum {
	DT_STRING,
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

void print_record(Record* self);

void test_record();

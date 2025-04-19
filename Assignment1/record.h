// record.h
#pragma once

typedef enum {
	STRING,
	LIST,
	TREE
} DataType;

typedef enum {
	STRING,
	INT
} IdentifierType;

typedef struct record {
	void* identifier;
	IdentifierType identifier_type;
	void* data;
	DataType data_type;
} Record;

Record create_record();

int compare_records(Record* self, Record other);

void print_record(Record* self);

void test_record(Record* self);

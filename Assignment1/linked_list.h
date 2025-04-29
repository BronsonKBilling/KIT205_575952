// linked_list.h
#pragma once

#include "record.h"

// Code is derivative of Week 6 tutorial

typedef char* String;

typedef struct listNode {
	Record* data; 
	struct listNode* next;
	struct listNode* prev;
} *ListNodePtr;

typedef struct list {
	ListNodePtr head;
	ListNodePtr tail;
} List;

List create_list();

void insert_list(List* self, Record* data);

void delete_node(List* self, ListNodePtr node_to_delete);

void print_list(List* self);

ListNodePtr find_list(List* self, void* identifier, IdentifierType type);

void test_list();
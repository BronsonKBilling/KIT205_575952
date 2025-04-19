// linked_list.h
#pragma once

#include "record.h"

// Code is derivative of Week 6 tutorial

typedef char* String;

typedef struct listNode {
	Record data; 
	struct listNode* next;
	struct listnode* prev;
} *ListNodePtr;

typedef struct list {
	ListNodePtr head;
	ListNodePtr tail;
} List;

List create_list();

void print_list(List* self);

void print_one_to_many(List* self, Record* match);

void insert(List* self, Record* data);

void test_list(List* self);
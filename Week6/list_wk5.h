// list_wk5.h
#pragma once

//TODO: change data type to char*
//Hint: use following typedef to make code clearer
typedef char* String;

typedef struct listNode {
	String data;
	struct listNode* next;
} *ListNodePtr;

typedef struct list {
	ListNodePtr head;
} List;


List new_list();

void print_list(List* self);

void insert_at_front(List* self, int data);

void delete_from_list(List* self, int data);

void destroy_list(List* self);

void list_test();
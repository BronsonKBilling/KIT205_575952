// hashtable_wk5.c
#include <stdio.h>
#include <stdlib.h>
#include "hashtable_wk5.h"
#include "list_wk5.h"

HashTable create_hashtable(int n) {
	HashTable newtable;

	// TODO: allocate memory for array and init lists
	newtable.size = n;
	newtable.table = malloc(sizeof(List) * n);

	for (int i = 0; i < n; i++)
	{
		newtable.table[i] = new_list();
	}

	return newtable;
}

int hash(String key, int size) {
	unsigned long h = 0; // unsigned long, so that when it wraps it's still positive
	int n = strlen(key);

	for (int i = 0; i < n; i++) {
		// multiply by 32 (<<5) and add ascii value
		h = (h << 5) + (int)(key[i]);
	}
	return h % size;
}

void hash_insert(HashTable* self, String key) {
	int insert_hash;
	// 1. find the list to insert into using hash
	insert_hash = hash(key, self->size);

	// 2. call list function to insert into that list
	insert_at_front(&self->table[insert_hash], key);
}

void hash_remove(HashTable* self, String key) {
	// TODO

}

void hash_print(HashTable* self) {
	for (int i = 0; i < self->size; i++) {
		printf("%d: ", i);
		print_list(&(self->table[i]));
	}
}

void hash_destroy(HashTable* self) {
	// TODO
}

void hash_adhoc_test() {
	int command = 1;
	HashTable table = create_hashtable(11);
	char buffer[100];

	while (command) {
		scanf_s("%d", &command);
		switch (command) {
		case 1:
			scanf_s("%s", buffer, 100);
			hash_insert(&table, buffer);
			break;
		case 2: // TODO: remove
			break;
		case 3: // TODO: print
			hash_print(&table);
			break;
		}
		printf("\n");
	}
	// TODO: destroy
}
// avl_tree.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "record.h"
#include "avl_tree.h"

// Creates and returns an empty AVL tree
AVL create_avl() {
	AVL new_tree;
	new_tree.root = NULL;

	return new_tree;
}

// Recursively finds a node containing the record with the given identifier
AVLNodePtr find_avl_node(AVLNodePtr self, String identifier) {
	if (self == NULL || strcmp(self->data->identifier, identifier) == 0) { /* not present, or here */
		return self;
	}
	else if (strcmp(self->data->identifier, identifier) > 0) {
		return find_avl_node(self->left, identifier);
	}
	else {
		return find_avl_node(self->right, identifier);
	}
}

// Finds an identifier that identifies a record within an AVL node. Returns the node if found or null if not
AVLNodePtr find_avl(AVL* self, String identifier) {
	return find_avl_node(self->root, identifier);
}

// Retrieves the height stored in the height field of the node, or -1 if the node is NULL
void find_left_right_height(AVLNodePtr self, int* left, int* right) {
	if (self->left == NULL) {
		*left = -1;
	}
	else {
		*left = self->left->height;
	}

	if (self->right == NULL) {
		*right = -1;
	}
	else {
		*right = self->right->height;
	}
}

int get_avl_node_height(AVLNodePtr self) {
	int left_height;
	int right_height;
	int height;

	if (self == NULL) {
		height = 0;
	}
	else {
		left_height = get_avl_node_height(self->left);
		right_height = get_avl_node_height(self->right);

		// The case of the right height and the left height being the same, but not equalling 0 is dealt with appropriately
		// in the else statement
		if (left_height == 0 && right_height == 0)
		{
			height = 1;
		}
		else if (left_height > right_height) {
			height = left_height + 1;
		}
		else {
			height = right_height + 1;
		}
	}

	return height;
}

AVLNodePtr left_rotate(AVLNodePtr self) {
	// The left child of y becomes the left child of x
	// x becomes the new right child of y
	// Whatever reference originally pointed to x now points to y

	AVLNodePtr right_child = self->right; // Used so the right child is not lost when rotating

	// Rotate
	self->right = right_child->left;
	right_child->left = self;
	 
	// Recalculate Heights
	right_child->height = get_avl_node_height(right_child);
	self->height = get_avl_node_height(self);

	return right_child;
}

AVLNodePtr right_rotate(AVLNodePtr self) {
	// The right child of x becomes the left child of y
	// Y becomes the new right child of x
	// Whatever reference originally pointed to y now points to x

	AVLNodePtr left_child = self->left; // Used so the left child is not lost when rotating

	// Rotate
	self->left = left_child->right;
	left_child->right = self;

	// Recalculate Heights
	left_child->height = get_avl_node_height(left_child);
	self->height = get_avl_node_height(self);

	return left_child;
}

// Balances the AVL tree at the point where the node is
AVLNodePtr balance_tree(AVLNodePtr self) {
	// Having these two variables make comparisons simpler as the heights do not need to be checked multiple times
	// It also makes it easier to factor in NULL nodes
	int leftHeight;  // The height of the left subtree
	int rightHeight; // The height of the right subtree
	bool left_initial_imbalance; // Whether or not self is imbalanced to the left or not
	ImbalanceType imbalance;
	AVLNodePtr updated_root = self;

	// get left and right heights
	find_left_right_height(self, &leftHeight, &rightHeight);

	// Assess imbalance
	if (leftHeight - rightHeight > 0) {
		left_initial_imbalance = true;
	}
	else {
		left_initial_imbalance = false;
	}

	

	if (left_initial_imbalance) {
		find_left_right_height(self->left, &leftHeight, &rightHeight);
		if (leftHeight - rightHeight > 0) {
			imbalance = LL;
		} 
		else {
			imbalance = LR;
		}
	}
	else {
		find_left_right_height(self->right, &leftHeight, &rightHeight);
		if (leftHeight - rightHeight > 0) {
			imbalance = RL;
		}
		else {
			imbalance = RR;
		}
	}

	// Rotate the nodes
	switch (imbalance)
	{
		case LL:
			updated_root = right_rotate(self);
			break;
		case LR:
			self->left = left_rotate(self->left);
			updated_root = right_rotate(self);
			break;
		case RL:
			self->right = right_rotate(self->right);
			updated_root = left_rotate(self);
			break;
		case RR:
			updated_root = left_rotate(self);
	}

	return updated_root;
}

// Recursively inserts a new node into an AVL tree with the given record as its data
AVLNodePtr insert_avl_node(AVLNodePtr self, Record* data) {
	// Having these two variables make comparisons simpler as the heights do not need to be checked multiple times
	// It also makes it easier to factor in NULL nodes
	int leftHeight;  // The height of the left subtree
	int rightHeight; // The height of the right subtree
	AVLNodePtr updated_node = NULL;
	Record* copied_record = clone_record(data);

	if (self == NULL) { /* found where to put it*/
		self = malloc(sizeof * self);
		self->data = copied_record;
		self->left = self->right = NULL;
		self->height = 0;

	}
	else if (compare_records(copied_record, self->data) == -1) {
		self->height = self->height + 1;
		self->left = insert_avl_node(self->left, copied_record);

	}
	else if (compare_records(copied_record, self->data) == 1) {
		self->height = self->height + 1;
		self->right = insert_avl_node(self->right, copied_record);
	}
	
	// get left and right heights
	find_left_right_height(self, &leftHeight, &rightHeight);

	if (abs(leftHeight - rightHeight) > 1) {
		updated_node = balance_tree(self);
	}

	return updated_node != NULL ? updated_node : self;
}

// Inserts a node with a given record item into an AVL tree
void insert_avl(AVL* self, Record* data) {
	self->root = insert_avl_node(self->root, data);
}

// recursive function to print in order
void print_avl_node(AVLNodePtr self) {
	if (self == NULL) {
		printf("_");
	}
	else {
		printf("(");
		print_avl_node(self->left);
		print_record(self->data);
		print_avl_node(self->right);
		printf(")");
	}
}

// Prints the identifiers of records in the data field of each AVL node, in ascending order
void print_avl(AVL* self) {
	print_avl_node(self->root);
}

// Tests all functions within the avl_tree.c file
void test_avl() {
	AVL test_tree = create_avl();
	Record testing_record = create_record();

	testing_record.identifier_type = IT_STRING;
	change_string_identifier(&testing_record, "Alex");

	insert_avl(&test_tree, &testing_record);

	print_avl(&test_tree);

	printf("\n");
	change_string_identifier(&testing_record, "Bryce");
	insert_avl(&test_tree, &testing_record);
	print_avl(&test_tree);

	printf("\n");
	change_string_identifier(&testing_record, "Charlie");
	insert_avl(&test_tree, &testing_record);
	print_avl(&test_tree);

	printf("\n");
	change_string_identifier(&testing_record, "Danny");
	insert_avl(&test_tree, &testing_record);
	print_avl(&test_tree);

	printf("\n");
	change_string_identifier(&testing_record, "Daanni");
	insert_avl(&test_tree, &testing_record);
	print_avl(&test_tree);

	print_record(test_tree.root->right->data);

	print_record(find_avl(&test_tree, "Danny")->data);
}
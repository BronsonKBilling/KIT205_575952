#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bst.h"

// create an empty bst
BST new_bst() {
	BST temp;
	temp.root = NULL;
	return temp;
}

// recursive function to find a value and return the containing node
BSTNodePtr find_bst_node(BSTNodePtr self, int n) {
	if (self == NULL || (n == self->data_item)) { /* not present, or here */
		return self;
	}
	else if (n < self->data_item) {
		return find_bst_node(self->left, n);
	}
	else {
		return find_bst_node(self->right, n);
	}
}

// find a value in the tree and return the node
BSTNodePtr find_bst(BST* self, int n) {
	return find_bst_node(self->root, n);
}

// recursive function to insert a value
BSTNodePtr insert_bst_node(BSTNodePtr self, int n) {
	if (self == NULL) { /* found where to put it*/
		self = malloc(sizeof * self);
		self->data_item = n;
		self->left = self->right = NULL;
	}
	else if (n < self->data_item) {
		self->left = insert_bst_node(self->left, n);
	}
	else if (n > self->data_item) {
		self->right = insert_bst_node(self->right, n);
	}
	return self;
}

// insert a value into the tree
void insert_bst(BST* self, int n) {
	self->root = insert_bst_node(self->root, n);
}

// helper function for delete
BSTNodePtr min_node(BSTNodePtr self) {
	BSTNodePtr current = self;
	while (current->left != NULL) {
		current = current->left;
	}
	return current;
}

// recursive function to delete a value
BSTNodePtr delete_bst_node(BSTNodePtr self, int n) {
	if (self != NULL) {
		if (n == self->data_item) { // found item 
			if (self->left != NULL && self->right != NULL) {
				// two child case 
				BSTNodePtr successor = min_node(self->right);
				self->data_item = successor->data_item;
				self->right = delete_bst_node(self->right, self->data_item);
			}
			else { // one or zero child case 
				BSTNodePtr to_free = self;
				if (self->left) {
					self = self->left;
				}
				else {
					self = self->right;
				}
				free(to_free);
			}
		}
		else if (n < self->data_item) {
			self->left = delete_bst_node(self->left, n);
		}
		else {
			self->right = delete_bst_node(self->right, n);
		}
	}
	return self;
}

// delete a value from the tree
void delete_bst(BST* self, int n) {
	self->root = delete_bst_node(self->root, n);
}

// recursive function to print in order
void print_in_order_bst_node(BSTNodePtr self) {
	if (self == NULL) {
		printf("_");
	}
	else {
		printf("(");
		print_in_order_bst_node(self->left);
		printf(" %d ", self->data_item);
		print_in_order_bst_node(self->right);
		printf(")");
	}
}

// print the tree in order
void print_in_order_bst(BST* self) {
	print_in_order_bst_node(self->root);
}

// recursive function to detroy all node
void destroy_bst_node(BSTNodePtr self) {
	if (self != NULL) {
		destroy_bst_node(self->left);
		self->left = NULL;
		destroy_bst_node(self->right);
		self->right = NULL;
		free(self);
	}
}

// destroy the tree
void destroy_bst(BST* self) {
	destroy_bst_node(self->root);
	self->root = NULL;
}

// Recursive funciton to print in pre order
void print_pre_order_bst_node(BSTNodePtr self) {
	if (self == NULL) {
		printf("_");
	}
	else {
		printf("(");
		printf(" %d ", self->data_item);
		print_pre_order_bst_node(self->left);
		print_pre_order_bst_node(self->right);
		printf(")");
	}
}

// Recursive funciton to print in post order
void print_post_order_bst_node(BSTNodePtr self) {
	if (self == NULL) {
		printf("_");
	}
	else {
		printf("(");
		print_post_order_bst_node(self->left);
		print_post_order_bst_node(self->right);
		printf(" %d ", self->data_item);	
		printf(")");
	}
}

// Print tree in pre order
void print_pre_order_bst(BST* self) {
	print_pre_order_bst_node(self->root);
}

// Print tree in post order
void print_post_order_bst(BST* self) {
	print_post_order_bst_node(self->root);
}

// Recursively gets height of BST
int get_bst_node_height(BSTNodePtr self) {
	int left_height;
	int right_height;
	int height;

	if (self == NULL) {
		height = 0;
	}
	else {
		left_height = get_bst_node_height(self->left);
		right_height = get_bst_node_height(self->right);

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

// Get height of BST
int get_bst_height(BST* self) {
	return get_bst_node_height(self->root);
}

void bst_adhoc_test() {
	BST tree = new_bst();
	int quit = 0;
	int data;
	while (quit == 0) {
		printf("Enter some data: ");
		scanf_s("%d", &data);
		if (data != 0) {
			insert_bst(&tree, data);
		}
		else {
			quit = 1;
		}
	}
	print_in_order_bst(&tree);
	printf("\n");
}

void bst_test() {
	int UNBALANCED_TREE_NUMBERS[] = { 1, 2, 3, 4, 5, 6 }; // Height of 6
	int BALANCED_TREE_NUMBERS[] = { 50, 25, 75, 10, 1, 20, 100};  // Height of 4

	BST empty_tree = new_bst();		
	BST unbalanced_tree = new_bst();
	BST balanced_tree = new_bst();
	BSTNodePtr result;

	// Insert ints into unbalanced tree. (Assumes that ints are 4 bytes)
	for (int i = 0; i < sizeof(UNBALANCED_TREE_NUMBERS) / 4; i++)
	{
		insert_bst(&unbalanced_tree, UNBALANCED_TREE_NUMBERS[i]);
	}

	// Insert ints into balanced tree. (Assumes that ints are 4 bytes)
	for (int i = 0; i < sizeof(BALANCED_TREE_NUMBERS) / 4; i++)
	{
		insert_bst(&balanced_tree, BALANCED_TREE_NUMBERS[i]);
	}

	// NOTE ABOUT TESTING: While the tests call the wrapper functions and the test names refer to the wrapper funcitons,
	// the tests are actually testing the private bst node functions. There is basically nothing to test in the wrapper
	// functions, but they are refered to when testing for brevity and simplicity.

	// 1 - Test 'find_bst()'
	printf("----------------\n1. find_bst() tests\n----------------\n");

	// 1.1 - Test first BST is empty. This immediately executes first if statement, does not trigger recursion
	//	     Expected value: NULL
	result = find_bst(&empty_tree, 0);
	if (result == NULL) {
		printf("1.1 - Success!");
	} 
	else {
		printf("1.1 - Error! Value is not expected");
	}

	// 1.2 - Test recursing the left hand side of the tree. Triggers the second if statement, recurses, then the first if statement
	result = find_bst(&balanced_tree, 25);
	printf("\n1.2 - Expected value: 25\n1.2 - Actual value: ");
	printf("%d", result->data_item);

	// 1.3 - Test recursing the right hand side of tree. Triggers the third if statement, then the first.
	result = find_bst(&balanced_tree, 75);
	printf("\n1.3 - Expected value: 75\n1.3 - Actual value: ");
	printf ("%d", result->data_item);
	
	// 1.4 - Test recursion when multiple left and right paths must be taken
	result = find_bst(&balanced_tree, 20);
	printf("\n1.4 - Expected value: 20\n1.4 - Actual value: ");
	printf("%d", result->data_item);

	// 1.5 - Tests when the data item is not present in the tree
	result = find_bst(&balanced_tree, 2000);
	if (result == NULL) {
		printf("\n1.5 - Success!");
	}
	else {
		printf("\n1.5 - Error! Value is not expected");
	}

	// 2 - Test insert_bst()
	printf("\n----------------\n2. insert_bst() tests\n----------------\n");

	// 2.1 - Test insert when the tree is empty. triggers first if statement.
	insert_bst(&empty_tree, 20);
	printf("2.1 - Expected result: (_ 20 _)\n2.1 - Actual result: ");
	print_in_order_bst(&empty_tree);

	// 2.2 - Test inserting into left subtree. Triggers second if statement then first.
	insert_bst(&empty_tree, 10);
	printf("\n2.2 - Expected result: ((_ 10 _) 20 _)\n2.2 - Actual result: ");
	print_in_order_bst(&empty_tree);

	// 2.3 - Test inserting into right subtree. Triggers third if statement, then first.
	insert_bst(&empty_tree, 30);
	printf("\n2.2 - Expected result: ((_ 10 _) 20 (_ 30 _))\n2.2 - Actual result: ");
	print_in_order_bst(&empty_tree);

	// 2.4 - Test inserting into a tree when multiple left and right paths need to be taken. Triggers all if statements
	insert_bst(&balanced_tree, 23);
	printf("\n2.2 - Expected result: ((((_ 1 _) 10 (_ 20 (_ 23 _))) 25 _) 50 (_ 75 (_ 100 _)))\n2.2 - Actual result: ");
	print_in_order_bst(&balanced_tree);

	// 2.5 - Test inserting a duplicate value into the tree
	insert_bst(&empty_tree, 30);
	printf("\n2.2 - Expected result: ((_ 10 _) 20 (_ 30 _))\n2.2 - Actual result: ");
	print_in_order_bst(&empty_tree);

	// 3 - Test destroy_bst()
	printf("\n----------------\n3. destroy_bst() tests\n----------------\n");

	// 3.1 - Test destroying a BST
	destroy_bst(&empty_tree);
	if (empty_tree.root == NULL) {
		printf("3.1 - Success!\n");
	}
	else {
		printf("3.1 - Error! Value is not expected\n");
	}

	// 3.2 - Test destroying an empty BST
	destroy_bst(&empty_tree);
	if (empty_tree.root == NULL) {
		printf("3.2 - Success!\n");
	}
	else {
		printf("3.2 - Error! Value is not expected\n");
	}

	// 4 - Test delete_bst()
	printf("\n----------------\n4. delete_bst() tests\n----------------\n");

	// 4.1 - Test deleting from an empty BST. No if statements should be triggered.
	delete_bst(&empty_tree, 0);
	if (empty_tree.root == NULL) {
		printf("4.1 - Success!");
	}
	else {
		printf("4.1 - Error! Value is not expected");
	}

	// 4.2 - Test deleting a leaf from the left-most side of the tree. Triggers outer if statement, the first if else 
	//		 within that, the if statement within the outer-most if statement, and the single
	//		 or no child case.
	delete_bst(&balanced_tree, 1);
	printf("\n4.2 - Expected result: (((_ 10 (_ 20 (_ 23 _))) 25 _) 50 (_ 75 (_ 100 _)))\n4.2 - Actual result: ");
	print_in_order_bst(&balanced_tree);

	// 4.3 - Test deleting a node with one child from the right side of the tree. Triggers outer if statement, 
	//		 the second if else within that, the if statement within the outer-most if statement, and the single
	//		 or no child case. This tests traversing to the right and deleting nodes that have a child
	delete_bst(&balanced_tree, 75);
	printf("\n4.3 - Expected result: (((_ 10 (_ 20 (_ 23 _))) 25 _) 50 (_ 100 _))\n4.3 - Actual result: ");
	print_in_order_bst(&balanced_tree);

	// 4.4 - Test deleting the root of a tree. Also tests deleting a node with two children. Triggers outer if statement, 
	//		 the if statement within that, and the two child case.
	delete_bst(&balanced_tree, 50);
	printf("\n4.4 - Expected result: (((_ 10 (_ 20 (_ 23 _))) 25 _) 100 _)\n4.4 - Actual result: ");
	print_in_order_bst(&balanced_tree);

	// 4.5 - Test deleting an item that does not exist from a tree.
	delete_bst(&balanced_tree, 50);
	printf("\n4.5 - Expected result: (((_ 10 (_ 20 (_ 23 _))) 25 _) 100 _)\n4.5 - Actual result: ");
	print_in_order_bst(&balanced_tree);

	// 5 - Test print_in_order_bst()
	//     This function's cases have been thoroughly tested indirectly throughout this function, apart from the test below
	printf("\n----------------\n5. print_in_order_bst() tests\n----------------\n");

	// 5.1 - Test printing an empty tree
	printf("5.1 - Expected result: _\n5.1 - Actual result: ");
	print_in_order_bst(&empty_tree);

	// 6 - Test print_pre_order_bst()
	printf("\n----------------\n6. print_pre_order_bst() tests\n----------------\n");

	// 6.1 - Test printing an empty tree
	printf("6.1 - Expected result: _\n6.1 - Actual result: ");
	print_pre_order_bst(&empty_tree);

	// 6.2 - Test printing a tree with items in it
	printf("\n6.2 - Expected result: ( 100 ( 25 ( 10 _( 20 _( 23 __)))_)_)\n6.2 - Actual result: ");
	print_pre_order_bst(&balanced_tree);

	// 7 - Test print_post_order_bst()
	printf("\n----------------\n7. print_post_order_bst() tests\n----------------\n");

	// 7.1 - Test printing an empty tree
	printf("7.1 - Expected result: _\n7.1 - Actual result: ");
	print_post_order_bst(&empty_tree);

	// 7.2 - Test printing a tree with items in it
	printf("\n7.2 - Expected result: (((_(_(__ 23 ) 20 ) 10 )_ 25 )_ 100 )\n7.2 - Actual result: ");
	print_post_order_bst(&balanced_tree);

	// 8 - Test get_bst_height()
	printf("\n----------------\n8. get_bst_height() tests\n----------------\n");

	// 8.1 - Test getting the height of an empty tree
	result = get_bst_height(&empty_tree);
	printf("8.1 - Expected result: 0\n8.1 - Actual result: ");
	printf("%d", result);

	// 8.2 - Test getting the height of a tree with only 1 item. This triggers the else statement and the first if 
	//       statement inside it
	insert_bst(&empty_tree, 100);
	result = get_bst_height(&empty_tree);
	printf("\n8.2 - Expected result: 1\n8.2 - Actual result: ");
	printf("%d", result);

	// 8.3 - Test getting the height of a tree where the tree must find the height of its left subtree(s). Triggers the
	//		 else statement and the first else if within it.
	insert_bst(&empty_tree, 50);
	result = get_bst_height(&empty_tree);
	printf("\n8.3 - Expected result: 2\n8.3 - Actual result: ");
	printf("%d", result);

	// 8.4 - Test getting the height of a tree where the tre must find the height of its right subtree(s). Triggers the
	//		 else statement and the else within it.
	result = get_bst_height(&unbalanced_tree);
	printf("\n8.4 - Expected result: 6\n8.4 - Actual result: ");
	printf("%d", result);

	// 8.5 - Test getting the height of a tree where the tree must find the height of both of its subtree(s). Triggers the
	//		 else statement and all if, else if, and else statements within it.
	insert_bst(&empty_tree, 150);
	result = get_bst_height(&empty_tree);
	printf("\n8.5 - Expected result: 2\n8.5 - Actual result: ");
	printf("%d", result);
}


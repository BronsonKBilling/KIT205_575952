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

// Finds the height of an AVL node. Although nodes have a height field, it is necessary to have this function as the
// height field will not be correct after rotating
int get_avl_node_height(AVLNodePtr self) {
	int left_height;	// The height of the left subtree
	int right_height;	// The height of the right subtree
	int height;		    // The height of self

	if (self == NULL) {
		height = -1;
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

// Gets the height of an avl tree node non recursively. Appropriate when inserting but not when rotating
// This function was adapted from code written by ChatGPT
int get_height_non_recursively(AVLNodePtr self) {
	int leftHeight;	 // The height of the left subtree
	int rightHeight; // The height of the right subtree
	if (self == NULL) {
		leftHeight = -2;
		rightHeight = -2;
	}
	else {
		leftHeight = self->left ? self->left->height : -1;
		rightHeight = self->right ? self->right->height : -1;
	}
	
	return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// Performs a left rotation. This function assumes that self and its right child are not null. This function will
// never be called unless these conditions are met as a left rotation would not be required.
AVLNodePtr left_rotate(AVLNodePtr self) {
	// The left child of self->right becomes the right child of self
	// Self becomes the left child of self->right
	// The reference of the right_child is returned to become the new reference of self

	AVLNodePtr right_child = self->right; // Right child of self. Used so the right child is not lost when rotating

	// Rotate
	self->right = right_child->left;
	right_child->left = self;
	 
	// Recalculate Heights
	right_child->height = get_avl_node_height(right_child);
	self->height = get_avl_node_height(self);

	return right_child;
}

// Performs a right rotation. This function assumes that self and its left child are not null. This function will
// never be called unless these conditions are met as a right rotation would not be required.
AVLNodePtr right_rotate(AVLNodePtr self) {
	// The right child of self->left becomes the left child of self
	// Self becomes the right child of self->left
	// The reference of the left_child is returned to become the new reference of self

	AVLNodePtr left_child = self->left; // Left child of self. Used so the left child is not lost when rotating

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
	ImbalanceType imbalance;	 // The type of imbalance
	AVLNodePtr updated_root = self; // The new root node

	// get left and right heights
	find_left_right_height(self, &leftHeight, &rightHeight);

	// Assess first imbalance
	if (leftHeight - rightHeight > 0) {
		left_initial_imbalance = true;
	}
	else {
		left_initial_imbalance = false;
	}

	// Assess the imbalance
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
		self->left = insert_avl_node(self->left, copied_record);
		int lh = self->left ? self->left->height : -1;
		int rh = self->right ? self->right->height : -1;
		self->height = (lh > rh ? lh : rh) + 1;
	}
	else if (compare_records(copied_record, self->data) == 1) {
		self->right = insert_avl_node(self->right, copied_record);
		int lh = self->left ? self->left->height : -1;
		int rh = self->right ? self->right->height : -1;
		self->height = (lh > rh ? lh : rh) + 1;
	}
	
	// get left and right heights
	find_left_right_height(self, &leftHeight, &rightHeight);

	if (abs(leftHeight - rightHeight) > 1) {
		updated_node = balance_tree(self);
	}

	return updated_node != NULL ? updated_node : self;
}

// Inserts a node with a given record item into an AVL tree. This function assumes that the record is not a duplicate
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

// Recursive function to print in order with no tree structure
void print_avl_node_without_tree(AVLNodePtr self) {
	if (self != NULL) {
		print_avl_node_without_tree(self->left);
		print_record(self->data);
		printf("\n");
		print_avl_node_without_tree(self->right);
		
	}
}

// Prints the identifiers of records in the data field of each AVL node, in ascending order with no tree structure
void print_avl_without_tree(AVL* self) {
	print_avl_node_without_tree(self->root);
}

// Prints the identifiers of records in the data field of each AVL node, in ascending order with a tree structure
void print_avl(AVL* self) {
	print_avl_node(self->root);
}

// deletes all the children of an AVL node and itself. This function will not be used for anything other than testing
void delete_avl(AVLNodePtr self) {
	if (self->left != NULL) {
		delete_avl(self->left);
	}
	
	if (self->right != NULL) {
		delete_avl(self->right);
	}

	// By now, all children should be deleted

	destroy_record(self->data);
	free(self);
}

// Deletes all the children of an AVL node. This function will not be used for anything other than testing
void delete_avl_children(AVL* self) {
	if (self->root->left != NULL) {
		delete_avl(self->root->left);
	}

	if (self->root->right != NULL) {
		delete_avl(self->root->right);
	}

	self->root->left = NULL;
	self->root->right = NULL;
}

// Tests all functions within the avl_tree.c file
void test_avl() {
	AVL test_tree;	// The tree to use for testing
	AVL empty_tree = create_avl();	// An empty tree to use for testing
	Record testing_record = create_record(); // A record to use for testing
	int leftHeight;	 // A variable that will be used to test if functions can get the left height accurately
	int rightHeight; // A variable that will be used to test if functions can get the left height accurately
	AVLNodePtr result; // The result of a search test

	// Note about testing wrappers: Functions such as find_avl_node and other wrapped private functions will be tested 
	// by calling their wrapper functions. This is for convenience and for simplicity. As the wrapper functions simply
	// call the private functions and have no other use or execution paths, they will not be tested directly.

	// Note about tests that return true or false: For a test to pass (exhibit intended behaviour) the result must be
	// true. Any false values are considered a failed test.

	// Note about testing find_left_right_height: This function contains two separate if else statements. These do not
	// impact eachother or rely on eachother at all, so they can be tested at the same time for efficiency.

	// Note about testing get_avl_node_height: This function is only called privately within this file and is never
	// needed by any other files, so a wrapper function is not necessary for this function. This means that when testing
	// this function, it will be called directly and the root of a tree will be the parameter.

	// Note about testing delete_avl_children and delete_avl: These functions only exist to reset the test_tree for the
	// purposes of testing. They will never be used outside of this context. Given this, and the fact that the call to 
	// delete_avl_children below is given a full tree that executes every possible execution path successfully, further
	// testing of this function is not necessary

	// Note about testing balance_tree: This function will be tested by creating imbalances using the insert_avl_node function.
	// This is still able to test all execution paths of the function without the added complexity of having to manually
	// insert nodes. All execution paths of insert_avl_node will still be tested despite them being executed while testing
	// balance_tree

	printf("\n------------------------------------------------------\n                  *avl_tree.c tests*\n");

	// ----------------------------------------------------------------------------------------------------------------
	// 1 - Test 'create_avl()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n1. create_avl() test\n----------------\n");

	// 1.1 - Tests if the function can create and return an AVL tree without throwing any errors
	test_tree = create_avl();
	printf("1.1 - Test passed! AVL Tree successfully created!\n");

	// ----------------------------------------------------------------------------------------------------------------
	// 2 - Test 'find_avl()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n2. find_avl() tests\n----------------\n");

	// 2.1 - Test when AVL is empty. This immediately executes first if statement, does not trigger recursion
	//	     Expected value: NULL
	result = find_avl(&test_tree, "Dave");
	printf("2.1 - result == null? ");
	printf("%s", result == NULL ? "true" : "false");

	// 2.2 - Test recursing the left hand side of the tree. Triggers the second if statement, recurses, then the first if statement

	// Tree root is "Peter" with "Hailey" as the left child
	testing_record.identifier_type = IT_STRING;
	change_string_identifier(&testing_record, "Peter");
	insert_avl(&test_tree, &testing_record);
	change_string_identifier(&testing_record, "Hailey");
	insert_avl(&test_tree, &testing_record);

	result = find_avl(&test_tree, "Hailey");
	printf("\n2.2 - Expected value: Hailey\n2.2 - Actual value: ");
	print_record(result->data);

	// 2.3 - Test recursing the right hand side of tree. Triggers the third if statement, then the first.

	// Right child of "Peter" becomes "Robert"
	change_string_identifier(&testing_record, "Robert");
	insert_avl(&test_tree, &testing_record);

	result = find_avl(&test_tree, "Robert");
	printf("\n2.3 - Expected value: Robert\n2.3 - Actual value: ");
	print_record(result->data);

	// 2.4 - Test recursion when multiple left and right paths must be taken

	// "Lucy" is the left child of "Mike", who is the right child of "Hailey". This means that the 
	// function is recursing multiple times and taking every possible execution path with these executions
	change_string_identifier(&testing_record, "Mike");
	insert_avl(&test_tree, &testing_record);
	change_string_identifier(&testing_record, "Rachel");
	insert_avl(&test_tree, &testing_record);
	change_string_identifier(&testing_record, "Lucy");
	insert_avl(&test_tree, &testing_record);

	result = find_avl(&test_tree, "Lucy");
	printf("\n2.4 - Expected value: Lucy\n2.4 - Actual value: ");
	print_record(result->data);

	// 2.5 - Tests when the data item is not present in the tree
	result = find_avl(&test_tree, "Hose");
	printf("\n2.5 - result == null? ");
	printf("%s\n", result == NULL ? "true" : "false");

	// ----------------------------------------------------------------------------------------------------------------
	// 3 - Test 'find_left_right_height()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n3. find_left_right_height() tests\n----------------\n");

	// 3.1 - Tests when both children are NULL. This tests the first if statement that checks if the left child is NULL,
	//	     and the second that checks if the right child is NULL

	// Finds the left and right heights of the node containing "Rachel", this node is a leaf
	find_left_right_height(test_tree.root->right->left, &leftHeight, &rightHeight);

	printf("3.1 - Expected values: -1, -1\n3.1 - Actual values: ");
	printf("%d, %d\n", leftHeight, rightHeight);

	// 3.2 - Tests when both children have a height. This tests the first and second else statements
	find_left_right_height(test_tree.root, &leftHeight, &rightHeight);

	printf("3.2 - Expected values: 1, 1\n3.2 - Actual values: ");
	printf("%d, %d\n", leftHeight, rightHeight);

	// ----------------------------------------------------------------------------------------------------------------
	// 4 - Test 'get_avl_node_height()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("----------------\n4. get_avl_node_height() tests\n----------------\n");

	// 4.1 - Test getting the height of an empty tree
	result = get_avl_node_height(empty_tree.root);
	printf("4.1 - Expected result: -1\n4.1 - Actual result: ");
	printf("%d", result);

	// 4.2 - Test getting the height of a tree with only 1 item. This triggers the else statement and the first if 
	//       statement inside it
	change_string_identifier(&testing_record, "Pablo");
	insert_avl(&empty_tree, &testing_record);

	result = get_avl_node_height(empty_tree.root);
	printf("\n4.2 - Expected result: 0\n4.2 - Actual result: ");
	printf("%d", result);

	// 4.3 - Test getting the height of a tree where the tree must find the height of its left subtree(s). Triggers the
	//		 else statement and the first else if within it.
	change_string_identifier(&testing_record, "Annie");
	insert_avl(&empty_tree, &testing_record);

	result = get_avl_node_height(empty_tree.root);
	printf("\n4.3 - Expected result: 1\n4.3 - Actual result: ");
	printf("%d", result);

	// 4.4 - Test getting the height of a tree where the tree must find the height of its right subtree(s). Triggers the
	//		 else statement and the else within it.
	destroy_record(empty_tree.root->left->data);
	free(empty_tree.root->left);
	empty_tree.root->left = NULL;
	change_string_identifier(&testing_record, "Randy");
	insert_avl(&empty_tree, &testing_record);

	result = get_avl_node_height(empty_tree.root);
	printf("\n4.4 - Expected result: 1\n4.4 - Actual result: ");
	printf("%d", result);

	// 4.5 - Test getting the height of a tree where the tree must find the height of both of its subtree(s). Triggers the
	//		 else statement and all if, else if, and else statements within it.
	result = get_avl_node_height(test_tree.root);
	printf("\n4.5 - Expected result: 2\n4.5 - Actual result: ");
	printf("%d", result);

	// ----------------------------------------------------------------------------------------------------------------
	// 5 - Test 'left_rotate()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("\n----------------\n5. left_rotate() tests\n----------------");

	// 5.1 - Test a left rotation. This function has no alternative execution paths, so only one test is necessary.
	//		 The assumptions stated in the function header also make trying the function will different NULL values unnecessary.
	//		 After the function is finished and the output is used, the left child of the right child of the node inputted into the
	//	     function should equal the inputs right child, and the inputted node should equal the left child of the input's 
	//		 right child. 

	// The insertion results in a RR imbalance at the node containing  "Steve"
	change_string_identifier(&testing_record, "Steve");
	insert_avl(&test_tree, &testing_record);
	change_string_identifier(&testing_record, "Taylah");
	insert_avl(&test_tree, &testing_record);
	// Manual insertion is needed to not trigger the rotate function premeturely
	change_string_identifier(&testing_record, "Xavier");
	test_tree.root->right->right->right->right = malloc(sizeof(struct avlNode));
	test_tree.root->right->right->right->right->data = clone_record(&testing_record);
	test_tree.root->right->right->right->right->height = 0;
	test_tree.root->right->right->right->right->left = NULL;
	test_tree.root->right->right->right->right->right = NULL;

	// Performs a rotation at "Steve"
	test_tree.root->right->right = left_rotate(test_tree.root->right->right);

	// root->right->right should now equal "Taylah". root->right->right->left should equal "Steve". root->right->right->right
	// should equal "Xavier"

	printf("\n5.1 - Expected result: Taylah\n5.1 - Actual result: ");
	print_record(test_tree.root->right->right->data);
	printf("\n5.1 - Expected result: Steve\n5.1 - Actual result: ");
	print_record(test_tree.root->right->right->left-> data);
	printf("\n5.1 - Expected result: Xavier\n5.1 - Actual result: ");
	print_record(test_tree.root->right->right->right->data);

	// ----------------------------------------------------------------------------------------------------------------
	// 6 - Test 'right_rotate()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("\n----------------\n6. right_rotate() tests\n----------------");

	// 6.1 - Test a right rotation. This function has no alternative execution paths, so only one test is necessary.
	//		 The assumptions stated in the function header also make trying the function will different NULL values unnecessary.
	//		 After the function is finished and the output is used, the right child of the left child of the node inputted into the
	//	     function should equal the inputs left child, and the inputted node should equal the right child of the input's 
	//		 left child. 

	// The insertion results in a LL imbalance at the node containing  "Hailey"
	// Manual insertion is needed to not trigger the rotate function premeturely
	change_string_identifier(&testing_record, "Catrina");
	insert_avl(&test_tree, &testing_record);
	change_string_identifier(&testing_record, "Bryan");
	test_tree.root->left->left->left->left = malloc(sizeof(struct avlNode));
	test_tree.root->left->left->left->left->data = clone_record(&testing_record);
	test_tree.root->left->left->left->left->height = 0;
	test_tree.root->left->left->left->left->left = NULL;
	test_tree.root->left->left->left->left->right = NULL;

	// Performs a rotation at "Hailey"
	test_tree.root->left->left = right_rotate(test_tree.root->left->left);

	// root->left->left should now equal "Catrina". root->left->left->left should equal "Bryan". root->left->left->right
	// should equal "Hailey"

	printf("\n6.1 - Expected result: Catrina\n6.1 - Actual result: ");
	print_record(test_tree.root->left->left->data);
	printf("\n6.1 - Expected result: Bryan\n6.1 - Actual result: ");
	print_record(test_tree.root->left->left->left->data);
	printf("\n6.1 - Expected result: Hailey\n6.1 - Actual result: ");
	print_record(test_tree.root->left->left->right->data);

	// ----------------------------------------------------------------------------------------------------------------
	// 7 - Test 'balance_tree()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("\n----------------\n7. balance_tree() tests\n----------------\n");

	// 7.1 - Test when there is a left-left imbalance. This executes the first if statement, setting left_initial_imbalance
	//		 to true. It then executes the if statement after this and then the if statement within that. It triggers the 
	//	     switch statement that handles LL imbalance and performs a single right rotation.
	
	delete_avl_children(&test_tree);
	change_string_identifier(&testing_record, "Kelly");
	insert_avl(&test_tree, &testing_record);
	
	// Inserting this new node will cause an imbalance
	change_string_identifier(&testing_record, "Bryce");
	insert_avl(&test_tree, &testing_record);

	// If balance_tree worked correctly, the LL imbalance should be corrected with a single right rotation.
	printf("7.1 - Expected result (root): Kelly\n7.1 - Actual result: ");
	print_record(test_tree.root->data);
	printf("\n7.1 - Expected result (left child): Bryce\n7.1 - Actual result: ");
	print_record(test_tree.root->left->data);
	printf("\n7.1 - Expected result (right child): Peter\n7.1 - Actual result: ");
	print_record(test_tree.root->right->data);

	// 7.2 - Test when there is a left-right imbalance. This executes the first if statement, setting left_initial_imbalance
	//		 to true. It then executes the if statement after this and then the else statement within that. It triggers the 
	//	     switch statement that handles LR imbalance and performs a left rotation then a right rotation.

	delete_avl_children(&test_tree);
	change_string_identifier(&testing_record, "India");
	insert_avl(&test_tree, &testing_record);

	// Inserting this new node will cause an imbalance
	change_string_identifier(&testing_record, "Jen");
	insert_avl(&test_tree, &testing_record);

	// If balance_tree worked correctly, the LL imbalance should be corrected with a single right rotation.
	printf("\n7.2 - Expected result (root): Jen\n7.2 - Actual result: ");
	print_record(test_tree.root->data);
	printf("\n7.2 - Expected result (left child): India\n7.2 - Actual result: ");
	print_record(test_tree.root->left->data);
	printf("\n7.2 - Expected result (right child): Kelly\n7.2 - Actual result: ");
	print_record(test_tree.root->right->data);

	// 7.3 - Test when there is a right-left imbalance. This executes the first else statement, setting left_initial_imbalance
	//		 to false. It then executes the else statement after this and then the if statement within that. It triggers the 
	//	     switch statement that handles RL imbalance and performs a right rotation then a left rotation.

	delete_avl_children(&test_tree);
	change_string_identifier(&testing_record, "Peter");
	insert_avl(&test_tree, &testing_record);

	// Inserting this new node will cause an imbalance
	change_string_identifier(&testing_record, "Kayla");
	insert_avl(&test_tree, &testing_record);

	// If balance_tree worked correctly, the LL imbalance should be corrected with a single right rotation.
	printf("\n7.3 - Expected result (root): Kayla\n7.3 - Actual result: ");
	print_record(test_tree.root->data);
	printf("\n7.3 - Expected result (left child): Jen\n7.3 - Actual result: ");
	print_record(test_tree.root->left->data);
	printf("\n7.3 - Expected result (right child): Peter\n7.3 - Actual result: ");
	print_record(test_tree.root->right->data);

	// 7.4 - Test when there is a right-right imbalance. This executes the first else statement, setting left_initial_imbalance
	//		 to false. It then executes the else statement after this and then the else statement within that. It triggers the 
	//	     switch statement that handles RR imbalance and performs a single left rotation.

	delete_avl_children(&test_tree);
	change_string_identifier(&testing_record, "Larry");
	insert_avl(&test_tree, &testing_record);

	// Inserting this new node will cause an imbalance
	change_string_identifier(&testing_record, "Peta");
	insert_avl(&test_tree, &testing_record);

	// If balance_tree worked correctly, the LL imbalance should be corrected with a single right rotation.
	printf("\n7.4 - Expected result (root): Larry\n7.4 - Actual result: ");
	print_record(test_tree.root->data);
	printf("\n7.4 - Expected result (left child): Kayla\n7.4 - Actual result: ");
	print_record(test_tree.root->left->data);
	printf("\n7.4 - Expected result (right child): Peta\n7.4 - Actual result: ");
	print_record(test_tree.root->right->data);

	// ----------------------------------------------------------------------------------------------------------------
	// 8 - Test 'insert_avl()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("\n----------------\n8. insert_avl() tests\n----------------\n");

	delete_avl_children(&test_tree);
	free(test_tree.root->data);
	free(test_tree.root);
	test_tree.root = NULL;

	// 8.1 - Test insert when the tree is empty. Triggers first if statement.
	insert_avl(&test_tree, &testing_record);
	printf("8.1 - Expected result: (_Peta_)\n8.1 - Actual result: ");
	print_avl(&test_tree);

	// 8.2 - Test inserting into left subtree. Triggers second if statement then first.
	change_string_identifier(&testing_record, "Kyle");
	insert_avl(&test_tree, &testing_record);
	printf("\n8.2 - Expected result: ((_Kyle_)Peta_)\n8.2 - Actual result: ");
	print_avl(&test_tree);

	// 8.3 - Test inserting into right subtree. Triggers third if statement, then first.
	change_string_identifier(&testing_record, "Valerie");
	insert_avl(&test_tree, &testing_record);
	printf("\n8.3 - Expected result: ((_Kyle_)Peta(_Valerie_))\n8.3 - Actual result: ");
	print_avl(&test_tree);

	// 8.4 - Test inserting into a tree when left and right paths need to be taken. Triggers all if statements
	change_string_identifier(&testing_record, "Lyra");
	insert_avl(&test_tree, &testing_record);
	printf("\n8.4 - Expected result: ((_Kyle(_Lyra_))Peta(_Valerie_))\n8.4 - Actual result: ");
	print_avl(&test_tree);

	// 8.5 - Test insertion when it will cause an imbalance. This triggers all if statements but also the if statement
	//	     at the end of the function that checks for an imbalance.
	change_string_identifier(&testing_record, "Laine");
	insert_avl(&test_tree, &testing_record);
	printf("\n8.5 - Expected result: (((_Kyle_)Laine(_Lyra_))Peta(_Valerie_))\n8.5 - Actual result: ");
	print_avl(&test_tree);

	// ----------------------------------------------------------------------------------------------------------------
	// 9 - Test 'get_height_non_recursively()'
	// ----------------------------------------------------------------------------------------------------------------
	printf("\n----------------\n9. get_height_non_recursively() tests\n----------------");

	// 9.1 - Test retrieving the height of a node leaf node
	printf("\n9.1 - Expected result: 0\n9.1 - Actual result: ");
	printf("%d", get_height_non_recursively(find_avl(&test_tree, "Valerie")));

	// 9.2 - Test retrieving the height of a null node
	printf("\n9.2 - Expected result: -1\n9.2 - Actual result: ");
	printf("%d", get_height_non_recursively(NULL));

	// 9.3 - Test retrieving the height of a node with valid subtrees
	printf("\n9.3 - Expected result: 2\n9.3 - Actual result: ");
	printf("%d", get_height_non_recursively(find_avl(&test_tree, "Peta")));

}
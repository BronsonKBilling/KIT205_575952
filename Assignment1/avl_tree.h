// avl_tree.h
#pragma once

#include "record.h";

// Code is derivitive of week 5 tutorial

typedef enum {
    LL, // Left-left imblance
    LR, // Left-right 
    RL, // Right-left
    RR // Right-right
} ImbalanceType;

/**
 * @struct avlNode
 * @brief Represents a single node in an AVL tree
 *
 * Contains a record data item and pointers to its left and right children.
 */
typedef struct avlNode {
    Record* data;
    int height;
    struct avlNode* left;
    struct avlNode* right;
} *AVLNodePtr;

/**
 * @struct avl
 * @brief Represents an AVL tree
 *
 * Contains a pointer to the root node of the AVL.
 */
typedef struct avl {
    AVLNodePtr root;
} AVL;

/**
 * @brief Creates a new, empty AVL tree
 *
 * @return The new AVL tree
 */
AVL create_avl();

/**
 * @brief Finds a node containing a data item with a given identifier in an AVL tree
 *
 * @param self Pointer to the AVL tree to search
 * @param identifier The identifier item to search for
 *
 * @return Pointer to the node with the given record, or NULL if it is not found
 */
AVLNodePtr find_avl(AVL* self, String identifier);

/**
 * @brief Inserts a node with a given data item into an AVL tree
 *
 * @param self Pointer to the AVL tree to insert into
 * @param n The data item to insert
 */
void insert_avl(AVL* self, Record* data);

/**
 * @brief Prints the identifiers of records in the data field of each AVL node, in ascending order 
 *
 * @param self Pointer to the AVL tree to print
 */
void print_avl(AVL* self);

/**
 * @brief Tests all functions within the avl_tree.c file
 */
void test_avl();


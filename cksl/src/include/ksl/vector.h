/**
 * @file ksl/vector.h
 * @brief implementation of token vector
 */

#pragma once
#ifndef __KSL_VECTOR_H__
#define __KSL_VECTOR_H__

// include

#include "ksl/token.h"
#include <stddef.h>

// types

// type declares

typedef struct Token Token;

// type defines

/**
 * @brief node of vector
 */
typedef struct VectorNode {
  struct VectorNode *previous_node; ///< previous node
  Token *token;                     ///< the token
  struct VectorNode *next_node;     ///< next node
} VectorNode;

/**
 * @brief a kind of container
 */
typedef struct Vector {
  VectorNode *head_node; ///< head node of vector
  VectorNode *tail_node; ///< tail node of vector
  size_t vector_length;  ///< length of vector
} Vector;

// functions: vector

/**
 * @brief construct a new vector node
 *
 * @param[in] token the value of node
 * @return a new node
 */
extern VectorNode *new_vector_node(Token *token);

/**
 * @brief get an empty vector
 *
 * @return an empty vector
 */
extern Vector *new_vector();

/**
 * @brief append a value to a vector
 *
 * @param[in] vector the vector to use
 * @param[in] token the value to append
 */
extern void append_to_vector(Vector *vector, Token *token);

/**
 * @brief preappend a value to a vector
 *
 * @param[in] vector the vector to use
 * @param[in] token the value to preappend
 */
extern void preappend_to_vector(Vector *vector, Token *token);

/**
 * @brief pop up the last node of a vector
 *
 * @param[in] vector the vector to modify
 * @return the node popped up
 */
extern VectorNode *popup_vector(Vector *vector);

/**
 * @brief get the n-th value of a vector (start from ONE)
 *
 * @param[in] vector the vector to use
 * @param[in] nth the position to get
 * @return the value in the \p nth position
 */
extern VectorNode *get_nth_of_vector(Vector *vector, size_t nth);

/**
 * @brief set the n-th value of a vector (start from ONE)
 *
 * @param[in] vector the vector to use
 * @param[in] nth the position to modify
 * @param[in] token the value to set
 */
extern void set_nth_of_vector(Vector *vector, size_t nth, Token *token);

/**
 * @brief insert value to the n-th position
 *
 * @param[in] vector the vector to use
 * @param[in] nth the position to insert
 * @param[in] token the value to insert
 */
extern void insert_to_vector(Vector *vector, size_t nth, Token *token);

/**
 * @brief delete value from a vector
 *
 * @param[in] vector the vector to use
 * @param[in] nth the position to delete
 * @return the node deleted
 */
extern VectorNode *delete_from_vector(Vector *vector, size_t nth);

/**
 * @brief deconstruct a vector
 *
 * @param[in] vector the vector to drop
 */
extern void drop_vector(Vector *vector);

/**
 * @brief show vector node
 *
 * @param[in] node the node to show
 * @param[in] nth the position of the node
 */
extern void show_vector_node(VectorNode *node, size_t nth);

/**
 * @brief show vector content
 *
 * @param[in] vector the vector to show
 */
extern void show_vector(Vector *vector);

#endif
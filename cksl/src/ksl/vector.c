/**
 * @file ksl/vector.c
 * @brief implementation of token vector
 */

// include

#include "ksl/vector.h"
#include "ksl/token.h"
#include "ksl/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// functions: vector

VectorNode *new_vector_node(Token *token) {
  // boundary test: null token
  is_null(token);
  // construct node
  VectorNode *node = malloc(sizeof(VectorNode));
  node->previous_node = NULL;
  node->token = token;
  node->next_node = NULL;
  // return: node
  return node;
}

Vector *new_vector() {
  // init: empty vector
  Vector *vector = malloc(sizeof(Vector));
  // empty value
  vector->head_node = NULL;
  vector->tail_node = NULL;
  vector->vector_length = 0;
  // return: vector
  return vector;
}

void append_to_vector(Vector *vector, Token *token) {
  // boundary check: null value
  is_null(vector);
  // init: value node
  VectorNode *value_node = new_vector_node(token);
  value_node->previous_node = vector->tail_node;
  // modify the vector
  if (vector->head_node == NULL && vector->tail_node == NULL) {
    // brand new vector
    vector->head_node = value_node;
    vector->tail_node = value_node;
  } else if ((vector->head_node == NULL && vector->tail_node != NULL) ||
             (vector->head_node != NULL && vector->tail_node == NULL)) {
    // wrong vector
    log_error("panic: wrong vector structure found in %s", __func__);
    exit(EXIT_FAILURE);
  } else {
    // normal insertation
    vector->tail_node->next_node = value_node;
    vector->tail_node = value_node;
  }
  vector->vector_length += 1;
}

void preappend_to_vector(Vector *vector, Token *token) {
  // boundary check: null value
  is_null(vector);
  // init: value node
  VectorNode *value_node = new_vector_node(token);
  value_node->next_node = vector->head_node;
  // modify the vector
  if (vector->head_node == NULL && vector->tail_node == NULL) {
    // brand new vector
    vector->head_node = value_node;
    vector->tail_node = value_node;
  } else if ((vector->head_node == NULL && vector->tail_node != NULL) ||
             (vector->head_node != NULL && vector->tail_node == NULL)) {
    // wrong vector
    log_error("panic: wrong vector structure found in %s", __func__);
    exit(EXIT_FAILURE);
  } else {
    // normal insertation
    vector->head_node->previous_node = value_node;
    vector->head_node = value_node;
  }
  vector->vector_length += 1;
}

VectorNode *popup_vector(Vector *vector) {
  return delete_from_vector(vector, vector->vector_length);
}

VectorNode *get_nth_of_vector(Vector *vector, size_t nth) {
  // boundary test: null pointer
  is_null(vector);
  // boundary test: nth > 0
  if (nth < 1) {
    log_error("panic: out of boundary [%zu]", nth);
    exit(EXIT_FAILURE);
  }
  VectorNode *current_node = vector->head_node;
  for (size_t i = 1; i < nth; ++i) {
    VectorNode *next_node = current_node->next_node;
    current_node = next_node;
  }
  return current_node;
}

void set_nth_of_vector(Vector *vector, size_t nth, Token *token) {
  // boundary test: null token
  is_null(token);
  // get the nth node
  VectorNode *node = get_nth_of_vector(vector, nth);
  // modify the value
  drop_token(node->token);
  node->token = token;
}

void insert_to_vector(Vector *vector, size_t nth, Token *token) {
  // init: value node
  VectorNode *value_node = new_vector_node(token);
  // get the node at the specific position
  VectorNode *pos_node = get_nth_of_vector(vector, nth);
  // insert value node
  pos_node->previous_node->next_node = value_node;
  value_node->previous_node = pos_node->previous_node;
  value_node->next_node = pos_node;
  pos_node->previous_node = value_node;
  vector->vector_length += 1;
}

VectorNode *delete_from_vector(Vector *vector, size_t nth) {
  // boundary test: vector
  is_null(vector);
  // boundary test: vector length
  if (nth > vector->vector_length) {
    log_error("panic: out of boundary, cannot delete");
    return NULL;
  }
  // get the specific node
  VectorNode *node = get_nth_of_vector(vector, nth);
  // delete node
  node->previous_node->next_node = node->next_node;
  node->next_node->previous_node = node->previous_node;
  node->next_node = NULL;
  node->previous_node = NULL;
  vector->vector_length -= 1;
  // return: node
  return node;
}

void drop_vector(Vector *vector) {
  // if vector is NULL, do nothing
  if (vector == NULL) {
    return;
  }
  // drop recursively
  VectorNode *current_node = vector->head_node;
  while (current_node != NULL) {
    VectorNode *next_node = current_node->next_node;
    drop_token(current_node->token);
    free(current_node);
    current_node = next_node;
  }
  // free vector
  free(vector);
}

void show_vector_node(VectorNode *node, size_t nth) {
  // boundary test: null pointer
  is_null(node);
  // show node value
  printf("node[%zu]\n", nth);
  printf("  type: %s\n", get_token_type_name(node->token->type));
  if (node->token->value != NULL) {
    printf("  value: [%s]\n", node->token->value);
  }
}

void show_vector(Vector *vector) {
  // boundary check: null vector
  is_null(vector);
  // show vector
  printf("vector: %zu\n", vector->vector_length);
  for (size_t i = 1; i <= vector->vector_length; ++i) {
    show_vector_node(get_nth_of_vector(vector, i), i);
  }
}
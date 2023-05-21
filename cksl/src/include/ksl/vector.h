/**
 * @file ksl/vector.h
 * @brief implementation of generic vector
 */

#pragma once
#ifndef __KSL_VECTOR_H__
#define __KSL_VECTOR_H__

// include

#include "ksl/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// types

// type defines

/**
 * \def new_vector_type (type)
 *
 * @brief generic(笑) vector
 *
 * must implement `void show_##type(type value, size_t indent);`
 */
#define new_vector_type(type)                                                  \
  typedef struct vector_node_##type {                                          \
    struct vector_node_##type *previous_node;                                  \
    type *value;                                                               \
    struct vector_node_##type *next_node;                                      \
  } vector_node_##type;                                                        \
  typedef struct vector_##type {                                               \
    vector_node_##type *head_node;                                             \
    vector_node_##type *tail_node;                                             \
    size_t vector_length;                                                      \
  } vector_##type;                                                             \
  extern vector_node_##type *new_vector_node_##type(type *value);              \
  extern vector_##type *new_vector_##type();                                   \
  extern void append_to_vector_##type(vector_##type *vector, type *value);     \
  extern void preappend_to_vector_##type(vector_##type *vector, type *value);  \
  extern vector_node_##type *get_nth_of_vector_##type(vector_##type *vector,   \
                                                      size_t nth);             \
  extern void set_nth_of_vector_##type(vector_##type *vector, size_t nth,      \
                                       type *value);                           \
  extern void insert_to_vector_##type(vector_##type *vector, size_t nth,       \
                                      type *value);                            \
  extern vector_node_##type *delete_from_vector_##type(vector_##type *vector,  \
                                                       size_t nth);            \
  extern vector_node_##type *popup_vector_##type(vector_##type *vector);       \
  extern void drop_vector_##type(vector_##type *vector);                       \
  extern void show_vector_node_##type(vector_node_##type *node, size_t nth);   \
  extern void show_vector_##type(vector_##type *vector);

/**
 * \def impl_vector_type (type)
 *
 * @brief implementation of \p type vector function
 */
#define impl_vector_type(type)                                                 \
  vector_node_##type *new_vector_node_##type(type *value) {                    \
    is_null(value);                                                            \
    vector_node_##type *node = malloc(sizeof(vector_node_##type));             \
    node->previous_node = NULL;                                                \
    node->value = value;                                                       \
    node->next_node = NULL;                                                    \
    return node;                                                               \
  }                                                                            \
  vector_##type *new_vector_##type() {                                         \
    vector_##type *vector = malloc(sizeof(vector_##type));                     \
    vector->head_node = NULL;                                                  \
    vector->tail_node = NULL;                                                  \
    vector->vector_length = 0;                                                 \
    return vector;                                                             \
  }                                                                            \
  void append_to_vector_##type(vector_##type *vector, type *value) {           \
    is_null(vector);                                                           \
    vector_node_##type *value_node = new_vector_node_##type(value);            \
    value_node->previous_node = vector->tail_node;                             \
    if (vector->head_node == NULL && vector->tail_node == NULL) {              \
      vector->head_node = value_node;                                          \
      vector->tail_node = value_node;                                          \
    } else if ((vector->head_node == NULL && vector->tail_node != NULL) ||     \
               (vector->head_node != NULL && vector->tail_node == NULL)) {     \
      log_error("panic: wrong vector structure found in %s", __func__);        \
      exit(EXIT_FAILURE);                                                      \
    } else {                                                                   \
      vector->tail_node->next_node = value_node;                               \
      vector->tail_node = value_node;                                          \
    }                                                                          \
    vector->vector_length += 1;                                                \
  }                                                                            \
  void preappend_to_vector_##type(vector_##type *vector, type *value) {        \
    is_null(vector);                                                           \
    vector_node_##type *value_node = new_vector_node_##type(value);            \
    value_node->next_node = vector->head_node;                                 \
    if (vector->head_node == NULL && vector->tail_node == NULL) {              \
      vector->head_node = value_node;                                          \
      vector->tail_node = value_node;                                          \
    } else if ((vector->head_node == NULL && vector->tail_node != NULL) ||     \
               (vector->head_node != NULL && vector->tail_node == NULL)) {     \
      log_error("panic: wrong vector structure found in %s", __func__);        \
      exit(EXIT_FAILURE);                                                      \
    } else {                                                                   \
      vector->head_node->previous_node = value_node;                           \
      vector->head_node = value_node;                                          \
    }                                                                          \
    vector->vector_length += 1;                                                \
  }                                                                            \
  vector_node_##type *get_nth_of_vector_##type(vector_##type *vector,          \
                                               size_t nth) {                   \
    is_null(vector);                                                           \
    if (nth < 1) {                                                             \
      log_error("panic: out of boundary [%zu]", nth);                          \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
    vector_node_##type *current_node = vector->head_node;                      \
    for (size_t i = 1; i < nth; ++i) {                                         \
      vector_node_##type *next_node = current_node->next_node;                 \
      current_node = next_node;                                                \
    }                                                                          \
    return current_node;                                                       \
  }                                                                            \
  void set_nth_of_vector_##type(vector_##type *vector, size_t nth,             \
                                type *value) {                                 \
    is_null(value);                                                            \
    vector_node_##type *node = get_nth_of_vector_##type(vector, nth);          \
    drop_##type(node->value);                                                  \
    node->value = value;                                                       \
  }                                                                            \
  void insert_to_vector_##type(vector_##type *vector, size_t nth,              \
                               type *value) {                                  \
    vector_node_##type *value_node = new_vector_node_##type(value);            \
    vector_node_##type *pos_node = get_nth_of_vector_##type(vector, nth);      \
    pos_node->previous_node->next_node = value_node;                           \
    value_node->previous_node = pos_node->previous_node;                       \
    value_node->next_node = pos_node;                                          \
    pos_node->previous_node = value_node;                                      \
    vector->vector_length += 1;                                                \
  }                                                                            \
  vector_node_##type *delete_from_vector_##type(vector_##type *vector,         \
                                                size_t nth) {                  \
    is_null(vector);                                                           \
    if (nth > vector->vector_length) {                                         \
      log_error("panic: out of boundary, cannot delete");                      \
      return NULL;                                                             \
    }                                                                          \
    vector_node_##type *node = get_nth_of_vector_##type(vector, nth);          \
    node->previous_node->next_node = node->next_node;                          \
    node->next_node->previous_node = node->previous_node;                      \
    node->next_node = NULL;                                                    \
    node->previous_node = NULL;                                                \
    vector->vector_length -= 1;                                                \
    return node;                                                               \
  }                                                                            \
  vector_node_##type *popup_vector_##type(vector_##type *vector) {             \
    return delete_from_vector_##type(vector, vector->vector_length);           \
  }                                                                            \
  void drop_vector_##type(vector_##type *vector) {                             \
    if (vector == NULL) {                                                      \
      return;                                                                  \
    }                                                                          \
    vector_node_##type *current_node = vector->head_node;                      \
    while (current_node != NULL) {                                             \
      vector_node_##type *next_node = current_node->next_node;                 \
      drop_##type(current_node->value);                                        \
      free(current_node);                                                      \
      current_node = next_node;                                                \
    }                                                                          \
    free(vector);                                                              \
  }                                                                            \
  void show_vector_node_##type(vector_node_##type *node, size_t nth) {         \
    is_null(node);                                                             \
    printf("node {%zu}:\n", nth);                                              \
    show_##type(node->value, 1);                                               \
  }                                                                            \
  void show_vector_##type(vector_##type *vector) {                             \
    is_null(vector);                                                           \
    printf("vector: %zu\n", vector->vector_length);                            \
    for (size_t i = 1; i <= vector->vector_length; ++i) {                      \
      show_vector_node_##type(get_nth_of_vector_##type(vector, i), i);         \
    }                                                                          \
  }

#endif
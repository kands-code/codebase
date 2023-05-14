/**
 * @file ksl/ksl_parser.c
 * @brief AST parser of KSL
 */

// includes

// self include
#include "ksl/ksl.h"
#include "ksl/ksl_ast.h"
#include "ksl/utils.h"
// standard include
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// functions: parser

char *ksl_interpreter(const char *content) {
  // construct AST from content
  KSLExpNode *nodes = ksl_parser(content);
  if (nodes == NULL) {
    return NULL;
  }
  // eval the AST
  KSLExpNode *result = ksl_eval(nodes);
  // cast result into string
  char *result_buffer = calloc(MAX_STRING_BUFFER_SIZE, sizeof(char));
  switch (result->val_type) {
  case INT:
    sprintf(result_buffer, "%lld", *(long long int *)result->exp_value);
    break;
  case FLT:
    sprintf(result_buffer, "%lf", *(double *)result->exp_value);
    break;
  case STR:
    sprintf(result_buffer, "%s", (char *)result->exp_value);
    break;
  default:
    log_error("panic: wrong node type (%s)", result->ident);
    return NULL;
  }
  // return: result
  return result_buffer;
}

KSLExpNode *ksl_parser(const char *content) {
  size_t content_bias = 0;
  KSLExpNode *nodes = NULL;
  KSLExpNode *prev_node = NULL;
  while (*(content + content_bias) != '\0') {
    char first_char = *(content + content_bias);
    size_t data_bias = 0;
    KSLExpNode *data = NULL;
    if (isblank(first_char)) {
      content_bias++;
      continue;
    } else if (isdigit(first_char)) {
      data = ksl_parser_number(content + content_bias, &data_bias);
    } else if (isupper(first_char)) {
      if (prev_node == NULL) {
        log_error("panic: parse operator error");
        return NULL;
      }
    }
    // check parse result
    if (data == NULL) {
      log_error("panic: parse error");
      return NULL;
    }
    prev_node = data;
    if (nodes == NULL) {
      nodes = data;
    }

    content_bias += data_bias;
  }
  return nodes;
}

KSLExpNode *ksl_parser_number(const char *content, size_t *bias) {
  // init: number
  KSLExpNode *number = malloc(sizeof(KSLExpNode));
  number->ident = 0;
  number->branches = NULL;
  number->branch_number = 0;
  // check content
  char current_char = *(content + *bias);
  bool is_flt = false;
  while (isdigit(current_char) || current_char == '.') {
    if (current_char == '.' && is_flt) {
      log_error("panic: multi-dot in parsing number");
      return NULL;
    } else if (current_char == '.') {
      is_flt = true;
    }
    *bias += 1;
    current_char = *(content + *bias);
  }
  // get number
  char *number_buffer = calloc(*bias + 1, sizeof(char));
  strncpy(number_buffer, content, *bias);
  // check node type
  if (is_flt) {
    number->val_type = FLT;
    double *flt_value = malloc(sizeof(double));
    sscanf(number_buffer, "%lf", flt_value);
    number->exp_value = (void *)flt_value;
  } else {
    number->val_type = INT;
    long long int *int_value = malloc(sizeof(long long int));
    sscanf(number_buffer, "%lld", int_value);
    number->exp_value = (void *)int_value;
  }
  free(number_buffer);
  // return: number
  return number;
}

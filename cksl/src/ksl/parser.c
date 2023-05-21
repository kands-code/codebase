/**
 * @file ksl/parser.c
 * @brief the parser of KSL
 */

// include

#include "ksl/parser.h"
#include "ksl/token.h"
#include "ksl/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// functions: parser

vector_Token **split_Tokens(vector_Token *tokens, size_t *expr_cnt) {
  // boundary test: null tokens
  is_null(tokens);
  // first scan: get the number of expressions
  *expr_cnt = 0;
  vector_node_Token *current_node = tokens->head_node;
  while (current_node != NULL) {
    if (current_node->value->type == Simicolon) {
      *expr_cnt += 1;
    }
    current_node = current_node->next_node;
  }
  // construct: splited tokens
  vector_Token **splited_tokens = calloc(*expr_cnt + 1, sizeof(vector_Token *));
  for (size_t i = 0; i < *expr_cnt + 1; ++i) {
    splited_tokens[i] = new_vector_Token();
  }
  size_t cur_cnt = 0;
  current_node = tokens->head_node;
  while (current_node != NULL) {
    if (current_node->value->type == Simicolon) {
      cur_cnt += 1;
    } else {
      append_to_vector_Token(splited_tokens[cur_cnt],
                             copy_Token(current_node->value));
    }
    current_node = current_node->next_node;
  }
  return splited_tokens;
}

Expression *copy_Expression(Expression *expr) { return NULL; }

void show_Expression(Expression expr, size_t indent) {
  for (size_t i = 0; i < indent; ++i) {
    putchar('\t');
  }
  printf("Expression Type: %d\n", expr.type);
  show_vector_Token(expr.tokens, indent + 1);
}

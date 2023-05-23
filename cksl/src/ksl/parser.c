/**
 * @file ksl/parser.c
 * @brief the parser of KSL
 */

// include

#include "ksl/parser.h"
#include "ksl/token.h"
#include "ksl/utils.h"
#include "ksl/vector.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// functions: parser

impl_vector_type(Expression);

Expression *new_Expression(const char *value, ExpressionType type,
                           vector_Expression *params) {
  Expression *expr = malloc(sizeof(Expression));
  expr->value = str_copy(value);
  expr->type = type;
  // ? notice: no copy, it's move
  expr->params = params;
  return expr;
}

char *get_ExpressionType(ExpressionType type) {
  switch (type) {
  case FuncBindExpr:
    return "FuncBindExpr";
  case StringExpr:
    return "StringExpr";
  case SymbolExpr:
    return "SymbolExpr";
  case IntegerExpr:
    return "IntegerExpr";
  case FloatExpr:
    return "FloatExpr";
  case TypeExpr:
    return "TypeExpr";
  case IdentExpr:
    return "IdentExpr";
  case FuncExpr:
    return "FuncExpr";
  default:
    log_error("panic: wrong expression type [%hu]", type);
    exit(EXIT_FAILURE);
  }
}

vector_Token **split_Tokens_by(vector_Token *tokens, TokenType tok_type,
                               size_t *expr_cnt) {
  // boundary test: null tokens
  is_null(tokens);
  // first scan: get the number of expressions
  *expr_cnt = 1;
  vector_node_Token *current_node = tokens->head_node;
  while (current_node != NULL) {
    if (current_node->value->type == tok_type) {
      if (current_node->next_node != NULL) {
        *expr_cnt += 1;
      }
    }
    current_node = current_node->next_node;
  }
  // construct: splited tokens
  vector_Token **splited_tokens = calloc(*expr_cnt, sizeof(vector_Token *));
  for (size_t i = 0; i < *expr_cnt; ++i) {
    splited_tokens[i] = new_vector_Token();
  }
  size_t cur_cnt = 0;
  current_node = tokens->head_node;
  while (current_node != NULL) {
    if (current_node->value->type == tok_type) {
      cur_cnt += 1;
    } else {
      append_to_vector_Token(splited_tokens[cur_cnt],
                             copy_Token(current_node->value));
    }
    current_node = current_node->next_node;
  }
  return splited_tokens;
}

Expression *copy_Expression(Expression *expr) {
  return new_Expression(expr->value, expr->type, expr->params);
}

void drop_Expression(Expression *expr) {
  if (expr == NULL) {
    return;
  }
  free(expr->value);
  drop_vector_Expression(expr->params);
  free(expr);
}

void show_Expression(Expression *expr, size_t indent) {
  // boundary test: null expression
  is_null(expr);
  indent_printf(indent, "Expression Value: %s", expr->value);
  indent_printf(indent, "Expression Type: %s", get_ExpressionType(expr->type));
  if (expr->params != NULL) {
    indent_printf(indent, "Expression Function Params:");
    show_vector_Expression(expr->params, indent + 1);
  }
}

// ! TODO
static Expression *normal_parser(vector_Token *tokens) {
  // boundary test: null tokens
  is_null(tokens);
  // init: parsed exprssion
  Expression *parsed_expr = NULL;
  // scan
  vector_node_Token *current_node = tokens->head_node;
  while (current_node != NULL) {
    ExpressionType normal_expr_type = IntegerExpr;
    switch (current_node->value->type) {
    case IntegerLiteral:
      normal_expr_type = IntegerExpr;
      break;
    case FloatLiteral:
      normal_expr_type = FloatExpr;
      break;
    case StringLiteral:
      normal_expr_type = StringExpr;
      break;
    case Symbol:
      normal_expr_type = SymbolExpr;
      break;
    case Identifier:
      if (current_node->next_node != NULL &&
          current_node->next_node->value->type == OpenFunction) {
        normal_expr_type = FuncExpr;
      } else {
        normal_expr_type = IdentExpr;
      }
      break;
    default:
      log_error("panic: wrong token type meet [%hu]",
                current_node->value->type);
      exit(EXIT_FAILURE);
    }

    Expression *normal_expr = NULL;
    if (normal_expr_type == FuncExpr) {
    } else {
      normal_expr =
          new_Expression(current_node->value->value, normal_expr_type, NULL);
    }
    if (parsed_expr == NULL) {
      parsed_expr = normal_expr;
    } else if (parsed_expr->type == FuncExpr) {
      append_to_vector_Expression(parsed_expr->params, normal_expr);
    }
    current_node = current_node->next_node;
  }
  return parsed_expr;
}

static Expression *val_bind_parser(vector_Token *lhs, vector_Token *rhs) {
  vector_Expression *params = new_vector_Expression();
  append_to_vector_Expression(
      params, new_Expression(get_nth_of_vector_Token(lhs, 1)->value->value,
                             IdentExpr, NULL));
  append_to_vector_Expression(
      params, new_Expression(get_nth_of_vector_Token(lhs, 3)->value->value,
                             TypeExpr, NULL));
  append_to_vector_Expression(params, normal_parser(rhs));
  Expression *val_bind = new_Expression("ValBind", FuncExpr, params);
  return val_bind;
}

vector_Expression *parser(vector_Token *tokens) {
  // boundary test: null tokens
  is_null(tokens);
  // split tokens
  size_t expr_cnt = 0;
  vector_Token **splited_tokens = split_Tokens_by(tokens, Simicolon, &expr_cnt);
  // process statements
  vector_Expression *exprs = new_vector_Expression();
  for (size_t i = 0; i < expr_cnt; ++i) {
    // first scan
    vector_node_Token *current_node = splited_tokens[i]->head_node;
    bool is_bind_stat = false;
    while (current_node != NULL) {
      // find bind
      if (current_node->value->type == Bind) {
        is_bind_stat = true;
        break;
      }
      current_node = current_node->next_node;
    }
    // check
    if (is_bind_stat) {
      // check value or function
      size_t open_func_cnt = 0;
      size_t close_func_cnt = 0;
      current_node = splited_tokens[i]->head_node;
      while (current_node != NULL && current_node->value->type != Bind) {
        if (current_node->value->type == OpenFunction) {
          open_func_cnt += 1;
        } else if (current_node->value->type == CloseFunction) {
          close_func_cnt += 1;
        }
        current_node = current_node->next_node;
      }
      // spilt at bind
      size_t bind_hand_count = 0;
      vector_Token **splited_bind_tokens =
          split_Tokens_by(splited_tokens[i], Bind, &bind_hand_count);
      // do another check
      if (bind_hand_count != 2) {
        log_error("panic: wrong bind statement with %zu parts",
                  bind_hand_count);
        return NULL;
      }

      // check function
      if (open_func_cnt == 0 && close_func_cnt == 0) {
        // value bind
        //! TODO
        Expression *val_bind =
            val_bind_parser(splited_bind_tokens[0], splited_bind_tokens[1]);
        append_to_vector_Expression(exprs, val_bind);
      } else if (open_func_cnt == close_func_cnt) {
        // function bind
      } else {
        // wrong syntax
        log_error("panic: can not match the function bracket");
        return NULL;
      }
      drop_vector_Token(splited_bind_tokens[0]);
      drop_vector_Token(splited_bind_tokens[1]);
      free(splited_bind_tokens);
    } else {
      // do normal parse
      append_to_vector_Expression(exprs, normal_parser(splited_tokens[i]));
    }
    drop_vector_Token(splited_tokens[i]);
  }
  free(splited_tokens);

  return exprs;
}

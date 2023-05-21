/**
 * @file ksl/token.h
 * @brief the paser of KSL
 */

#pragma once
#ifndef __KSL_PARSER_H__
#define __KSL_PARSER_H__

// include

#include "ksl/token.h"
#include "ksl/vector.h"
#include <stddef.h>

// types

/**
 * @brief types of expression
 */
typedef enum ExpressionType {
  ValueExpr,      ///< pure value, like `23;`, or `"Hello";`
  ValBindExpr,    ///< bind value, `x: Integer := 10;`
  FuncBindExpr,   ///< bind function, `f[x: Integer]: Integer := x + 1;`
  FuncCallExpr,   ///< function calling, `f[10];`
  ArithmeticExpr, ///< pure arithmetic expression, `1 + 2;`
} ExpressionType;

/**
 * @brief expression
 */
typedef struct Expression {
  ExpressionType type;  ///< type of the expression
  vector_Token *tokens; ///< tokens of the expression
} Expression;

//! use vector
new_vector_type(Expression);

// functions: parser

/**
 * @brief split tokens to get expression ranges
 *
 * @param[in] tokens all the tokens
 * @param[out] expr_cnt number expressions
 * @return splited tokens
 */
extern vector_Token **split_Tokens(vector_Token *tokens, size_t *expr_cnt);

/**
 * @brief get a copy of expression
 *
 * @param[in] expr the expression to copy
 * @return the copy of \p expr
 */
extern Expression *copy_Expression(Expression *expr);

/**
 * @brief show expression
 *
 * @param[in] expr the expression to show
 * @param[in] indent the indent before content
 */
extern void show_Expression(Expression expr, size_t indent);

#endif

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

typedef struct vector_Expression vector_Expression;

/**
 * @brief types of expression
 */
typedef enum ExpressionType {
  FuncBindExpr, ///< bind function, `f[x: Integer]: Integer := x + 1;`
  StringExpr,   ///< string type
  SymbolExpr,   ///< symbol type
  IntegerExpr,  ///< inetger type
  FloatExpr,    ///< float type
  TypeExpr,     ///< type expression
  FuncExpr,     ///< function call expression
} ExpressionType;

/**
 * @brief expression
 */
typedef struct Expression {
  char *value;               ///< identifier of expression
  ExpressionType type;       ///< type of the expression
  vector_Expression *params; ///< function body (optional)
} Expression;

//! use vector
new_vector_type(Expression);

// functions: parser

/**
 * @brief construct a new expression
 *
 * @param[in] value the ident or value of expression
 * @param[in] type the type of expression
 * @param[in] params params to expression for function (optoonal)
 * @return new expression
 */
extern Expression *new_Expression(const char *value, ExpressionType type,
                                  vector_Expression *params);

/**
 * @brief get the name of expression type
 *
 * @param[in] type the expression type
 * @return the name of \p type
 */
extern char *get_ExpressionType(ExpressionType type);

/**
 * @brief split tokens to get expression ranges
 *
 * @param[in] tokens all the tokens
 * @param[in] tok_type split token type
 * @param[out] expr_cnt number expressions
 * @return splited tokens
 */
extern vector_Token **split_Tokens_by(vector_Token *tokens, TokenType tok_type,
                                      size_t *expr_cnt);

/**
 * @brief get a copy of expression
 * ! TODO
 * @param[in] expr the expression to copy
 * @return the copy of \p expr
 */
extern Expression *copy_Expression(Expression *expr);

/**
 * @brief delete an expression
 *
 * @param[in] expr the expression to drop
 */
extern void drop_Expression(Expression *expr);

/**
 * @brief show expression
 *
 * @param[in] expr the expression to show
 * @param[in] indent the indent before content
 */
extern void show_Expression(Expression *expr, size_t indent);

// ! TODO
extern vector_Expression *parser(vector_Token *tokens);

#endif

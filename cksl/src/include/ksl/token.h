/**
 * @file ksl/token.h
 * @brief the tokenizer of KSL
 */

#pragma once
#include <stddef.h>
#ifndef __KSL_TOKEN_H__
#define __KSL_TOKEN_H__

// include

#include "ksl/vector.h"

// types

/**
 * @brief types of token
 */
typedef enum TokenType {
  Identity,          ///< identity
  IntegerLiteral,    ///< literal of integer
  FloatLiteral,      ///< literal of floating-point number
  StringLiteral,     ///< literal of string
  Symbol,            ///< symbol
  Type,              ///< type
  TypecColon,        ///< `:` before type
  Bind,              ///< bind operator `:=`
  StructBind,        ///< bind sign in struct `=`
  StructGet,         ///< get sign of struct `->`
  AddOps,            ///< addition sign `+`
  SubOps,            ///< subtraction sign `-`
  MulOps,            ///< multiplication sign `*`
  DivOps,            ///< division sign `/`
  PowOps,            ///< power sign `^`
  Seperator,         ///< seperator `,`
  Simicolon,         ///< simicolon `;`
  ExpressionCombine, ///< combine of expression `|`
  OpenParenthese,    ///< start parenthese `(`
  CloseParenthese,   ///< stop parenthese `)`
  OpenFunction,      ///< start function `[`
  CloseFunction,     ///< stop function `]`
  OpenList,          ///< start list `{`
  CloseList,         ///< stop list `}`
} TokenType;

/**
 * @brief token of KSL
 */
typedef struct Token {
  TokenType type; ///< type of token
  char *value;    ///< value of token
} Token;

//! use vector
new_vector_type(Token);

// functions: token

/**
 * @brief construct a token
 *
 * @param[in] type the type of token
 * @param[in] value the value of token
 * @return the token with \p type and \p value
 */
extern Token *new_Token(TokenType type, char *value);

/**
 * @brief delete a token
 *
 * @param[in] tok the token to drop
 */
extern void drop_Token(Token *tok);

/**
 * @brief show token
 *
 * @param[in] tok the token to show
 * @param[in] ident indent before content
 */
extern void show_Token(Token *tok, size_t indet);

/**
 * @brief get the name of each token type
 *
 * @param[in] type the token type
 * @return the name of \p type
 */
extern char *get_TokenType(TokenType type);

/**
 * @brief tokenize the code
 *
 * @param[in] code the code to parser
 * @return tokens
 */
extern vector_Token *tokenizer(const char *code);

#endif
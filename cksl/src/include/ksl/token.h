/**
 * @file ksl/token.h
 * @brief the tokenizer of KSL
 */

#pragma once
#ifndef __KSL_TOKEN_H__
#define __KSL_TOKEN_H__

// include

#include "ksl/vector.h"

// types

// type declares

typedef struct Vector Vector;

// type defines

/**
 * @brief types of token
 */
typedef enum TokenType {
  Identity,        ///< identity
  IntegerLiteral,  ///< literal of integer
  FloatLiteral,    ///< literal of floating-point number
  StringLiteral,   ///< literal of string
  Symbol,          ///< symbol
  Type,            ///< type
  TypecColon,      ///< `:` before type
  Bind,            ///< bind operator `:=`
  Seperator,       ///< seperator `,`
  Simicolon,       ///< simicolon `;`
  OpenParenthese,  ///< open parenthese `(`
  CloseParenthese, ///< close parenthese `)`
  OpenFunction,    ///< start function `[`
  CloseFunction,   ///< stop function `]`
  OpenList,        ///< start list `{`
  CloseList,       ///< stop list `}`
} TokenType;

/**
 * @brief token of KSL
 */
typedef struct Token {
  TokenType type; ///< type of token
  char *value;    ///< value of token
} Token;

// functions: token

/**
 * @brief construct a token
 *
 * @param[in] type the type of token
 * @param[in] value the value of token
 * @return the token with \p type and \p value
 */
extern Token *new_token(TokenType type, char *value);

/**
 * @brief delete a token
 *
 * @param[in] toekn the token to drop
 */
extern void drop_token(Token *token);

/**
 * @brief get the name of each token type
 *
 * @param[in] type the token type
 * @return the name of \p type
 */
extern char *get_token_type_name(TokenType type);

/**
 * @brief tokenize the code
 *
 * @param[in] code the code to parser
 * @return tokens
 */
extern Vector *tokenizer(const char *code);

#endif
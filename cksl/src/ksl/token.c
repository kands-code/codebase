/**
 * @file ksl/token.c
 * @brief the tokenizer of KSL
 */

// include

#include "ksl/token.h"
#include "ksl/utils.h"
#include "ksl/vector.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

Token *new_token(TokenType type, const char *value) {
  is_null(value);
  // init: token
  Token *token = malloc(sizeof(Token));
  token->type = type;
  // copy value to token
  token->value = calloc(strlen(value) + 1, sizeof(char));
  token->value = strncpy(token->value, value, strlen(value));
  // keep value end with '\0'
  token->value[strlen(value)] = '\0';
  // return: token
  return token;
}

void drop_token(Token *token) {
  // if token is already null, do nothing
  if (token == NULL) {
    return;
  }
  // free all of token
  free(token->value);
  free(token);
}

char *get_token_type_name(TokenType type) {
  switch (type) {
  case Seperator:
    return "Seperator";
  case Bind:
    return "Bind";
  case Simicolon:
    return "Simicolon";
  case Identity:
    return "Identity";
  case IntegerLiteral:
    return "IntegerLiteral";
  case FloatLiteral:
    return "FloatLiteral";
  case StringLiteral:
    return "StringLiteral";
  case OpenParenthese:
    return "OpenParenthese";
  case CloseParenthese:
    return "CloseParenthese";
  case OpenFunction:
    return "OpenFunction";
  case CloseFunction:
    return "CloseFunction";
  case OpenList:
    return "OpenList";
  case CloseList:
    return "CloseList";
  default:
    log_error("panic: wrong token type [%hu]", type);
    exit(EXIT_FAILURE);
  }
}

Vector *tokenizer(const char *code) {
  // boundary test: null pointer
  is_null(code);
  // start tokenize
  Vector *tokens = new_vector();
  bool is_string_literal = false;
  bool is_ident = false;
  bool is_bind = false;
  bool is_number_literal = false;
  bool is_float_literal = false;
  size_t start_point = 0;
  for (size_t i = 0; i < strlen(code); ++i) {
    char current_char = code[i];
    if (!is_string_literal && isblank(current_char)) {
      // blank characters do not matter
      continue;
    } else if (!is_string_literal && is_ident &&
               !(islower(current_char) || isupper(current_char) ||
                 current_char == '_' || isdigit(current_char))) {
      // stop identity
      char *value = calloc(i - start_point + 1, sizeof(char));
      size_t value_cnt = 0;
      for (size_t j = start_point; j < i; ++j) {
        if (!isblank(code[j])) {
          value[value_cnt++] = code[j];
        }
      }
      value[value_cnt] = '\0';
      // append identity
      append_to_vector(tokens, new_token(Identity, value));
      is_ident = false;
      // roll back
      i -= 1;
    } else if (is_number_literal && !isdigit(current_char)) {
      // stop capture number
      char *value = calloc(i - start_point + 1, sizeof(char));
      size_t value_cnt = 0;
      for (size_t j = start_point; j < i; ++j) {
        if (!isblank(code[j])) {
          value[value_cnt++] = code[j];
        }
      }
      value[value_cnt] = '\0';
      // append token to tokens
      if (is_float_literal) {
        append_to_vector(tokens, new_token(FloatLiteral, value));
        is_float_literal = false;
      } else {
        append_to_vector(tokens, new_token(IntegerLiteral, value));
      }
      is_number_literal = false;
      // roll back
      i -= 1;
    } else if (current_char == ',') {
      // append seperator
      char *value = calloc(2, sizeof(char));
      value[0] = ',';
      value[1] = '\0';
      append_to_vector(tokens, new_token(Seperator, value));
    } else if (current_char == ';') {
      // append simicolon
      char *value = calloc(2, sizeof(char));
      value[0] = ';';
      value[1] = '\0';
      append_to_vector(tokens, new_token(Simicolon, value));
    } else if (current_char == '(') {
      // append open parenthese
      char *value = calloc(2, sizeof(char));
      value[0] = '(';
      value[1] = '\0';
      append_to_vector(tokens, new_token(OpenParenthese, value));
    } else if (current_char == ')') {
      // append close parenthese
      char *value = calloc(2, sizeof(char));
      value[0] = ')';
      value[1] = '\0';
      append_to_vector(tokens, new_token(CloseParenthese, value));
    } else if (current_char == '[') {
      // append open function
      char *value = calloc(2, sizeof(char));
      value[0] = '[';
      value[1] = '\0';
      append_to_vector(tokens, new_token(OpenFunction, value));
    } else if (current_char == ']') {
      // append close function
      char *value = calloc(2, sizeof(char));
      value[0] = ']';
      value[1] = '\0';
      append_to_vector(tokens, new_token(CloseFunction, value));
    } else if (current_char == '{') {
      // append open list
      char *value = calloc(2, sizeof(char));
      value[0] = '{';
      value[1] = '\0';
      append_to_vector(tokens, new_token(OpenList, value));
    } else if (current_char == '}') {
      // append close function
      char *value = calloc(2, sizeof(char));
      value[0] = '}';
      value[1] = '\0';
      append_to_vector(tokens, new_token(CloseList, value));
    } else if (current_char == ':' && !is_bind) {
      // start capture bind sign
      is_bind = true;
    } else if (is_bind && current_char == '=') {
      // stop capture bind sign
      char *value = calloc(3, sizeof(char));
      value[0] = ':';
      value[1] = '=';
      value[2] = '\0';
      // append token
      append_to_vector(tokens, new_token(Bind, value));
      is_bind = false;
    } else if (is_bind && current_char != '=') {
      // wrong bind sign
      log_error("panic: wrong bind sign in %s", __func__);
      drop_vector(tokens);
      return NULL;
    } else if (!is_ident && !is_string_literal && !is_number_literal &&
               isdigit(current_char)) {
      // start capture number
      start_point = i;
      is_number_literal = true;
    } else if (current_char == '.' && !is_float_literal) {
      // the number is floating-point number
      is_float_literal = true;
    } else if (current_char == '.' && is_float_literal) {
      // duplicate dot
      log_error("panic: duplicate dot in %f", __func__);
      // failed to tokenize
      drop_vector(tokens);
      return NULL;
    } else if (!is_string_literal &&
               (islower(current_char) || isupper(current_char) ||
                current_char == '_') &&
               !is_ident) {
      // start capture identity
      is_ident = true;
      start_point = i;
    } else if (current_char == '"' && !is_string_literal) {
      // start capture string
      is_string_literal = true;
      start_point = i;
    } else if (is_string_literal && current_char == '"') {
      // stop string
      char *value = calloc(i - start_point + 2, sizeof(char));
      strncpy(value, code + start_point, i - start_point + 1);
      value[i - start_point + 1] = '\0';
      // append string
      append_to_vector(tokens, new_token(StringLiteral, value));
      is_string_literal = false;
    }
  }
  // append last token
  if (is_bind) {
    char *value = calloc(3, sizeof(char));
    value[0] = ':';
    value[1] = '=';
    value[2] = '\0';
    // append token
    append_to_vector(tokens, new_token(Bind, value));
    is_bind = false;
  } else if (is_number_literal) {
    char *value = calloc(strlen(code) - start_point + 1, sizeof(char));
    size_t value_cnt = 0;
    for (size_t j = start_point; j < strlen(code); ++j) {
      if (!isblank(code[j])) {
        value[value_cnt++] = code[j];
      }
    }
    value[value_cnt] = '\0';
    // append token to tokens
    if (is_float_literal) {
      append_to_vector(tokens, new_token(FloatLiteral, value));
      is_float_literal = false;
    } else {
      append_to_vector(tokens, new_token(IntegerLiteral, value));
    }
    is_number_literal = false;
  } else if (is_ident) {
    char *value = calloc(strlen(code) - start_point + 1, sizeof(char));
    size_t value_cnt = 0;
    for (size_t j = start_point; j < strlen(code); ++j) {
      if (!isblank(code[j])) {
        value[value_cnt++] = code[j];
      }
    }
    value[value_cnt] = '\0';
    // append identity
    append_to_vector(tokens, new_token(Identity, value));
    is_ident = false;
  } else if (is_string_literal) {
    char *value = calloc(strlen(code) - start_point + 1, sizeof(char));
    strncpy(value, code + start_point, strlen(code) - start_point);
    value[strlen(code) - start_point] = '\0';
    // append string
    append_to_vector(tokens, new_token(StringLiteral, value));
    is_string_literal = false;
  }
  // return: tokens
  return tokens;
}

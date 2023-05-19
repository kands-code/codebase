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

/**
 * @brief process numbers in tokenization
 *
 * @param[in] code the code to process
 * @param[in] num_bias the bias of number
 * @return the token of number
 */
Token *get_num_token(const char *code, size_t *num_bias) {
  // boundary test: null pointer
  is_null(code);
  is_null(num_bias);
  // first scan
  *num_bias = 0;
  size_t num_buffer_size = 1;
  bool is_floating_point_number = false;
  while (*(code + *num_bias) != '\0') {
    char current_char = *(code + *num_bias);
    // check current character
    if (current_char == '.' && is_floating_point_number) {
      // duplicate dot
      log_error("panic: duplicate dot in %f", __func__);
      return NULL;
    } else if (current_char == '.' && !is_floating_point_number) {
      // number is floating-point number
      is_floating_point_number = true;
      num_buffer_size += 1;
    } else if (isdigit(current_char)) {
      // normal number
      num_buffer_size += 1;
    } else {
      // meet other character, exit scan
      break;
    }
    // increase bias
    *num_bias += 1;
  }
  // construct number token
  char *token_value = calloc(num_buffer_size, sizeof(char));
  size_t value_cnt = 0;
  for (size_t i = 0; i < *num_bias && value_cnt < num_buffer_size; ++i) {
    char current_char = code[i];
    if (isblank(current_char)) {
      // blank do not matter
      continue;
    } else {
      token_value[value_cnt++] = current_char;
    }
  }
  token_value[num_buffer_size] = '\0';
  // return: number token
  return new_token(is_floating_point_number ? FloatLiteral : IntegerLiteral,
                   token_value);
}

/**
 * @brief process identity in tokenization
 *
 * @param[in] code the code to process
 * @param[in] ident_bias the bias of identity
 * @return the token of identity
 */
Token *get_ident_token(const char *code, size_t *ident_bias) {
  // boundary test: null pointer
  is_null(code);
  is_null(ident_bias);
  // scan identity
  *ident_bias = 0;
  while (*(code + *ident_bias) != '\0') {
    char current_char = *(code + *ident_bias);
    if (isdigit(current_char) || isalpha(current_char) || current_char == '_') {
      // ident := (`(A-Z)` | `(a-z)` | `(0-9)` | `_`)*
      *ident_bias += 1;
    } else {
      // meet other character
      break;
    }
  }
  // construct idenity token
  char *token_value = calloc(*ident_bias + 1, sizeof(char));
  for (size_t i = 0; i < *ident_bias; ++i) {
    token_value[i] = code[i];
  }
  token_value[*ident_bias + 1] = '\0';
  return new_token(Identity, token_value);
}

/**
 * @brief process string in tokenization
 *
 * @param[in] code the code to process
 * @param[in] ident_bias the bias of string
 * @return the token of string
 */
Token *get_str_token(const char *code, size_t *str_bias) {
  // boundary test: null pointer
  is_null(code);
  is_null(str_bias);
  // scan identity
  *str_bias = 0;
  bool is_ending = false;
  while (*(code + *str_bias) != '\0') {
    char current_char = *(code + *str_bias);
    *str_bias += 1;
    if (current_char == '"') {
      // meet ending
      is_ending = true;
      break;
    }
  }
  // check ending
  if (!is_ending) {
    // no ending string
    log_error("panic: no ending string");
    return NULL;
  }
  // construct string token
  char *token_value = calloc(*str_bias, sizeof(char));
  strncpy(token_value, code, *str_bias - 1);
  token_value[*str_bias] = '\0';
  // return: string token
  return new_token(StringLiteral, token_value);
}

Vector *tokenizer(const char *code) {
  // boundary test: null pointer
  is_null(code);
  // start tokenize
  Vector *tokens = new_vector();
  // iter all char of code
  size_t bias = 0;
  while (*(code + bias) != '\0') {
    char current_char = *(code + bias);
    // init: token
    size_t token_bias = 0;
    Token *token = NULL;
    // check current char
    if (isblank(current_char)) {
      // omit all black characters
      bias += 1;
      continue;
    } else if (isdigit(current_char)) {
      // if meet number, capture number
      token = get_num_token(code + bias, &token_bias);
    } else if (isalpha(current_char)) {
      // try capture identity
      token = get_ident_token(code + bias, &token_bias);
    } else if (current_char == '"') {
      // start to capture string
      bias += 1;
      token = get_str_token(code + bias, &token_bias);
    } else if (current_char == ':') {
      if (*(code + bias + 1) == '=') {
        // meet bind sign
        bias += 2;
        append_to_vector(tokens, new_token(Bind, str_copy(":=")));
        continue;
      }
    } else if (current_char == '(') {
      // open parenthese
      bias += 1;
      append_to_vector(tokens, new_token(OpenParenthese, str_copy("(")));
      continue;
    } else if (current_char == ')') {
      // close parenthese
      bias += 1;
      append_to_vector(tokens, new_token(CloseParenthese, str_copy(")")));
      continue;
    } else if (current_char == '[') {
      // open function
      bias += 1;
      append_to_vector(tokens, new_token(OpenFunction, str_copy("[")));
      continue;
    } else if (current_char == ']') {
      // close function
      bias += 1;
      append_to_vector(tokens, new_token(CloseFunction, str_copy("]")));
      continue;
    } else if (current_char == '{') {
      // open list
      bias += 1;
      append_to_vector(tokens, new_token(OpenList, str_copy("{")));
      continue;
    } else if (current_char == '}') {
      // close list
      bias += 1;
      append_to_vector(tokens, new_token(CloseList, str_copy("}")));
      continue;
    } else if (current_char == ',') {
      // sperator
      bias += 1;
      append_to_vector(tokens, new_token(Seperator, str_copy(",")));
      continue;
    } else if (current_char == ';') {
      // simicolon
      bias += 1;
      append_to_vector(tokens, new_token(Simicolon, str_copy(";")));
      continue;
    }
    // append token to tokens
    if (token == NULL) {
      // failed to get number token
      drop_vector(tokens);
      return NULL;
    } else {
      // append to tokens
      append_to_vector(tokens, token);
      // add token bias to bias
      bias += token_bias;
    }
  }
  // return: tokens
  return tokens;
}

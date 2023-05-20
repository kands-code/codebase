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

Token *new_token(TokenType type, char *value) {
  // init: token
  Token *token = malloc(sizeof(Token));
  token->type = type;
  if (value != NULL) {
    // copy value to token
    token->value = value;
  } else {
    // no need value
    token->value = NULL;
  }
  // return: token
  return token;
}

void drop_token(Token *token) {
  // if token is already null, do nothing
  if (token == NULL) {
    return;
  }
  if (token->value != NULL) {
    // free all of token
    free(token->value);
  }
  free(token);
}

char *get_token_type_name(TokenType type) {
  switch (type) {
  case Identity:
    return "Identity";
  case IntegerLiteral:
    return "IntegerLiteral";
  case FloatLiteral:
    return "FloatLiteral";
  case StringLiteral:
    return "StringLiteral";
  case Symbol:
    return "Symbol";
  case Type:
    return "Type";
  case TypecColon:
    return "TypeColon";
  case Bind:
    return "Bind";
  case Seperator:
    return "Seperator";
  case Simicolon:
    return "Simicolon";
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
  char *token_value = calloc(num_buffer_size + 1, sizeof(char));
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
 * @brief get other token from code string
 *
 * @param[in] code the code to process
 * @param[in] ident_bias the bias of token
 * @param[in] type the type of token
 * @return the token
 */
Token *get_other_token(const char *code, size_t *token_bias, TokenType type) {
  // boundary test: null pointer
  is_null(code);
  is_null(token_bias);
  // first scan
  if (type == Identity) {
    *token_bias = 0;
  } else if (type == Symbol || type == StringLiteral) {
    *token_bias = 1;
  }
  // for string check
  bool is_ending = false;
  while (*(code + *token_bias) != '\0') {
    char current_char = *(code + *token_bias);
    if (type == StringLiteral && current_char != '"') {
      *token_bias += 1;
    } else if (type == StringLiteral && current_char == '"') {
      is_ending = true;
      *token_bias += 1;
      break;
    } else if (type == Type && isalnum(current_char)) {
      // typ := [a-zA-Z]*
      *token_bias += 1;
    } else if ((type == Identity || type == Symbol) &&
               (isdigit(current_char) || isalpha(current_char) ||
                current_char == '_')) {
      // ident | sym := (`(A-Z)` | `(a-z)` | `(0-9)` | `_`)*
      *token_bias += 1;
    } else {
      // meet other character
      break;
    }
  }
  // check string ending
  if (type == StringLiteral && !is_ending) {
    // no ending string
    log_error("panic: no ending string");
    return NULL;
  }
  // construct token
  char *token_value;
  if (type == StringLiteral) {
    token_value = calloc(*token_bias - 1, sizeof(char));
    // remove double quote from string literal
    strncpy(token_value, code + 1, *token_bias - 2);
    token_value[*token_bias - 2] = '\0';
  } else {
    token_value = calloc(*token_bias + 1, sizeof(char));
    for (size_t i = 0; i < *token_bias; ++i) {
      token_value[i] = code[i];
    }
    token_value[*token_bias] = '\0';
  }
  return new_token(type, token_value);
}

/**
 * @brief omit comments in code
 *
 * @param[in] code the code
 * @param[in] comment_bias bias of comment
 */
void omit_comment(const char *code, size_t *comment_bias) {
  // boundary test: null pointer
  is_null(code);
  is_null(comment_bias);
  // try match end of comment
  *comment_bias = 2;
  while (*(code + *comment_bias) != '\0') {
    char current_char = *(code + *comment_bias);
    char next_char = *(code + *comment_bias + 1);
    // meet end of comment
    if (current_char == '*' && next_char != '\0' && next_char == ')') {
      *comment_bias += 2;
      return;
    }
    *comment_bias += 1;
  }
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
      if (tokens->tail_node != NULL &&
          tokens->tail_node->token->type == TypecColon) {
        // get type token
        token = get_other_token(code + bias, &token_bias, Type);
      } else {
        // get ident token
        token = get_other_token(code + bias, &token_bias, Identity);
      }
    } else if (current_char == '#') {
      // symbol start with `#`, for example, #true, #Pi
      token = get_other_token(code + bias, &token_bias, Symbol);
    } else if (current_char == '"') {
      // start to capture string
      token = get_other_token(code + bias, &token_bias, StringLiteral);
    } else if (current_char == ':') {
      if (*(code + bias + 1) == '=') {
        // meet bind sign
        bias += 2;
        append_to_vector(tokens, new_token(Bind, NULL));
      } else {
        // meet type colon
        bias += 1;
        append_to_vector(tokens, new_token(TypecColon, NULL));
      }
      continue;
    } else if (current_char == '(') {
      if (*(code + bias + 1) == '*') {
        // omit comment
        size_t comment_bias = 0;
        omit_comment(code + bias, &comment_bias);
        bias += comment_bias;
      } else {
        // open parenthese
        bias += 1;
        append_to_vector(tokens, new_token(OpenParenthese, NULL));
      }
      continue;
    } else if (current_char == ')') {
      // close parenthese
      bias += 1;
      append_to_vector(tokens, new_token(CloseParenthese, NULL));
      continue;
    } else if (current_char == '[') {
      // open function
      bias += 1;
      append_to_vector(tokens, new_token(OpenFunction, NULL));
      continue;
    } else if (current_char == ']') {
      // close function
      bias += 1;
      append_to_vector(tokens, new_token(CloseFunction, NULL));
      continue;
    } else if (current_char == '{') {
      // open list
      bias += 1;
      append_to_vector(tokens, new_token(OpenList, NULL));
      continue;
    } else if (current_char == '}') {
      // close list
      bias += 1;
      append_to_vector(tokens, new_token(CloseList, NULL));
      continue;
    } else if (current_char == ',') {
      // sperator
      bias += 1;
      append_to_vector(tokens, new_token(Seperator, NULL));
      continue;
    } else if (current_char == ';') {
      // simicolon
      bias += 1;
      append_to_vector(tokens, new_token(Simicolon, NULL));
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

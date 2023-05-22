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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//! use vector
impl_vector_type(Token);

Token *new_Token(TokenType type, char *value) {
  // init: token
  Token *tok = malloc(sizeof(Token));
  tok->type = type;
  if (value != NULL) {
    // copy value to token
    tok->value = value;
  } else {
    // no need value
    tok->value = NULL;
  }
  // return: token
  return tok;
}

Token *copy_Token(Token *tok) {
  // boundary test: null token
  is_null(tok);
  if (tok->value == NULL) {
    return new_Token(tok->type, NULL);
  } else {
    // make a copy of a string
    return new_Token(tok->type, str_copy(tok->value));
  }
}

void drop_Token(Token *tok) {
  // if token is already null, do nothing
  if (tok == NULL) {
    return;
  }
  if (tok->value != NULL) {
    // free all of token
    free(tok->value);
  }
  free(tok);
}

void show_Token(Token *tok, size_t indent) {
  indent_printf(indent, "Token Type: %s", get_TokenType(tok->type));
  if (tok->value != NULL) {
    indent_printf(indent, "Token Value: %s", tok->value);
  }
}

char *get_TokenType(TokenType type) {
  switch (type) {
  case Identifier:
    return "Identifier";
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
  case StructBind:
    return "StructBind";
  case StructGet:
    return "StructGet";
  case AddOps:
    return "AddOps";
  case SubOps:
    return "SubOps";
  case MulOps:
    return "MulOps";
  case DivOps:
    return "DivOps";
  case PowOps:
    return "PowOps";
  case Seperator:
    return "Seperator";
  case Simicolon:
    return "Simicolon";
  case ExpressionCombine:
    return "ExpressionCombine";
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
static Token *get_num_token(const char *code, size_t *num_bias) {
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
  return new_Token(is_floating_point_number ? FloatLiteral : IntegerLiteral,
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
static Token *get_other_token(const char *code, size_t *tok_bias,
                              TokenType type) {
  // boundary test: null pointer
  is_null(code);
  is_null(tok_bias);
  // first scan
  if (type == Identifier) {
    *tok_bias = 0;
  } else if (type == Symbol || type == StringLiteral) {
    *tok_bias = 1;
  }
  // for string check
  bool is_ending = false;
  while (*(code + *tok_bias) != '\0') {
    char current_char = *(code + *tok_bias);
    if (type == StringLiteral && current_char != '"') {
      *tok_bias += 1;
    } else if (type == StringLiteral && current_char == '"') {
      is_ending = true;
      *tok_bias += 1;
      break;
    } else if (type == Type && isalnum(current_char)) {
      // typ := [a-zA-Z]*
      *tok_bias += 1;
    } else if ((type == Identifier || type == Symbol) &&
               (isdigit(current_char) || isalpha(current_char) ||
                current_char == '_')) {
      // ident | sym := (`(A-Z)` | `(a-z)` | `(0-9)` | `_`)*
      *tok_bias += 1;
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
  char *tok_value;
  if (type == StringLiteral) {
    tok_value = calloc(*tok_bias - 1, sizeof(char));
    // remove double quote from string literal
    strncpy(tok_value, code + 1, *tok_bias - 2);
    tok_value[*tok_bias - 2] = '\0';
  } else {
    tok_value = calloc(*tok_bias + 1, sizeof(char));
    for (size_t i = 0; i < *tok_bias; ++i) {
      tok_value[i] = code[i];
    }
    tok_value[*tok_bias] = '\0';
  }
  return new_Token(type, tok_value);
}

/**
 * @brief omit comments in code
 *
 * @param[in] code the code
 * @param[in] comment_bias bias of comment
 */
static void omit_comment(const char *code, size_t *comment_bias) {
  // boundary test: null pointer
  is_null(code);
  is_null(comment_bias);
  // try match end of comment
  *comment_bias = 2;
  while (*(code + *comment_bias) != '\0') {
    char current_char = *(code + *comment_bias);
    char next_char = *(code + *comment_bias + 1);
    // meet end of comment
    if (current_char == '*' && next_char == ')') {
      *comment_bias += 2;
      return;
    }
    *comment_bias += 1;
  }
}

vector_Token *tokenizer(const char *code) {
  // boundary test: null pointer
  is_null(code);
  // start tokenize
  vector_Token *tokens = new_vector_Token();
  // iter all char of code
  size_t bias = 0;
  while (*(code + bias) != '\0') {
    char current_char = *(code + bias);
    char next_char = *(code + bias + 1);
    // init: token
    size_t tok_bias = 0;
    Token *tok = NULL;
    // check current char
    if (isblank(current_char) || current_char == '\n') {
      // omit all black characters
      bias += 1;
      continue;
    } else if (isdigit(current_char)) {
      // if meet number, capture number
      tok = get_num_token(code + bias, &tok_bias);
    } else if (isalpha(current_char)) {
      if (tokens->tail_node != NULL &&
          tokens->tail_node->value->type == TypecColon) {
        // get type token
        tok = get_other_token(code + bias, &tok_bias, Type);
      } else {
        // get ident token
        tok = get_other_token(code + bias, &tok_bias, Identifier);
      }
    } else if (current_char == '|') {
      // meet expression combine
      bias += 1;
      append_to_vector_Token(tokens, new_Token(ExpressionCombine, NULL));
      continue;
    } else if (current_char == '=') {
      // meet struct bind
      bias += 1;
      append_to_vector_Token(tokens, new_Token(StructBind, NULL));
      continue;
    } else if (current_char == '+') {
      // meet addition sign
      bias += 1;
      append_to_vector_Token(tokens, new_Token(AddOps, NULL));
      continue;
    } else if (current_char == '*') {
      // meet multiplication sign
      bias += 1;
      append_to_vector_Token(tokens, new_Token(MulOps, NULL));
      continue;
    } else if (current_char == '-') {
      if (next_char == '>') {
        // meet strcut get
        bias += 2;
        append_to_vector_Token(tokens, new_Token(StructGet, NULL));
      } else {
        // meet substraction sign
        bias += 1;
        append_to_vector_Token(tokens, new_Token(SubOps, NULL));
      }
      continue;
    } else if (current_char == '/') {
      // meet division sign
      bias += 1;
      append_to_vector_Token(tokens, new_Token(DivOps, NULL));
      continue;
    } else if (current_char == '^') {
      // meet power sign
      bias += 1;
      append_to_vector_Token(tokens, new_Token(PowOps, NULL));
      continue;
    } else if (current_char == '#') {
      // symbol start with `#`, for example, #true, #Pi
      tok = get_other_token(code + bias, &tok_bias, Symbol);
    } else if (current_char == '"') {
      // start to capture string
      tok = get_other_token(code + bias, &tok_bias, StringLiteral);
    } else if (current_char == ':') {
      if (next_char == '=') {
        // meet bind sign
        bias += 2;
        append_to_vector_Token(tokens, new_Token(Bind, NULL));
      } else {
        // meet type colon
        bias += 1;
        append_to_vector_Token(tokens, new_Token(TypecColon, NULL));
      }
      continue;
    } else if (current_char == '(') {
      // omit comment
      if (*(code + bias + 1) == '*') {
        size_t comment_bias = 0;
        omit_comment(code + bias, &comment_bias);
        bias += comment_bias;
      } else {
        bias += 1;
        append_to_vector_Token(tokens, new_Token(OpenParenthese, NULL));
      }
      continue;
    } else if (current_char == ')') {
      bias += 1;
      append_to_vector_Token(tokens, new_Token(CloseParenthese, NULL));
      continue;
    } else if (current_char == '[') {
      // open function
      bias += 1;
      append_to_vector_Token(tokens, new_Token(OpenFunction, NULL));
      continue;
    } else if (current_char == ']') {
      // close function
      bias += 1;
      append_to_vector_Token(tokens, new_Token(CloseFunction, NULL));
      continue;
    } else if (current_char == '{') {
      // open list
      bias += 1;
      append_to_vector_Token(tokens, new_Token(OpenList, NULL));
      continue;
    } else if (current_char == '}') {
      // close list
      bias += 1;
      append_to_vector_Token(tokens, new_Token(CloseList, NULL));
      continue;
    } else if (current_char == ',') {
      // sperator
      bias += 1;
      append_to_vector_Token(tokens, new_Token(Seperator, NULL));
      continue;
    } else if (current_char == ';') {
      // simicolon
      bias += 1;
      append_to_vector_Token(tokens, new_Token(Simicolon, NULL));
      continue;
    }
    // append token to tokens
    if (tok == NULL) {
      // failed to get number token
      drop_vector_Token(tokens);
      return NULL;
    } else {
      // append to tokens
      append_to_vector_Token(tokens, tok);
      // add token bias to bias
      bias += tok_bias;
    }
  }
  // return: tokens
  return tokens;
}

/**
 * @file ksl/ksl_ast.h
 * @brief AST of KSL
 */

#pragma once
#ifndef __KSL_KSL_AST_H__
#define __KSL_KSL_AST_H__

// includes

// standard include
#include <stddef.h>

// types

/**
 * @brief value type in KSL
 */
typedef enum KSLValType {
  INT, ///< integer number
  FLT, ///< floating-point number
  STR, ///< string
} KSLValType;

/**
 * @brief expression node type of KSL
 */
typedef struct KSLExpNode {
  size_t ident;                ///< identity of expression
  size_t branch_number;        ///< number of branches the expression have
  void *exp_value;             ///< the value of the expression
  KSLValType val_type;         ///< type of value
  struct KSLExpNode *branches; ///< all branches of expression
} KSLExpNode;

// functions

// ! TODO

extern void ksl_setval(const char *ident, void *value);

extern void ksl_setfun(const char *ident, const char *pattern);

#endif

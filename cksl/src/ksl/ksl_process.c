/**
 * @file ksl/ksl_process.c
 * @brief some process steps of KSL
 */

// includes

// self incldue
#include "ksl/ksl.h"
#include "ksl/ksl_ast.h"
#include "ksl/utils.h"

// globals

// ! TODO function table and variable table

// functions: process

KSLExpNode *ksl_eval(KSLExpNode *nodes) {
  if (nodes == NULL) {
    log_error("panic: null pointer error in eval");
    return NULL;
  }
  if (nodes->exp_value != NULL) {
    return nodes;
  }

  return NULL;
}

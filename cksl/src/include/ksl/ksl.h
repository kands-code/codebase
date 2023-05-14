/**
 * @file ksl/ksl.h
 * @brief implementation of KSL
 */

#pragma once
#ifndef __KSL_KSL_H__
#define __KSL_KSL_H__

// includes

// self include
#include "ksl/ksl_ast.h"
// standard include
#include <stddef.h>

// constants

/**
 * \def MAX_STRING_BUFFER_SIZE
 *
 * maximun size of string buffer
 */
#define MAX_STRING_BUFFER_SIZE 1024

// functions: input

/**
 * get input by REPL
 *
 * @param[in] prompt the prompt to show
 */
extern void ksl_repl(const char *prompt);

/**
 * show help of REPL
 */
extern void ksl_repl_help();

// functions: parser

/**
 * @brief interpret the content
 *
 * @param[in] content the content to interpret
 * @return the result of interpretation
 */
extern char *ksl_interpreter(const char *content);

// ! TODO

extern KSLExpNode *ksl_parser(const char *content);

extern KSLExpNode *ksl_parser_number(const char *content, size_t *bias);

// functions: process

extern KSLExpNode *ksl_eval(KSLExpNode *nodes);

#endif

/**
 * @file ksl/token.h
 * @brief the paser of KSL
 */

#pragma once
#ifndef __KSL_PARSER_H__
#define __KSL_PARSER_H__

// include

#include "ksl/vector.h"
#include <stddef.h>

// types

typedef struct Expression {

} Expression;

//! use vector
new_vector_type(Expression);

// functions: parser

extern void show_Expression(Expression expr, size_t indent);

#endif
/// @file: matrix/utils.h
/// @info: some utilities of matrix library

#pragma once
#ifndef __MATRIX_UTILS_H__
#define __MATRIX_UTILS_H__

// include

#include <complex.h>
#include <stdbool.h>

// macros

/// @macro: MAX (x, y)
/// @info: return the maximum value between x and y
#define MAX(x, y) (x) > (y) ? (x) : (y)

/// @macro: MIN (x, y)
/// @info: return the minimum value between x and y
#define MIN(x, y) (x) > (y) ? (y) : (x)

/// @macro: IS_ODD (x)
/// @info: check a number whether an odd number
#define IS_ODD(x) ((x)&0x01)

/// @macro: IS_EVEN (x)
/// @info: check a number whether an even number
#define IS_EVEN(x) (!IS_ODD(x))

// functions: log

/// @function: log_info (const char *, ...)
///                     -> stderr
/// @info: print info logs
extern void log_info(const char *message, ...);

/// @function: log_warn (const char *, ...)
///                     -> stderr
/// @info: print warning logs
extern void log_warn(const char *message, ...);

/// @function: log_error (const char *, ...)
///                      -> stderr
/// @info: print error logs
extern void log_error(const char *message, ...);

/// @function: is_complex_zero (complex float)
///                            -> bool
/// @param: <value> the value to check
/// @return: if the value is zero, return true, or false
/// @info: check whether a value is zero
extern bool is_complex_zero(complex float value);

#endif
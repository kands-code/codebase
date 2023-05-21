/**
 * @file matrix/utils.h
 * @brief some utilities of matrix library
 */

#pragma once
#ifndef __MATRIX_UTILS_H__
#define __MATRIX_UTILS_H__

// include

#include <complex.h>
#include <stdbool.h>

// macros

/**
 * \def MAX (x, y)
 *
 * get the maximum value between \p x and \p y
 */
#define MAX(x, y) (x) > (y) ? (x) : (y)

/**
 * \def MIN (x, y)
 *
 * get the minimum value between \p x and \p y
 */
#define MIN(x, y) (x) > (y) ? (y) : (x)

/**
 * \def IS_ODD(x)
 *
 * check \p x whether an odd number
 */
#define IS_ODD(x) ((x)&0x01)

/**
 * \def IS_EVEN (x)
 *
 * check \p x whether an even number
 */
#define IS_EVEN(x) (!IS_ODD(x))

// functions: log

/**
 * @brief print info logs
 *
 * @param[in] message the message to log
 * @param[in] ... the params of the message
 */
extern void log_info(const char *message, ...);

/**
 * @brief print warning logs
 *
 * @param[in] message the message to log
 * @param[in] ... the params of the message
 */
extern void log_warn(const char *message, ...);

/**
 * @brief print error logs
 *
 * @param[in] message the message to log
 * @param[in] ... the params of the message
 */
extern void log_error(const char *message, ...);

/**
 * @brief check whether a value is zero
 *
 * @param[in] value the value to check
 * @return true if \p value is zero, or false
 */
extern bool is_complex_zero(complex float value);

#endif

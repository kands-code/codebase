/**
 * @file ksl/utils.h
 * @brief some utilities of KSL
 */

#pragma once
#ifndef __KSL_UTILS_H__
#define __KSL_UTILS_H__

// includes

// standard include
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

#endif

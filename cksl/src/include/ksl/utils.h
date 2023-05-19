/**
 * @file ksl/utils.h
 * @brief some utilities of KSL
 */

#pragma once
#ifndef __KSL_UTILS_H__
#define __KSL_UTILS_H__

// include

#include <stdbool.h>

// macros

/**
 * \def static_cast (type, value)
 *
 * cast a pointer to a certain type pointer
 */
#define static_cast(type, value) ((type) *)(value)

/**
 * \def is_null (object)
 *
 * check an object is a NULL or not
 */
#define is_null(object)                                                        \
  do {                                                                         \
    if (object == NULL) {                                                      \
      log_error("panic: null pointer error in %s", __func__);                  \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

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

// functions: utils

/**
 * @brief get a copy of a string
 *
 * @param[in] origin_string the original string
 * @return the copy of \p original_string
 */
extern char *str_copy(const char *origin_string);

#endif
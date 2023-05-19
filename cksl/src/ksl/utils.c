/**
 * @file ksl/utils.c
 * @brief some utilities for KSL
 */

// include

#include "ksl/utils.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// constants: ANSI color control sequences

/**
 * \def ANSI_COLOR_RED
 *
 * color of red
 */
static const char *ANSI_COLOR_RED = "\x1b[31m";

/**
 * \def ANSI_COLOR_YELLOW
 *
 * color of yellow
 */
static const char *ANSI_COLOR_YELLOW = "\x1b[33m";

/**
 * \def ANSI_COLOR_BLUE
 *
 * color of blue
 */
static const char *ANSI_COLOR_BLUE = "\x1b[34m";

/**
 * \def ANSI_COLOR_RESET
 *
 * reset color
 */
static const char *ANSI_COLOR_RESET = "\x1b[0m";

// functions: log

void log_info(const char *message, ...) {
  // init: varying list of arguments
  va_list args;
  // start read varying list
  va_start(args, message);
  // prepare info message
  char info_message[256];
  vsprintf(info_message, message, args);
  // print: info message to stderr
  fprintf(stderr, "%s%s%s\n", ANSI_COLOR_BLUE, info_message, ANSI_COLOR_RESET);
  // end: close varing list
  va_end(args);
}

void log_warn(const char *message, ...) {
  // init: varying list of arguments
  va_list args;
  // start read varying list
  va_start(args, message);
  // prepare warning message
  char warn_message[256];
  vsprintf(warn_message, message, args);
  // print: warning message to stderr
  fprintf(stderr, "%s%s%s\n", ANSI_COLOR_YELLOW, warn_message,
          ANSI_COLOR_RESET);
  // end: close varing list
  va_end(args);
}

void log_error(const char *message, ...) {
  // init: varying list of arguments
  va_list args;
  // start read varying list
  va_start(args, message);
  // prepare error message
  char error_message[256];
  vsprintf(error_message, message, args);
  // print: error message to stderr
  fprintf(stderr, "%s%s%s\n", ANSI_COLOR_RED, error_message, ANSI_COLOR_RESET);
  // end: close varing list
  va_end(args);
}

// functions: utils

char *str_copy(const char *origin_string) {
  // a short cut of copy a string
  size_t origin_string_length = strlen(origin_string);
  char *copy_string = calloc(origin_string_length + 1, sizeof(char));
  strncpy(copy_string, origin_string, origin_string_length);
  copy_string[origin_string_length + 1] = '\0';
  return copy_string;
}

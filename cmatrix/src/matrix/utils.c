#include "matrix/utils.h"
#include <stdarg.h>
#include <stdio.h>

// constants: ANSI color control sequences

/// @constant: color of red
static const char *ANSI_COLOR_RED = "\x1b[31m";

/// @constant: color of yellow
static const char *ANSI_COLOR_YELLOW = "\x1b[33m";

/// @constant: color of blue
static const char *ANSI_COLOR_BLUE = "\x1b[34m";

/// @constant: reset color
static const char *ANSI_COLOR_RESET = "\x1b[0m";

// functions: log

/// @function: log_info (const char *, ...)
///                     -> stderr
/// @info: print info logs
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

/// @function: log_warn (const char *, ...)
///                     -> stderr
/// @info: print warning logs
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

/// @function: log_error (const char *, ...)
///                      -> stderr
/// @info: print error logs
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

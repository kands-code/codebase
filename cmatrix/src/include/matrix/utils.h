#pragma once
#ifndef __MATRIX_UTILS_H__
#define __MATRIX_UTILS_H__

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

#endif
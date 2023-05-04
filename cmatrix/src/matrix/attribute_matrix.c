/// @file: matrix/attribute_matrix.c
/// @info: get attributes of matrices

// include

#include "matrix/matrix.h"
#include "matrix/utils.h"
#include <complex.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// functions: attribute

/// @function: get_matrix_val (const MatrixT *, uint8_t,
///                            uint8_t)
///                           -> complex float
/// @param: <matrix> the matrix to use
/// @param: <row> the row position of value
/// @param: <col> the column position of value
/// @return: the value at (row, col) of the matrix
/// @info: get value at the specific position of a matrix
complex float get_matrix_val(const MatrixT *matrix, uint8_t row, uint8_t col) {
  // boundary test: null pointer
  if (matrix == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // boundary test: access position
  if (row == 0 || col == 0 || row > matrix->size[0] || col > matrix->size[1]) {
    log_error("panic: %s out of boundary (%u, %u)", __func__, row, col);
    exit(EXIT_FAILURE);
  }
  // get: value at specific position
  return matrix->data[(row - 1) * matrix->size[1] + col - 1];
}

/// @function: set_matrix_val: (MatrixT *, uint8_t, uint8_t,
///                             complex float)
///                            -> unit
/// @param: <matrix> the matrix to modify
/// @param: <row> the row position of value
/// @param: <col> the column position of value
/// @param: <val> the value to use
/// @return: the value at (row, col) of the matrix
/// @info: set the value at the specific position of a matrix
void set_matrix_val(MatrixT *matrix, uint8_t row, uint8_t col,
                    complex float val) {
  // boundary test: null pointer
  if (matrix == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // boundary test: access position
  if (row == 0 || col == 0 || row > matrix->size[0] || col > matrix->size[1]) {
    log_error("panic: %s out of boundary (%u, %u)[%.3f%+.3f]", __func__, row,
              col, crealf(val), cimagf(val));
    exit(EXIT_FAILURE);
  }
  // set: value at specific position
  matrix->data[(row - 1) * matrix->size[1] + col - 1] = val;
}

/// @function: get_matrix_row (const MatrixT *, uint8_t)
///                           -> MatrixT *
/// @param: <matrix> the matrix to use
/// @param: <row> the row to get
/// @return: the row vector of the matrix
/// @info: get the row of a matrix
MatrixT *get_matrix_row(const MatrixT *matrix, uint8_t row) {
  // boundary test: null pointer
  if (matrix == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // boundary test: access position
  if (row == 0) {
    log_error("panic: %s out of boundary (row: %u)", __func__, row);
    exit(EXIT_FAILURE);
  }
  // init: row vector
  MatrixT *row_vector = new_matrix(1, matrix->size[1]);
  // copy the row
  for (size_t i = 1; i <= matrix->size[1]; ++i) {
    set_matrix_val(row_vector, 1, i, get_matrix_val(matrix, row, i));
  }
  // return: row vector
  return row_vector;
}

/// @function: get_matrix_row (const MatrixT *, uint8_t)
///                           -> MatrixT *
/// @param: <matrix> the matrix to use
/// @param: <col> the column to get
/// @return: the column vector of the matrix
/// @info: get the column of a matrix
MatrixT *get_matrix_col(const MatrixT *matrix, uint8_t col) {
  // boundary test: null pointer
  if (matrix == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // boundary test: access position
  if (col == 0) {
    log_error("panic: %s out of boundary (col: %u)", __func__, col);
    exit(EXIT_FAILURE);
  }
  // init: col vector
  MatrixT *col_vector = new_matrix(matrix->size[0], 1);
  // copy the column
  for (size_t i = 1; i <= matrix->size[0]; ++i) {
    set_matrix_val(col_vector, i, 1, get_matrix_val(matrix, i, col));
  }
  // return: row vector
  return col_vector;
}

/// @function: get_matrix_trace (const MatrixT *)
///                             -> complex float
/// @param: <matrix> the matrix to use
/// @return: the trace of the matrix
/// @info: get the trace of matrix
complex float get_matrix_trace(const MatrixT *matrix) {
  // boundary test: null pointer
  if (matrix == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // boundary tes: square matrix
  if (matrix->size[0] != matrix->size[1]) {
    log_error("panic: matrix must be squared at %s with size (%u, %u)",
              __func__, matrix->size[0], matrix->size[1]);
    exit(EXIT_FAILURE);
  }
  // calculate the trace of matrix
  complex float trace_value = new_complex(0.0f, 0.0f);
  for (size_t i = 1; i <= matrix->size[0]; ++i) {
    trace_value += get_matrix_val(matrix, i, i);
  }
  // return: the trace
  return trace_value;
}

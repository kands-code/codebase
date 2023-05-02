#include "matrix/matrix.h"
#include "matrix/utils.h"
#include <complex.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// functions: init

/// @function: new_complex (float, float)
///                        -> complex float
/// @param: <real> the real part of the complex number
/// @param: <imag> the imagine part of  the complex number
/// @return: the complex number
/// @info: construct a complex number with float type
complex float new_complex(float x, float y) {
  // return: complex number
  return __builtin_complex(x, y);
}

/// @function: new_matrix (uint8_t, uint8_t)
///                       -> MatrixT *
/// @param: <row> the row size of matrix
/// @param: <col> the column size of matrix
/// @return: the matrix with size (row, col) filled with zero
/// @info: construct an zero matrix with size (row, col)
MatrixT *new_matrix(uint8_t row, uint8_t col) {
  // boundary test: size
  if (row == 0 || col == 0) {
    log_error("panic: size must bigger than 0");
    exit(EXIT_FAILURE);
  }
  // malloc: matrix type
  MatrixT *matrix = malloc(sizeof(MatrixT));
  // assign: size
  matrix->size[0] = row;
  matrix->size[1] = col;
  // malloc: matrix data
  matrix->data = calloc(row * col, sizeof(complex float));
  // assign: set data to zeros
  for (size_t i = 0; i < matrix->size[0] * matrix->size[1]; ++i) {
    matrix->data[i] = new_complex(0.0f, 0.0f);
  }
  // return: zero matrix
  return matrix;
}

/// @function: new_matrix_from_array (uint8_t, uint8_t,
///                                   MatrixOrientation,
///                                   const complexfloat *)
///                                  -> MatrixT *
/// @param: <row> the row size of matrix
/// @param: <col> the column size of matrix
/// @param: <orientation> the orientation which is used
/// @param: <array> the array to use (must longer or equal than row * col)
/// @return: the matrix with size (row, col) filled by the array
/// @info: construct an zero matrix with size (row, col)
MatrixT *new_matrix_from_array(uint8_t row, uint8_t col,
                               MatrixOrientation orientation,
                               const complex float *array) {
  MatrixT *matrix = new_matrix(row, col);
  if (orientation == ROW) {
    for (size_t i = 0; i < matrix->size[0] * matrix->size[1]; ++i) {
      matrix->data[i] = array[i];
    }
  } else if (orientation == COLUMN) {
    MatrixT *row_matrix = new_matrix_from_array(row, col, ROW, array);
    MatrixT *col_matrix = transpose_matrix(row_matrix);
    drop_matrix(row_matrix);
    drop_matrix(matrix);
    matrix = col_matrix;
  } else {
    log_error("panic: illegal argument of orientation: %d", orientation);
    exit(EXIT_FAILURE);
  }
  return matrix;
}

/// @function: copy_matrix (MatrixT *)
///                        -> MatrixT *
/// @param: <matrix> the original matrix
/// @return: the copy of the original matrix
/// @info: copy a matrix
MatrixT *copy_matrix(const MatrixT *matrix) {
  // boundary test: null pointer
  if (matrix == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // init: copied matrix
  MatrixT *copied_matrix = new_matrix(matrix->size[0], matrix->size[1]);
  // copy all data from original matrix
  for (size_t i = 0; i < copied_matrix->size[0] * copied_matrix->size[1]; ++i) {
    copied_matrix->data[i] = matrix->data[i];
  }
  // return: copied matrix
  return copied_matrix;
}

/// @function: drop_matrix (MatrixT *)
///                        -> unit
/// @param: <matrix> the matrix to drop
/// @info: delete a matrix
extern void drop_matrix(MatrixT *matrix) {
  // if matrix is null, it's fine
  if (matrix == NULL) {
    return;
  }
  // data of matrix maybe null
  if (matrix->data != NULL) {
    free(matrix->data);
  }
  // free the matrix
  free(matrix);
}

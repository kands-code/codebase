/**
 * @file matrix/attribute_matrix.c
 * @brief get attributes of matrices
 */

// include

#include "matrix/matrix.h"
#include "matrix/matrix_ext.h"
#include "matrix/utils.h"
#include <complex.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// functions: attribute

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

bool is_upper_triangle(const MatrixT *matrix) {
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
  // start check
  for (uint8_t row = 2; row <= matrix->size[0]; ++row) {
    for (uint8_t col = 1; col < row; ++col) {
      // if any value under diagonal (include) is not zero
      // return false
      complex float val = get_matrix_val(matrix, row, col);
      if (fabsf(crealf(val)) > FLT_MIN || fabsf(cimagf(val)) > FLT_MIN) {
        return false;
      }
    }
  }
  // pass check
  return true;
}

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

complex float get_matrix_frobenius_norm(const MatrixT *matrix) {
  // boundary test: null pointer
  if (matrix == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // fnorm(A) = sqrt(sum(A^2))
  complex float frobenius_norm = new_complex(0.0f, 0.0f);
  for (uint16_t i = 0; i < matrix->size[0] * matrix->size[1]; ++i) {
    frobenius_norm += matrix->data[i] * matrix->data[i];
  }
  // return: Frobenius Norm
  return csqrtf(frobenius_norm);
}

uint8_t get_matrix_rank(const MatrixT *matrix) {
  // simplify the matrix
  MatrixT *simplest_matrix = simplify_matrix(matrix);
  // get basic infomation of the matrix
  uint8_t matrix_diagonal_size =
      MIN(simplest_matrix->size[0], simplest_matrix->size[1]);
  // start to count
  uint8_t offset = 0;
  uint8_t iter = 1;
  while (iter <= matrix_diagonal_size &&
         iter + offset <= simplest_matrix->size[1]) {
    complex float pivot_value =
        get_matrix_val(simplest_matrix, iter, iter + offset);
    if (is_complex_zero(pivot_value)) {
      offset++;
      continue;
    }
    iter++;
  }
  drop_matrix(simplest_matrix);
  return iter - 1;
}

MatrixT *get_submatrix(const MatrixT *matrix, uint8_t row, uint8_t col) {
  // boundary test: null pointer
  if (matrix == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // boundary test: matrix size
  if (matrix->size[0] == 1 || matrix->size[1] == 1) {
    log_error("panic: matrix size (%hhu, %hhu) is too small", matrix->size[0],
              matrix->size[1]);
    exit(EXIT_FAILURE);
  }
  // boundary test: access position
  if (row == 0 || col == 0 || row > matrix->size[0] || col > matrix->size[1]) {
    log_error("panic: %s out of boundary (%u, %u)", __func__, row, col);
    exit(EXIT_FAILURE);
  }
  // init: submatrix
  MatrixT *submatrix = new_matrix(matrix->size[0] - 1, matrix->size[1] - 1);
  // start copy
  uint8_t row_omit = 0;
  for (uint8_t i = 1; i <= submatrix->size[0]; ++i) {
    if (i == row) {
      row_omit = 1;
    }
    uint8_t col_omit = 0;
    for (uint8_t j = 1; j <= submatrix->size[1]; ++j) {
      if (j == col) {
        col_omit = 1;
      }
      set_matrix_val(submatrix, i, j,
                     get_matrix_val(matrix, i + row_omit, j + col_omit));
    }
  }
  // return: submatrix
  return submatrix;
}

complex float get_matrix_cofactor(const MatrixT *matrix, uint8_t row,
                                  uint8_t col) {
  // cofactor(r, c) = det(sub(A, r, c))
  MatrixT *submatrix = get_submatrix(matrix, row, col);
  complex float cofacter = get_matrix_determinant(submatrix);
  drop_matrix(submatrix);
  return cofacter;
}

complex float get_matrix_algebraic_cofactor(const MatrixT *matrix, uint8_t row,
                                            uint8_t col) {
  complex float cofacter = get_matrix_cofactor(matrix, row, col);
  return IS_ODD(row + col) ? (-cofacter) : cofacter;
}

complex float get_matrix_determinant(const MatrixT *matrix) {
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
  complex float determinant = new_complex(1.0f, 0.0f);
  MatrixT **lu_res = upper_triangularize_matrix(matrix);
  for (uint8_t i = 1; i <= lu_res[1]->size[0]; ++i) {
    determinant *= get_matrix_val(lu_res[1], i, i);
  }
  drop_matrices(lu_res, 2);
  return determinant;
}

MatrixT *get_adjoint_matrix(const MatrixT *matrix) {
  // init: adjoint matrix
  MatrixT *adjoint_matrix = new_matrix(matrix->size[1], matrix->size[0]);
  // fill adjoint matrix with algebraic cofactor by transposition
  for (uint8_t row = 1; row <= adjoint_matrix->size[0]; ++row) {
    for (uint8_t col = 1; col <= adjoint_matrix->size[1]; ++col) {
      set_matrix_val(adjoint_matrix, row, col,
                     get_matrix_algebraic_cofactor(matrix, col, row));
    }
  }
  // return: adjoint matrix
  return adjoint_matrix;
}

MatrixT *get_inverse_matrix(const MatrixT *matrix) {
  // inv(A) = adj(A) / det(A)
  complex float determinant = get_matrix_determinant(matrix);
  // boundary test: determinant can not be zero
  if (is_complex_zero(determinant)) {
    log_error("panic: the matrix");
    show_matrix(matrix);
    log_error("isn't inversable");
    exit(EXIT_FAILURE);
  }
  MatrixT *adjoint_matrix = get_adjoint_matrix(matrix);
  MatrixT *inverse_matrix = scalar_mul_matrix(1 / determinant, adjoint_matrix);
  drop_matrix(adjoint_matrix);
  return inverse_matrix;
}

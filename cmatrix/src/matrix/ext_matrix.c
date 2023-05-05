/// @file: matrix/ext_matrix.c
/// @info: extension function of matrix library

// include

#include "matrix/matrix.h"
#include "matrix/matrix_ext.h"
#include "matrix/utils.h"
#include <complex.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// function: extensions

/// @function: upper_triangularize_matrix (const MatrixT *)
///                                    -> MatrixT **
/// @param: <matrix> the matrix to decomposition
/// @return: the result of LU decomposition of matrix
/// @info: get the INVERSE Left matrix and Right matrix
MatrixT **upper_triangularize_matrix(const MatrixT *matrix) {
  // boundary test: null pointer
  if (matrix == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // get basic info of matrix
  uint8_t matrix_row = matrix->size[0];
  uint8_t matrix_col = matrix->size[1];
  // get matrix diagonal size
  uint8_t matrix_diagonal_size = MIN(matrix_row, matrix_col);
  uint8_t offset = 0;
  // init: LU decomposition result
  MatrixT **lu_result = calloc(2, sizeof(MatrixT *));
  lu_result[0] = new_identity_matrix(matrix_row, matrix_row);
  lu_result[1] = copy_matrix(matrix);
  size_t change_cnt = 0;
  // strat: elimilation
  for (uint8_t iter = 1;
       iter < matrix_diagonal_size && iter + offset <= matrix_col;) {
    // check: pivot can not be zero
    if (is_complex_zero(get_matrix_val(lu_result[1], iter, iter + offset))) {
      // do row exchange
      for (uint8_t r = iter + 1; r <= matrix_row; ++r) {
        // find a non-zero value
        if (!is_complex_zero(get_matrix_val(lu_result[1], r, iter))) {
          // construct exchange matrix
          MatrixT *exchange_matrix =
              new_identity_matrix(matrix_row, matrix_row);
          set_matrix_val(exchange_matrix, iter, iter, new_complex(0.0f, 0.0f));
          set_matrix_val(exchange_matrix, r, r, new_complex(0.0f, 0.0f));
          set_matrix_val(exchange_matrix, iter, r, new_complex(1.0f, 0.0f));
          set_matrix_val(exchange_matrix, r, iter, new_complex(1.0f, 0.0f));
          // apply changes
          MatrixT *changed_matrix = mul_matrix(exchange_matrix, lu_result[1]);
          drop_matrix(lu_result[1]);
          lu_result[1] = changed_matrix;
          change_cnt++;
          // store changes
          MatrixT *new_left_matrix = mul_matrix(exchange_matrix, lu_result[0]);
          drop_matrix(lu_result[0]);
          lu_result[0] = new_left_matrix;
          // skip loop
          drop_matrix(exchange_matrix);
          break;
        }
      }
      // re-check pivot
      if (is_complex_zero(get_matrix_val(lu_result[1], iter, iter + offset))) {
        offset++;
        continue;
      }
    }
    // do elimilation
    complex float pivot_value =
        get_matrix_val(lu_result[1], iter, iter + offset);
    for (uint8_t elim_row = iter + 1; elim_row <= matrix_row; ++elim_row) {
      complex float elim_value =
          get_matrix_val(lu_result[1], elim_row, iter + offset);
      if (is_complex_zero(elim_value)) {
        continue;
      }
      // construct elimilation matrix
      MatrixT *elim_matrix = new_identity_matrix(matrix_row, matrix_row);
      set_matrix_val(elim_matrix, elim_row, iter, -elim_value / pivot_value);
      // apply elimilation
      MatrixT *new_right_matrix = mul_matrix(elim_matrix, lu_result[1]);
      drop_matrix(lu_result[1]);
      lu_result[1] = new_right_matrix;
      // store elimilation
      MatrixT *new_left_matrix = mul_matrix(elim_matrix, lu_result[0]);
      drop_matrix(lu_result[0]);
      lu_result[0] = new_left_matrix;
      drop_matrix(elim_matrix);
    }
    // keep on iter
    iter++;
  }
  // check change times
  if (IS_ODD(change_cnt)) {
    MatrixT *final_right_matrix = scalar_mul_matrix(-1, lu_result[1]);
    drop_matrix(lu_result[1]);
    lu_result[1] = final_right_matrix;
  }
  // return: result of LU decomposition
  return lu_result;
}

/// @function: decomposition_matrix_lu (const MatrixT *)
///                                    -> MatrixT **
/// @param: <matrix> the matrix to decomposition
/// @return: the result of LU decomposition of matrix
/// @info: get the Left matrix and Right matrix
MatrixT **decomposition_matrix_lu(const MatrixT *matrix) {
  MatrixT **lu_result = upper_triangularize_matrix(matrix);
  MatrixT *left_matrix = get_inverse_matrix(lu_result[0]);
  drop_matrix(lu_result[0]);
  lu_result[0] = left_matrix;
  return lu_result;
}

/// @function: simplify_matrix (const MatrixT *)
///                            -> MatrixT *
/// @param: <matrix> the matrix to simplify
/// @return: the simplified matrix
/// @info: get the simplest form of the matrix
MatrixT *simplify_matrix(const MatrixT *matrix) {
  MatrixT **lu_result = upper_triangularize_matrix(matrix);
  MatrixT *simplest_matrix = copy_matrix(lu_result[1]);
  drop_matrices(lu_result, 2);
  // get basic info of matrix
  uint8_t matrix_row = simplest_matrix->size[0];
  uint8_t matrix_col = simplest_matrix->size[1];
  // get matrix diagonal size
  uint8_t matrix_diagonal_size = MIN(matrix_row, matrix_col);
  uint8_t offset = 0;
  for (uint8_t iter = 1;
       iter <= matrix_diagonal_size && iter + offset <= matrix_col;) {
    complex float pivot_value =
        get_matrix_val(simplest_matrix, iter, iter + offset);
    if (is_complex_zero(pivot_value)) {
      offset++;
      continue;
    }
    // make pivot to 1+0I
    if (!is_complex_zero(pivot_value - new_complex(1.0f, 0.0f))) {
      MatrixT *unitify_matrix = new_identity_matrix(matrix_row, matrix_row);
      set_matrix_val(unitify_matrix, iter, iter,
                     new_complex(1.0f, 0.0f) / pivot_value);
      MatrixT *temp_matrix = mul_matrix(unitify_matrix, simplest_matrix);
      drop_matrix(unitify_matrix);
      drop_matrix(simplest_matrix);
      simplest_matrix = temp_matrix;
    }
    // elimilation
    for (uint8_t row_back = iter - 1; row_back > 0; --row_back) {
      complex float elim_value =
          get_matrix_val(simplest_matrix, row_back, iter + offset);
      if (is_complex_zero(elim_value)) {
        continue;
      }
      MatrixT *elim_matrix = new_identity_matrix(matrix_row, matrix_row);
      set_matrix_val(elim_matrix, row_back, iter, -elim_value);
      MatrixT *new_simplest_matrix = mul_matrix(elim_matrix, simplest_matrix);
      drop_matrix(elim_matrix);
      drop_matrix(simplest_matrix);
      simplest_matrix = new_simplest_matrix;
    }
    iter++;
  }
  return simplest_matrix;
}

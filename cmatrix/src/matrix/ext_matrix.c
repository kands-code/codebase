/// @file: matrix/ext_matrix.c
/// @info: extension function of matrix library

// include

#include "matrix/matrix.h"
#include "matrix/matrix_ext.h"
#include "matrix/utils.h"
#include <complex.h>
#include <float.h>
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

/// @function: decomposition_matrix_qr (const MatrixT *)
///                                    -> MatrixT **
/// @param: <matrix> the matrix to decomposition
/// @return: the result of QR decomposition of matrix
/// @info: apply QR decomposition on the matrix
MatrixT **decomposition_matrix_qr(const MatrixT *matrix) {
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
  uint8_t size = matrix->size[0];
  // init: result of QR decomposition
  MatrixT **qr_result = calloc(2, sizeof(MatrixT *));
  qr_result[0] = new_identity_matrix(size, size);
  qr_result[1] = copy_matrix(matrix);
  // construct P_k
  for (uint8_t iter = 1; iter < size; ++iter) {
    // get the `iter` column of matrix (P^m A)
    // m from 0 to (n - 1)
    MatrixT *col_iter = get_matrix_col(qr_result[1], iter);
    // normalize the column vector
    complex float norm = get_matrix_frobenius_norm(col_iter);
    MatrixT *norm_col_iter = scalar_mul_matrix(1 / norm, col_iter);
    drop_matrix(col_iter);
    // calculate the value of d_rare = sqrt(sum(column_iter ^ 2))
    complex float d_rare = new_complex(0.0f, 0.0f);
    for (uint8_t j = iter; j <= size; ++j) {
      complex float val_at_j = get_matrix_val(norm_col_iter, j, 1);
      d_rare += val_at_j * val_at_j;
    }
    d_rare = csqrtf(d_rare);
    // use the `iter`-th value to get the sign of D
    // if value > 0, then get negative, or positive
    complex float d_iter = get_matrix_val(norm_col_iter, iter, 1);
    if (crealf(d_iter) > FLT_MIN) {
      d_rare = -d_rare;
    }
    // construct vector V_iter
    MatrixT *V_iter = new_matrix(size, 1);
    // v_j = 0, j in [0, iter - 1]
    // v_iter = sqrt(1/2 (1 - d_iter / d_rare))
    complex float v_iter = csqrtf(0.5 * (1 - d_iter / d_rare));
    set_matrix_val(V_iter, iter, 1, v_iter);
    // p = - d_rare / v_iter
    complex float p = -d_rare * v_iter;
    // v_j = d_j / (2 p), j in [iter + 1, n]
    for (uint8_t j = iter + 1; j <= size; ++j) {
      set_matrix_val(V_iter, j, 1,
                     get_matrix_val(norm_col_iter, j, 1) / (2 * p));
    }
    drop_matrix(norm_col_iter);
    // get transposed V_iter
    MatrixT *trans_V_iter = transpose_matrix(V_iter);
    // calculate V V^T
    MatrixT *matrix_V_Vt = mul_matrix(V_iter, trans_V_iter);
    drop_matrix(trans_V_iter);
    drop_matrix(V_iter);
    // get the matrix -2 V V^T
    MatrixT *double_V_Vt =
        scalar_mul_matrix(new_complex(-2.0f, 0.0f), matrix_V_Vt);
    drop_matrix(matrix_V_Vt);
    // the identity matrix with size (n, n)
    MatrixT *id = new_identity_matrix(size, size);
    // matrix p = I - 2 V V^T
    MatrixT *new_matrix_p = add_matrix(id, double_V_Vt);
    drop_matrix(id);
    drop_matrix(double_V_Vt);
    // store P = P_{n} P_{n - 1} ...
    MatrixT *temp_p = mul_matrix(new_matrix_p, qr_result[0]);
    drop_matrix(qr_result[0]);
    qr_result[0] = temp_p;
    // apply p matrix R = P_{n} (P_{n - 1} ...) A
    MatrixT *temp_r = mul_matrix(new_matrix_p, qr_result[1]);
    drop_matrix(qr_result[1]);
    qr_result[1] = temp_r;
    drop_matrix(new_matrix_p);
  }
  // get matrix Q = (P_{n} P_{n - 1} ...)^T
  MatrixT *matrix_q = transpose_matrix(qr_result[0]);
  drop_matrix(qr_result[0]);
  qr_result[0] = matrix_q;
  // return: result of QR decomposition
  return qr_result;
}

/// @function: get_matrix_eigenvalue_qr (const MatrixT *,
///                                      size_t)
///                                     -> MatrixT *
/// @param: <matrix> the matrix to use
/// @param: <max_iter> maximum iter times
/// @return: a matrix contains the eigenvalue of the matrix
/// @info: use QR method to calculate eigenvalue of a matrix
MatrixT *get_matrix_eigenvalue_qr(const MatrixT *matrix, size_t max_iter) {
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
  // start iter
  MatrixT *matrix_A = copy_matrix(matrix);
  size_t iter = 0;
  while (iter < max_iter) {
    if (is_upper_triangle(matrix_A)) {
      break;
    }
    // A_{n} = R_{n - 1} Q_{n - 1}
    MatrixT **qr_res = decomposition_matrix_qr(matrix_A);
    MatrixT *next_matrix_A = mul_matrix(qr_res[1], qr_res[0]);
    drop_matrix(matrix_A);
    drop_matrices(qr_res, 2);
    matrix_A = next_matrix_A;
    iter++;
  }
  // check iter times
  if (iter >= max_iter) {
    log_warn("warn: reach the max iter");
  }
  return matrix_A;
}

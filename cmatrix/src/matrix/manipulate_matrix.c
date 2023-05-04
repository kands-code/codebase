/// @file: matrix/manipulate_matrix.c
/// @info: manipulate matrices

// include

#include "matrix/matrix.h"
#include "matrix/utils.h"
#include <complex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// functions: manipulate

/// @function: show_matrix (const MatrixT *)
///                        -> unit
/// @param: <matrix> the matrix to show
/// @info: print the matrix with default precision
void show_matrix(const MatrixT *matrix) {
  // default precision: 3
  show_matrix_with_precision(matrix, 3);
}

/// @function: show_matrix (const MatrixT *,
///                         uint8_t)
///                        -> unit
/// @param: <matrix> the matrix to show
/// @param: <precision> the precision to use
/// @info: print the matrix with a specific precision
void show_matrix_with_precision(const MatrixT *matrix, uint8_t precision) {
  // boundary test: null pointer
  if (matrix == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // print start flag
  puts("<<matrix>>");
  // print data
  for (size_t i = 1; i <= matrix->size[0]; ++i) {
    printf("[");
    for (size_t j = 1; j < matrix->size[1]; ++j) {
      complex float val = get_matrix_val(matrix, i, j);
      printf("%*.*f%+.*f, ", precision * 2, precision, crealf(val), precision,
             cimagf(val));
    }
    complex float val = get_matrix_val(matrix, i, matrix->size[1]);
    printf("%*.*f%+.*f]\n", precision * 2, precision, crealf(val), precision,
           cimagf(val));
  }
  // print end flag
  puts("<<matrix>>");
}

/// @function: transpose_matrix (const MatrixT *)
///                             -> MatrixT *
/// @param: <matrix> the matrix to use
/// @return: the transposed matrix
/// @info: transpose a matrix
MatrixT *transpose_matrix(const MatrixT *matrix) {
  // boundary test: null pointer
  if (matrix == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // init: transposed matrix
  MatrixT *transposed_matrix = new_matrix(matrix->size[1], matrix->size[0]);
  for (size_t i = 1; i <= matrix->size[0]; ++i) {
    for (size_t j = 1; j <= matrix->size[1]; ++j) {
      set_matrix_val(transposed_matrix, j, i, get_matrix_val(matrix, i, j));
    }
  }
  // return: transposed matrix
  return transposed_matrix;
}

/// @function: scalar_mul_matrix (complex float, const MatrixT *)
///                              -> MatrixT *
/// @param: <scalar> the scalar to use
/// @param: <matrix> the matrix to use
/// @return: the production of scalar and matrix
/// @info: do scalar production with the scalar and the matrix
MatrixT *scalar_mul_matrix(complex float scalar, const MatrixT *matrix) {
  // boundary test: null pointer
  if (matrix == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // init: production matrix
  MatrixT *prod_matrix = new_matrix(matrix->size[0], matrix->size[1]);
  // do scalar production
  for (size_t i = 0; i < matrix->size[0] * matrix->size[1]; ++i) {
    prod_matrix->data[i] = matrix->data[i] * scalar;
  }
  return prod_matrix;
}

/// @function: add_matrix (const MatrixT *, const MatrixT *)
///                       -> MatrixT *
/// @param: <lhm> the left hand side matrix
/// @param: <rhm> the right hand side matrix
/// @return: the sum of the two matrices
/// @info: do addition of two matrices
MatrixT *add_matrix(const MatrixT *lhm, const MatrixT *rhm) {
  // boundary test: null pointer
  if (lhm == NULL || rhm == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // boundary test: equal size
  if (lhm->size[0] != rhm->size[0] || lhm->size[1] != rhm->size[1]) {
    log_error(
        "panic: lhm size (%u, %u) is not compatible with rhm size (%u, %u)",
        lhm->size[0], lhm->size[1], rhm->size[0], rhm->size[1]);
    exit(EXIT_FAILURE);
  }
  // init: sum matrix
  MatrixT *sum_matrix = new_matrix(lhm->size[0], rhm->size[1]);
  // add two matrices
  for (size_t i = 0; i < rhm->size[0] * lhm->size[1]; ++i) {
    sum_matrix->data[i] = lhm->data[i] + rhm->data[i];
  }
  // return: sum matrix
  return sum_matrix;
}

/// @function: vector_inner_product (const MatrixT *, const MatrixT *)
///                                 -> complex float
/// @param: <lhv> the left hand side vector
/// @param: <rhv> the right hand side vector
/// @return: the inner production of the two vectors
/// @info: do common inner production of two vectors
complex float vector_inner_product(const MatrixT *lhv, const MatrixT *rhv) {
  // boundary test: null pointer
  if (lhv == NULL || rhv == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // boundary test: vector
  if (!((lhv->size[0] == 1 && rhv->size[0] == 1 &&
         lhv->size[1] == rhv->size[1]) ||
        (lhv->size[1] == 1 && rhv->size[1] == 1 &&
         lhv->size[0] == rhv->size[0]))) {
    log_error(
        "panic: lhv size (%u, %u) is not compatible with rhv size (%u, %u)",
        lhv->size[0], lhv->size[1], rhv->size[0], rhv->size[1]);
    exit(EXIT_FAILURE);
  }
  // do inner product
  complex float inner_prod = new_complex(0.0f, 0.0f);
  for (size_t i = 0; i < lhv->size[0] * rhv->size[1]; ++i) {
    inner_prod += lhv->data[i] * rhv->data[i];
  }
  return inner_prod;
}

/// @function: vector_col_row_product (const MatrixT *, const MatrixT *)
///                                   -> MatrixT *
/// @param: <lhv> the left hand side vector (coloumn vector)
/// @param: <rhv> the right hand side vector (row vector)
/// @return: the inner production of the two vectors
/// @info: do inner production of two vectors
MatrixT *vector_col_row_product(const MatrixT *lhv, const MatrixT *rhv) {
  // boundary test: null pointer
  if (lhv == NULL || rhv == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // boundary test: compatible size
  if (!(lhv->size[1] == 1 && rhv->size[0] == 1)) {
    log_error(
        "panic: lhv size (%u, %u) is not compatible with rhv size (%u, %u)",
        lhv->size[0], lhv->size[1], rhv->size[0], rhv->size[1]);
    exit(EXIT_FAILURE);
  }
  // init: inner product
  MatrixT *inner_prod = new_matrix(lhv->size[0], rhv->size[1]);
  // do inner production
  for (size_t i = 1; i <= lhv->size[0]; ++i) {
    for (size_t j = 1; j <= rhv->size[1]; ++j) {
      set_matrix_val(inner_prod, i, j,
                     get_matrix_val(lhv, i, 1) * get_matrix_val(rhv, 1, j));
    }
  }
  return inner_prod;
}

/// @function: mul_matrix (const MatrixT *, const MatrixT *)
///                       -> MatrixT *
/// @param: <lhm> the left hand side matrix
/// @param: <rhm> the right hand side matrix
/// @return: the production of the two matrices
/// @info: do production of two matrices
MatrixT *mul_matrix(const MatrixT *lhm, const MatrixT *rhm) {
  // boundary test: null pointer
  if (lhm == NULL || rhm == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // boundary test: compitable size
  if (lhm->size[1] != rhm->size[0]) {
    log_error(
        "panic: lhm size (%u, %u) is not compatible with rhm size (%u, %u)",
        lhm->size[0], lhm->size[1], rhm->size[0], rhm->size[1]);
    exit(EXIT_FAILURE);
  }
  // init: production matrix
  MatrixT *prod_matrix = new_matrix(lhm->size[0], rhm->size[1]);
  // do production
  for (size_t i = 1; i <= lhm->size[1]; ++i) {
    MatrixT *col_vector = get_matrix_col(lhm, i);
    MatrixT *row_vector = get_matrix_row(rhm, i);
    MatrixT *temp_matrix = vector_col_row_product(col_vector, row_vector);
    drop_matrix(row_vector);
    drop_matrix(col_vector);
    MatrixT *new_prod_matrix = add_matrix(temp_matrix, prod_matrix);
    drop_matrix(temp_matrix);
    drop_matrix(prod_matrix);
    prod_matrix = new_prod_matrix;
  }
  return prod_matrix;
}

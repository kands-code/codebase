/**
 * @file matrix/matrix.h
 * @brief main header file of matrix library
 */

#pragma once
#include <stdbool.h>
#ifndef __MATRIX_MATRIX_H__
#define __MATRIX_MATRIX_H__

// include

#include <complex.h>
#include <stddef.h>
#include <stdint.h>

// types

/**
 * @brief matrix type
 */
typedef struct MatrixT {
  uint8_t size[2];     ///< size of matrix
  complex float *data; ///< data of matrix
} MatrixT;

/**
 * @brief the orientation which is used to init the matrix
 */
typedef enum MatrixOrientation {
  ROW = 0,    ///< row orientation
  COLUMN = 1, ///< column orientation
} MatrixOrientation;

// functions: init

/**
 * @brief construct a complex number
 *
 * @param[in] real the real part of the complex number
 * @param[in] imag the imagine part of  the complex number
 * @return the complex number
 */
extern complex float new_complex(float real, float imag);

/**
 * @brief construct an zero matrix
 *
 * @param[in] row the row size of matrix
 * @param[in] col the column size of matrix
 * @return the matrix with size ( \p row, \p col ) filled with zero
 */
extern MatrixT *new_matrix(uint8_t row, uint8_t col);

/**
 * @brief construct an identity matrix
 *
 * @param[in] row the row size of matrix
 * @param[in] col the column size of matrix
 * @return the identity matrix with size ( \p row, \p col )
 */
extern MatrixT *new_identity_matrix(uint8_t row, uint8_t col);

/**
 * @brief construct a random real matrix
 *
 * @param[in] row the row size of matrix
 * @param[in] col the col size of matrix
 * @return the random matrix with size ( \p row, \p col )
 */
extern MatrixT *new_random_real_matrix(uint8_t row, uint8_t col);

/**
 * @brief construct a random matrix
 *
 * @param[in] row the row size of matrix
 * @param[in] col the col size of matrix
 * @return the random matrix with size ( \p row, \p col )
 */
extern MatrixT *new_random_matrix(uint8_t row, uint8_t col);

/**
 * @brief construct an zero matrix from an array
 *
 * @param[in] row the row size of matrix
 * @param[in] col the column size of matrix
 * @param[in] orientation the orientation which is used
 * @param[in] array the array to use ( len( \p array ) <= \p row * \p col )
 * @return the matrix with size ( \p row, \p col) filled by \p array
 */
extern MatrixT *new_matrix_from_array(uint8_t row, uint8_t col,
                                      MatrixOrientation orientation,
                                      const complex float *array);

/**
 * @brief construct a matrix from user input
 *
 * @return a matrix
 */
extern MatrixT *new_matrix_from_input();

/**
 * @brief read matrices from file
 *
 * @param[in] file_path the path to the file
 * @param[in] matrix_number number of matrix to read
 * @return matrices from file
 */
extern MatrixT **new_matrix_from_file(const char *file_path,
                                      size_t *matrix_number);

/**
 * @brief save matrices to file filled by ROW
 *
 * @param[in] file_path the path to the file
 * @param[in] matrices the matrices to save
 * @param[in] matrix_number number of matrix to save
 */
extern void save_matrix_to_file(const char *file_path, MatrixT **matrices,
                                size_t matrix_number);

/**
 * @brief copy a matrix
 *
 * @param[in] matrix the original matrix
 * @return the copy of the original matrix
 */
extern MatrixT *copy_matrix(const MatrixT *matrix);

/**
 * @brief delete a matrix
 *
 * @param[in] matrix the matrix to drop
 */
extern void drop_matrix(MatrixT *matrix);

/**
 * @brief delete matrices at one time
 * @param[in] matrices matrices to drop
 * @param[in] matrices_number number of matrices to drop
 */
extern void drop_matrices(MatrixT **matrices, size_t matrices_number);

// functions: attribute

/**
 * @brief get value at the specific position of a matrix
 *
 * @param[in] matrix the matrix to use
 * @param[in] row the row position of value
 * @param[in] col the column position of value
 * @return the value at (row, col) of the matrix
 */
extern complex float get_matrix_val(const MatrixT *matrix, uint8_t row,
                                    uint8_t col);

/**
 * @brief set the value at the specific position of a matrix
 *
 * @param[in] matrix the matrix to modify
 * @param[in] row the row position of value
 * @param[in] col the column position of value
 * @param[in] val the value to use
 */
extern void set_matrix_val(MatrixT *matrix, uint8_t row, uint8_t col,
                           complex float val);

/**
 * @brief get the row of a matrix
 *
 * @param[in] matrix the matrix to use
 * @param[in] row the row to get
 * @return the row vector of \p matrix
 */
extern MatrixT *get_matrix_row(const MatrixT *matrix, uint8_t row);

/**
 * @brief get the column of a matrix
 *
 * @param[in] matrix the matrix to use
 * @param[in] col the column to get
 * @return the column vector of \p matrix
 */
extern MatrixT *get_matrix_col(const MatrixT *matrix, uint8_t col);

/**
 * @brief check a matrix whether a upper matrix
 *
 * @param[in] matrix the matrix to check
 * @return if can be regarded as upper matrix, return true, or false
 */
extern bool is_upper_triangle(const MatrixT *matrix);

/**
 * @brief get the trace of matrix
 *
 * @param[in] matrix the matrix to use
 * @return the trace of \p matrix
 */
extern complex float get_matrix_trace(const MatrixT *matrix);

/**
 * @brief get the Frobenius Norm of a matrix
 *
 * @param[in] matrix the matrix to use
 * @return the Frobenius Norm
 */
extern complex float get_matrix_frobenius_norm(const MatrixT *matrix);

/**
 * @brief get the matrix rank
 *
 * @param[in] matrix the matrix to use
 * @return the rank of \p matrix
 */
extern uint8_t get_matrix_rank(const MatrixT *matrix);

/**
 * @brief get the submatrix of a matrix
 *
 * @param[in] matrix the matrix to use
 * @param[in] row the row to omit
 * @param[in] col the col to omit
 * @return the submatrix of \p matrix
 */
extern MatrixT *get_submatrix(const MatrixT *matrix, uint8_t row, uint8_t col);

/**
 * @brief get the cofactor of a matrix
 *
 * @param[in] matrix the matrix to use
 * @param[in] row the row to omit
 * @param[in] col the col to omit
 * @return the cofacter of \p matrix
 */
extern complex float get_matrix_cofactor(const MatrixT *matrix, uint8_t row,
                                         uint8_t col);

/**
 * @brief get the algebraic cofactor of a matrix
 *
 * @param[in] matrix the matrix to use
 * @param[in] row the row to omit
 * @param[in] col the col to omit
 * @return the algebraic cofacter of \p matrix
 */
extern complex float get_matrix_algebraic_cofactor(const MatrixT *matrix,
                                                   uint8_t row, uint8_t col);

/**
 * @brief calculate the determinant of a matrix
 *
 * @param[in] matrix the matrix to use
 * @return the determinant of \p matrix
 */
extern complex float get_matrix_determinant(const MatrixT *matrix);

/**
 * @brief get the adjoint matrix of a matrix
 *
 * @param[in] matrix the matrix to use
 * @return the adjoint matrix of \p matrix
 */
extern MatrixT *get_adjoint_matrix(const MatrixT *matrix);

/**
 * @brief get the inverse matrix of a matrix
 *
 * @param[in] matrix matrix to use
 * @return the inverse matrix of \p matrix
 */
extern MatrixT *get_inverse_matrix(const MatrixT *matrix);

// functions: manipulate

/**
 * @brief  print the matrix with default precision
 *
 * @param[in] matrix the matrix to show
 */
extern void show_matrix(const MatrixT *matrix);

/**
 * @brief print the matrix with a specific precision
 *
 * @param[in] matrix the matrix to show
 * @param[in] precision the precision to use
 */
extern void show_matrix_with_precision(const MatrixT *matrix,
                                       uint8_t precision);

/**
 * @brief transpose a matrix
 *
 * @param[in] matrix the matrix to use
 * @return the transpose matrix of \p matrix
 */
extern MatrixT *transpose_matrix(const MatrixT *matrix);

/**
 * @brief do scalar product with the scalar and a matrix
 *
 * @param[in] scalar the scalar to use
 * @param[in] matrix the matrix to use
 * @return the product with \p scalar and \p matrix
 */
extern MatrixT *scalar_mul_matrix(complex float scalar, const MatrixT *matrix);

/**
 * @brief do addition of two matrices
 *
 * @param[in] lsm the left hand side matrix
 * @param[in] rsm the right hand side matrix
 * @return the sum of the lhm and rhm
 */
extern MatrixT *add_matrix(const MatrixT *lsm, const MatrixT *rsm);

/**
 * @brief do common inner product of two vectors
 *
 * @param[in] lhv the left hand side vector
 * @param[in] rhv the right hand side vector
 * @return the inner product of the \p lhv and \p rhv
 */
extern complex float vector_inner_product(const MatrixT *lhv,
                                          const MatrixT *rhv);

/**
 * @brief do inner product of two vectors
 *
 * @param[in] lhv the left hand side vector
 * @param[in] rhv the right hand side vector
 * @return the inner product of the \p lhv and \p rhv
 */
extern MatrixT *vector_col_row_product(const MatrixT *lhv, const MatrixT *rhv);

/**
 * @brief do cross product of two vectors
 *
 * @param[in] lhv the left hand side vector
 * @param[in] rhv the right hand side vector
 * @return the cross product of the \p lhv and \p rhv
 */
extern MatrixT *vector_cross_product_3d(const MatrixT *lhv, const MatrixT *rhv);

/**
 * @brief do multiplication of two matrices
 *
 * @param[in] lhm the left hand side matrix
 * @param[in] rhm the right hand side matrix
 * @return the product of the \p lhm and \p rhm
 */
extern MatrixT *mul_matrix(const MatrixT *lhm, const MatrixT *rhm);

/**
 * @brief do tensor product of two matrices
 *
 * @param[in] lhm the left hand side matrix
 * @param[in] rhm the right hand side matrix
 * @return the tensor product of the \p lhm and \p rhm
 */
extern MatrixT *tensor_product_matrix(const MatrixT *lhm, const MatrixT *rhm);

#endif
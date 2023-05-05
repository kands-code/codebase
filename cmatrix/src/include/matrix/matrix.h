/// @file: matrix/matrix.h
/// @info: main header file of matrix library

#pragma once
#include <stdbool.h>
#ifndef __MATRIX_MATRIX_H__
#define __MATRIX_MATRIX_H__

// include

#include <complex.h>
#include <stddef.h>
#include <stdint.h>

// types

/// @type: MatrixT { uint8_t[2]; complex float * }
/// @content: <size> size of matrix
/// @content: <data> data of matrix
/// @info: matrix type
typedef struct MatrixT {
  uint8_t size[2];
  complex float *data;
} MatrixT;

/// @type: MatrixOrientation = ROW | COLUMN
/// @info: the orientation which is used to init the matrix
typedef enum MatrixOrientation {
  ROW = 0,
  COLUMN = 1,
} MatrixOrientation;

// functions: init

/// @function: new_complex (float, float)
///                        -> complex float
/// @param: <real> the real part of the complex number
/// @param: <imag> the imagine part of  the complex number
/// @return: the complex number
/// @info: construct a complex number with float type
extern complex float new_complex(float x, float y);

/// @function: new_matrix (uint8_t, uint8_t)
///                       -> MatrixT *
/// @param: <row> the row size of matrix
/// @param: <col> the column size of matrix
/// @return: the matrix with size (row, col) filled with zero
/// @info: construct an zero matrix with size (row, col)
extern MatrixT *new_matrix(uint8_t row, uint8_t col);

/// @function: new_identity_matrix (uint8_t, uint8_t)
///                                -> MatrixT *
/// @param: <row> the row size of matrix
/// @param: <col> the column size of matrix
/// @return: the identity matrix with size (row, col)
/// @info: construct an identity matrix with size (row, col)
extern MatrixT *new_identity_matrix(uint8_t row, uint8_t col);

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
extern MatrixT *new_matrix_from_array(uint8_t row, uint8_t col,
                                      MatrixOrientation orientation,
                                      const complex float *array);

/// @function: new_matrix_from_file (const char *,
///                                  size_t *)
///                                 -> Matrix **
/// @param: <file_path> the path to the file
/// @param: <matrix_number> number of matrix to read
/// @return: matrices from file
/// @info: read matrices from file
extern MatrixT **new_matrix_from_file(const char *file_path,
                                      size_t *matrix_number);

/// @function: new_matrix_from_file (const char *, Matrix **,
///                                  size_t)
///                                 -> unit
/// @param: <file_path> the path to the file
/// @param: <matrices> the matrices to save
/// @param: <matrix_number> number of matrix to save
/// @info: save matrices to file filled by row
extern void save_matrix_to_file(const char *file_path, MatrixT **matrices,
                                size_t matrix_number);

/// @function: copy_matrix (const MatrixT *)
///                        -> MatrixT *
/// @param: <matrix> the original matrix
/// @return: the copy of the original matrix
/// @info: copy a matrix
extern MatrixT *copy_matrix(const MatrixT *matrix);

/// @function: drop_matrix (MatrixT *)
///                        -> unit
/// @param: <matrix> the matrix to drop
/// @info: delete a matrix
extern void drop_matrix(MatrixT *matrix);

/// @function: drop_matrices (Matrix **, size_t)
///                          -> unit
/// @param: <matrices> matrices to drop
/// @param: <matrices_number> number of matrices to drop
/// @info: delete matrices at one time
extern void drop_matrices(MatrixT **matrices, size_t matrices_number);

// functions: attribute

/// @function: get_matrix_val (const MatrixT *, uint8_t,
///                            uint8_t)
///                           -> complex float
/// @param: <matrix> the matrix to use
/// @param: <row> the row position of value
/// @param: <col> the column position of value
/// @return: the value at (row, col) of the matrix
/// @info: get value at the specific position of a matrix
extern complex float get_matrix_val(const MatrixT *matrix, uint8_t row,
                                    uint8_t col);

/// @function: set_matrix_val (MatrixT *, uint8_t, uint8_t,
///                            complex float)
///                           -> unit
/// @param: <matrix> the matrix to modify
/// @param: <row> the row position of value
/// @param: <col> the column position of value
/// @param: <val> the value to use
/// @return: the value at (row, col) of the matrix
/// @info: set the value at the specific position of a matrix
extern void set_matrix_val(MatrixT *matrix, uint8_t row, uint8_t col,
                           complex float val);

/// @function: get_matrix_row (const MatrixT *, uint8_t)
///                           -> MatrixT *
/// @param: <matrix> the matrix to use
/// @param: <row> the row to get
/// @return: the row vector of the matrix
/// @info: get the row of a matrix
extern MatrixT *get_matrix_row(const MatrixT *matrix, uint8_t row);

/// @function: get_matrix_row (const MatrixT *, uint8_t)
///                           -> MatrixT *
/// @param: <matrix> the matrix to use
/// @param: <col> the column to get
/// @return: the column vector of the matrix
/// @info: get the column of a matrix
extern MatrixT *get_matrix_col(const MatrixT *matrix, uint8_t col);

/// @function: is_upper_triangle (const MatrixT *)
///                              -> bool
/// @param: <matrix> the matrix to check
/// @return: if can be regarded as upper matrix, return true, or false
/// @info: check a matrix whether a upper matrix
extern bool is_upper_triangle(const MatrixT *matrix);

/// @function: get_matrix_trace (const MatrixT *)
///                             -> complex float
/// @param: <matrix> the matrix to use
/// @return: the trace of the matrix
/// @info: get the trace of matrix
extern complex float get_matrix_trace(const MatrixT *matrix);

/// @function: get_matrix_frobenius_norm (const MatrixT *)
///                                      -> complex float
/// @param: <matrix> the matrix to use
/// @return: the Frobenius Norm
/// @info: get the Frobenius Norm of a matrix
extern complex float get_matrix_frobenius_norm(const MatrixT *matrix);

/// @function: get_matrix_rank (const MatrixT *)
///                            -> uint8_t
/// @param: <matrix> the matrix to use
/// @return: the rank of the matrix
/// @info: get the matrix rank
extern uint8_t get_matrix_rank(const MatrixT *matrix);

/// @function: get_submatrix (const MatrixT *, uint8_t, uint8_t)
///                          -> MatrixT *
/// @param: <matrix> the matrix to use
/// @param: <row> the row to omit
/// @param: <col> the col to omit
/// @return: the submatrix of matrix
/// @info: get the submatrix of a matrix
extern MatrixT *get_submatrix(const MatrixT *matrix, uint8_t row, uint8_t col);

/// @function: get_matrix_cofactor (const MatrixT *, uint8_t,
///                                 uint8_t)
///                                -> complex float
/// @param: <matrix> the matrix to use
/// @param: <row> the row to omit
/// @param: <col> the col to omit
/// @return: the cofacter of the matrix
/// @info: get the cofactor of a matrix
extern complex float get_matrix_cofactor(const MatrixT *matrix, uint8_t row,
                                         uint8_t col);

/// @function: get_matrix_algebraic_cofactor (const MatrixT *, uint8_t,
///                                           uint8_t)
///                                          -> complex float
/// @param: <matrix> the matrix to use
/// @param: <row> the row to omit
/// @param: <col> the col to omit
/// @return: the algebraic cofacter of the matrix
/// @info: get the algebraic cofactor of a matrix
extern complex float get_matrix_algebraic_cofactor(const MatrixT *matrix,
                                                   uint8_t row, uint8_t col);

/// @function: get_matrix_determinant (const MatrixT *)
///                                   -> complex float
/// @param: <matrix> the matrix to use
/// @return: the determinant of the matrix
/// @info: calculate the determinant of a matrix
extern complex float get_matrix_determinant(const MatrixT *matrix);

/// @function: get_adjoint_matrix (const MatrixT *)
///                               -> MatrixT *
/// @param: the matrix to use
/// @return: the adjoint matrix of the matrix
/// @info: get the adjoint matrix of a matrix
extern MatrixT *get_adjoint_matrix(const MatrixT *matrix);

/// @function: get_inverse_matrix (const MatrixT *)
///                               -> MatrixT *
/// @param: <matrix> matrix to use
/// @return: the inverse matrix
/// @info: get the inverse matrix of a matrix
extern MatrixT *get_inverse_matrix(const MatrixT *matrix);

// functions: manipulate

/// @function: show_matrix (const MatrixT *)
///                        -> unit
/// @param: <matrix> the matrix to show
/// @info: print the matrix with default precision
extern void show_matrix(const MatrixT *matrix);

/// @function: show_matrix (const MatrixT *,
///                         uint8_t)
///                        -> unit
/// @param: <matrix> the matrix to show
/// @param: <precision> the precision to use
/// @info: print the matrix with a specific precision
extern void show_matrix_with_precision(const MatrixT *matrix,
                                       uint8_t precision);

/// @function: transpose_matrix (const MatrixT *)
///                             -> MatrixT *
/// @param: <matrix> the matrix to use
/// @return: the transposed matrix
/// @info: transpose a matrix
extern MatrixT *transpose_matrix(const MatrixT *matrix);

/// @function: scalar_mul_matrix (complex float, const MatrixT *)
///                              -> MatrixT *
/// @param: <scalar> the scalar to use
/// @param: <matrix> the matrix to use
/// @return: the production of scalar and matrix
/// @info: do scalar production with the scalar and the matrix
extern MatrixT *scalar_mul_matrix(complex float scalar, const MatrixT *matrix);

/// @function: add_matrix (const MatrixT *, const MatrixT *)
///                       -> MatrixT *
/// @param: <lhm> the left hand side matrix
/// @param: <rhm> the right hand side matrix
/// @return: the sum of the two matrices
/// @info: do addition of two matrices
extern MatrixT *add_matrix(const MatrixT *lhm, const MatrixT *rhm);

/// @function: vector_inner_product (const MatrixT *, const MatrixT *)
///                                 -> complex float
/// @param: <lhv> the left hand side vector
/// @param: <rhv> the right hand side vector
/// @return: the inner production of the two vectors
/// @info: do common inner production of two vectors
extern complex float vector_inner_product(const MatrixT *lhv,
                                          const MatrixT *rhv);

/// @function: vector_col_row_product (const MatrixT *, const MatrixT *)
///                                   -> MatrixT *
/// @param: <lhv> the left hand side vector (column vector)
/// @param: <rhv> the right hand side vector (row vector)
/// @return: the inner production of the two vectors
/// @info: do inner production of two vectors
extern MatrixT *vector_col_row_product(const MatrixT *lhv, const MatrixT *rhv);

/// @function: mul_matrix (const MatrixT *, const MatrixT *)
///                       -> MatrixT *
/// @param: <lhm> the left hand side matrix
/// @param: <rhm> the right hand side matrix
/// @return: the production of the two matrices
/// @info: do production of two matrices
extern MatrixT *mul_matrix(const MatrixT *lhm, const MatrixT *rhm);

#endif
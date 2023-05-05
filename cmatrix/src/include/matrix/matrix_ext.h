/// @file: matrix/matrix_ext.h
/// @info: extensional header file of matrix library

#pragma once
#ifndef __MATRIX_MATRIX_EXT_H__
#define __MATRIX_MATRIX_EXT_H__

// include

#include "matrix/matrix.h"

// function: extensions

/// @function: upper_triangularize_matrix (const MatrixT *)
///                                    -> MatrixT **
/// @param: <matrix> the matrix to decomposition
/// @return: the result of LU decomposition of matrix
/// @info: get the INVERSE Left matrix and Right matrix
extern MatrixT **upper_triangularize_matrix(const MatrixT *matrix);

/// @function: decomposition_matrix_lu (const MatrixT *)
///                                    -> MatrixT **
/// @param: <matrix> the matrix to decomposition
/// @return: the result of LU decomposition of matrix
/// @info: get the Left matrix and Right matrix
extern MatrixT **decomposition_matrix_lu(const MatrixT *matrix);

/// @function: simplify_matrix (const MatrixT *)
///                            -> MatrixT *
/// @param: <matrix> the matrix to simplify
/// @return: the simplified matrix
/// @info: get the simplest form of the matrix
extern MatrixT *simplify_matrix(const MatrixT *matrix);

/// @function: decomposition_matrix_qr (const MatrixT *)
///                                    -> MatrixT **
/// @param: <matrix> the matrix to decomposition
/// @return: the result of QR decomposition of matrix
/// @info: apply QR decomposition on the matrix
extern MatrixT **decomposition_matrix_qr(const MatrixT *matrix);

/// @function: get_matrix_eigenvalue_qr (const MatrixT *,
///                                      size_t)
///                                     -> MatrixT **
/// @param: <matrix> the matrix to use
/// @param: <max_iter> maximum iter times
/// @return: the eigen system of the matrix
/// @info: use QR method to calculate eigenvalue of a matrix
extern MatrixT **get_matrix_eigenvalue_qr(const MatrixT *matrix,
                                          size_t max_iter);

#endif
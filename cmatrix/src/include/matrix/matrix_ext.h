/// @file: matrix/matrix_ext.h
/// @info: extensional header file of matrix library

#pragma once
#ifndef __MATRIX_MATRIX_EXT_H__
#define __MATRIX_MATRIX_EXT_H__

// include

#include "matrix/matrix.h"

// function: extensions

/// @function: decomposition_matrix_lu (const MatrixT *)
///                                    -> MatrixT **
/// @param: <matrix> the matrix to decomposition
/// @return: the result of LU decomposition of matrix
/// @info: get the INVERSE Left matrix and Right matrix
extern MatrixT **decomposition_matrix_lu(const MatrixT *matrix);

/// @function: simplify_matrix (const MatrixT *)
///                            -> MatrixT *
/// @param: <matrix> the matrix to simplify
/// @return: the simplified matrix
/// @info: get the simplest form of the matrix
extern MatrixT *simplify_matrix(const MatrixT *matrix);

#endif
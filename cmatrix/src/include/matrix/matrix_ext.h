/**
 * @file matrix/matrix_ext.h
 * @brief extensional header file of matrix library
 */

#pragma once
#ifndef __MATRIX_MATRIX_EXT_H__
#define __MATRIX_MATRIX_EXT_H__

// include

#include "matrix/matrix.h"

// function: extensions

/**
 * @brief triangularize a matrix
 *
 * @param[in] matrix the matrix to use
 * @return the premutation matrix and triangularized \p matrix
 */
extern MatrixT **upper_triangularize_matrix(const MatrixT *matrix);

/**
 * @brief decompose a matrix with LU method
 *
 * @param[in] matrix the matrix to use
 * @return the result of LU decomposition of \p matrix
 */
extern MatrixT **decomposition_matrix_lu(const MatrixT *matrix);

/**
 * @brief simplify a matrix
 *
 * @param[in] matrix the matrix to simplify
 * @return simplified \p matrix
 */
extern MatrixT *simplify_matrix(const MatrixT *matrix);

/**
 * @brief decompose a matrix with QR method
 *
 * @param[in] matrix the matrix to use
 * @return the result of QR decomposition of \p matrix
 */
extern MatrixT **decomposition_matrix_qr(const MatrixT *matrix);

/**
 * @brief use QR method to calculate the eigen system of a matrix
 *
 * @param[in] matrix the matrix to use
 * @param[in] max_iter maximum iter times
 * @return the eigen system of \p matrix
 */
extern MatrixT **get_matrix_eigensystem_qr(const MatrixT *matrix,
                                           size_t max_iter);

#endif
#include "matrix/matrix.h"
#include "matrix/utils.h"
#include <complex.h>
#include <stdio.h>

int main(void) {
  complex float arr1[] = {1, 2, 3, 4};
  complex float arr2[] = {3, 5, 7, 9};
  MatrixT *matrix1 = new_matrix_from_array(2, 2, ROW, arr1);
  show_matrix(matrix1);
  MatrixT *matrix2 = new_matrix_from_array(2, 2, ROW, arr2);
  show_matrix(matrix2);
  MatrixT *prod_matrix = mul_matrix(matrix1, matrix2);
  show_matrix(prod_matrix);
  return 0;
}
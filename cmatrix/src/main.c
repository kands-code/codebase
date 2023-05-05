#include "matrix/matrix.h"
#include "matrix/matrix_ext.h"
#include "matrix/utils.h"
#include <complex.h>
#include <stddef.h>
#include <stdlib.h>

int main(void) {
  complex float arr1[] = {3, -3, 1};
  complex float arr2[] = {4, 9, 2};
  MatrixT *mat1 = new_matrix_from_array(3, 1, ROW, arr1);
  MatrixT *mat2 = new_matrix_from_array(3, 1, ROW, arr2);

  MatrixT *kro = vector_cross_product_3d(mat1, mat2);
  show_matrix(kro);
  drop_matrix(kro);

  drop_matrix(mat2);
  drop_matrix(mat1);

  return 0;
}

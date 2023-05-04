#include "matrix/matrix.h"
#include "matrix/matrix_ext.h"
#include "matrix/utils.h"
#include <complex.h>
#include <stddef.h>
#include <stdlib.h>

int main(void) {
  // size_t cnt = 0;
  // MatrixT **matrices = new_matrix_from_file("data/test.mt", &cnt);
  // for (size_t i = 0; i < cnt; ++i) {
  //   show_matrix(matrices[i]);
  // }
  // for (size_t i = 0; i < cnt; ++i) {
  //   drop_matrix(matrices[i]);
  // }
  // free(matrices);
  complex float arr1[] = {2, -1, 3, 1, 4, -2, 5, 4, 6, -3, 8, 4};
  MatrixT *mat1 = new_matrix_from_array(3, 4, ROW, arr1);
  MatrixT *sim = simplify_matrix(mat1);
  show_matrix(sim);
  drop_matrix(sim);
  drop_matrix(mat1);
  return 0;
}
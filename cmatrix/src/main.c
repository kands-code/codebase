#include "matrix/matrix.h"
#include "matrix/matrix_ext.h"
#include "matrix/utils.h"
#include <complex.h>
#include <stddef.h>
#include <stdlib.h>

int main(void) {
  complex float arr[] = {5, 5, 5, 5};
  MatrixT *matrix = new_matrix_from_array(2, 2, ROW, arr);
  show_matrix(matrix);
  log_info("=========================");
  MatrixT **lu_res = decomposition_matrix_lu(matrix);

  show_matrix(lu_res[0]);
  log_info("=========================");
  show_matrix(lu_res[1]);
  log_info("=========================");

  drop_matrix(matrix);
  drop_matrices(lu_res, 2);
  return 0;
}

#include "matrix/matrix.h"
#include "matrix/matrix_ext.h"
#include "matrix/utils.h"
#include <complex.h>
#include <stddef.h>
#include <stdlib.h>

int main(void) {
  complex float arr[] = {3, 4, 0, 1, 3, 0, 0, 0, 2};
  MatrixT *matrix = new_matrix_from_array(3, 3, ROW, arr);
  show_matrix(matrix);
  log_info("=========================");
  MatrixT **eigen = get_matrix_eigenvalue_qr(matrix, 50);

  show_matrix(eigen[0]);
  log_info("=========================");
  show_matrix(eigen[1]);
  log_info("=========================");

  drop_matrices(eigen, 2);
  drop_matrix(matrix);

  return 0;
}

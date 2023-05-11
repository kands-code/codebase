#include "matrix/matrix.h"
#include "matrix/matrix_ext.h"
#include "matrix/utils.h"
#include <complex.h>
#include <stddef.h>
#include <stdlib.h>

int main(void) {
  MatrixT *mat = new_matrix_from_input();
  show_matrix(mat);

  complex float det = get_matrix_determinant(mat);
  log_info("det(mat) = %.4f%+.4f", crealf(det), cimagf(det));

  drop_matrix(mat);

  return 0;
}

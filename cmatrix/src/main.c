#include "matrix/matrix.h"
#include "matrix/matrix_ext.h"
#include "matrix/utils.h"
#include <complex.h>
#include <stddef.h>
#include <stdlib.h>

int main(void) {
  complex float arr[] = {1, 4, 7, 2, 5, 8};
  MatrixT *mat = new_matrix_from_array(3, 2, ROW, arr);

  MatrixT **qr = decomposition_matrix_qr(mat);
  show_matrix(qr[0]);
  show_matrix(qr[1]);
  MatrixT *recheck1 = mul_matrix(qr[0], qr[1]);
  show_matrix(recheck1);
  drop_matrix(recheck1);

  MatrixT *trans_q = transpose_matrix(qr[0]);
  MatrixT *recheck2 = mul_matrix(trans_q, qr[0]);
  drop_matrix(trans_q);
  show_matrix(recheck2);
  drop_matrix(recheck2);

  drop_matrices(qr, 2);
  drop_matrix(mat);

  return 0;
}

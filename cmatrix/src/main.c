#include "matrix/matrix.h"
#include "matrix/utils.h"
#include <complex.h>
#include <stddef.h>
#include <stdlib.h>

int main(void) {
  size_t cnt = 0;
  MatrixT **matrices = new_matrix_from_file("data/test.mt", &cnt);
  for (size_t i = 0; i < cnt; ++i) {
    show_matrix(matrices[i]);
  }
  for (size_t i = 0; i < cnt; ++i) {
    drop_matrix(matrices[i]);
  }
  free(matrices);
  return 0;
}
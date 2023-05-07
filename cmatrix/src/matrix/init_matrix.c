/**
 * @file  matrix/init_matrix.c
 * @brief init or delete a matrix
 */

// inlcude

#include "matrix/matrix.h"
#include "matrix/utils.h"
#include <complex.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

// functions: init

complex float new_complex(float real, float imag) {
  // return: complex number
  return __builtin_complex(real, imag);
}

MatrixT *new_matrix(uint8_t row, uint8_t col) {
  // boundary test: size
  if (row == 0 || col == 0) {
    log_error("panic: size must bigger than 0");
    exit(EXIT_FAILURE);
  }
  // malloc: matrix type
  MatrixT *matrix = malloc(sizeof(MatrixT));
  // assign: size
  matrix->size[0] = row;
  matrix->size[1] = col;
  // malloc: matrix data
  matrix->data = calloc(row * col, sizeof(complex float));
  // assign: set data to zeros
  for (size_t i = 0; i < matrix->size[0] * matrix->size[1]; ++i) {
    matrix->data[i] = new_complex(0.0f, 0.0f);
  }
  // return: zero matrix
  return matrix;
}

MatrixT *new_identity_matrix(uint8_t row, uint8_t col) {
  // get an empty matrix
  MatrixT *identity_matrix = new_matrix(row, col);
  // get the size of the diagonal of matrix
  uint8_t matrix_diagonal_size = MIN(row, col);
  // fill the diagonal with <1.0 + 0.0 I>
  for (uint8_t i = 1; i <= matrix_diagonal_size; ++i) {
    set_matrix_val(identity_matrix, i, i, new_complex(1.0f, 0.0f));
  }
  // return: identity matrix
  return identity_matrix;
}

MatrixT *new_random_real_matrix(uint8_t row, uint8_t col) {
  srand(time(NULL));
  MatrixT *rand_matrix = new_matrix(row, col);
  for (uint16_t i = 0; i < row * col; ++i) {
    rand_matrix->data[i] = new_complex((float)rand() / (float)RAND_MAX, 0);
  }
  return rand_matrix;
}

MatrixT *new_random_matrix(uint8_t row, uint8_t col) {
  srand(time(NULL));
  MatrixT *rand_matrix = new_matrix(row, col);
  for (uint16_t i = 0; i < row * col; ++i) {
    rand_matrix->data[i] = new_complex((float)rand() / (float)RAND_MAX,
                                       (float)rand() / (float)RAND_MAX);
  }
  return rand_matrix;
}

MatrixT *new_matrix_from_array(uint8_t row, uint8_t col,
                               MatrixOrientation orientation,
                               const complex float *array) {
  MatrixT *matrix = new_matrix(row, col);
  if (orientation == ROW) {
    for (size_t i = 0; i < matrix->size[0] * matrix->size[1]; ++i) {
      matrix->data[i] = array[i];
    }
  } else if (orientation == COLUMN) {
    MatrixT *row_matrix = new_matrix_from_array(row, col, ROW, array);
    MatrixT *col_matrix = transpose_matrix(row_matrix);
    drop_matrix(row_matrix);
    drop_matrix(matrix);
    matrix = col_matrix;
  } else {
    log_error("panic: illegal argument of orientation: %d", orientation);
    exit(EXIT_FAILURE);
  }
  return matrix;
}

MatrixT **new_matrix_from_file(const char *file_path, size_t *matrix_number) {
  // test: open file
  FILE *file_handle = fopen(file_path, "r");
  if (file_handle == NULL) {
    log_error("panic: failed to open file (%s)", file_path);
    exit(EXIT_FAILURE);
  }
  // test: get file infomation
  struct stat file_stat;
  if (stat(file_path, &file_stat) == -1) {
    log_error("panic: failed to get file stat (%s)", file_path);
    exit(EXIT_FAILURE);
  }
  // init: capacity of matrices
  size_t matrices_capacity = 16;
  size_t matrix_cnt = 0;
  bool is_read_matrix = false;
  MatrixT **matrices = calloc(matrices_capacity, sizeof(MatrixT *));
  // init: read buffer
  char *read_buffer = malloc(file_stat.st_size);
  // start to read file
  while (fscanf(file_handle, "%[^\n] ", read_buffer) != EOF) {
    // boundary test: matrices capacity
    if (matrix_cnt + 2 > matrices_capacity) {
      matrices_capacity *= 2;
      matrices = realloc(matrices, matrices_capacity * sizeof(MatrixT *));
    }
    // init: matrix data
    if (strcmp("[matrix]", read_buffer) == 0) {
      is_read_matrix = true;
      matrices[matrix_cnt] = malloc(sizeof(MatrixT));
    } else if (strncmp("size =", read_buffer, strlen("size =")) == 0 &&
               is_read_matrix) {
      // read size infomation
      sscanf(read_buffer, "size = %hhu %hhu", &matrices[matrix_cnt]->size[0],
             &matrices[matrix_cnt]->size[1]);
      matrices[matrix_cnt]->data =
          calloc(matrices[matrix_cnt]->size[0] * matrices[matrix_cnt]->size[1],
                 sizeof(complex float));
    } else if (strncmp("data =", read_buffer, strlen("data =")) == 0 &&
               is_read_matrix) {
      // read data infomantion
      char *data_buffer = malloc(file_stat.st_size);
      sscanf(read_buffer, "data = %[^\n]", data_buffer);
      // clangd can not find `strtok_r`, weired
      char *value_buffer;
      size_t data_cnt = 0;
      value_buffer = strtok(data_buffer, " ");
      while (value_buffer != NULL &&
             data_cnt < matrices[matrix_cnt]->size[0] *
                            matrices[matrix_cnt]->size[1]) {
        float real = 0.0f;
        float imag = 0.0f;
        sscanf(value_buffer, "%f%f", &real, &imag);
        matrices[matrix_cnt]->data[data_cnt++] = new_complex(real, imag);
        value_buffer = strtok(NULL, " ");
      }
      free(data_buffer);
      is_read_matrix = false;
      matrix_cnt++;
    } else {
      log_error("panic: parser error (%s)", read_buffer);
      exit(EXIT_FAILURE);
    }
  }
  // free read buffer
  free(read_buffer);
  // close file
  fclose(file_handle);
  // return: matrices
  *matrix_number = matrix_cnt;
  return matrices;
}

void save_matrix_to_file(const char *file_path, MatrixT **matrices,
                         size_t matrix_number) {
  // boundary test: null pointer
  if (matrices == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // test: open file
  FILE *file_handle = fopen(file_path, "w");
  if (file_handle == NULL) {
    log_error("panic: failed to open file (%s)", file_path);
    exit(EXIT_FAILURE);
  }
  // write matrix infomations
  for (size_t i = 0; i < matrix_number; ++i) {
    // add matrix flag
    fputs("[matrix]\n", file_handle);
    // save size infomation
    fprintf(file_handle, "size = %u %u\n", matrices[i]->size[0],
            matrices[i]->size[1]);
    // save data
    fprintf(file_handle, "data =");
    for (size_t j = 0; j < matrices[i]->size[0] * matrices[i]->size[1]; ++j) {
      fprintf(file_handle, " %f%+f", crealf(matrices[i]->data[j]),
              cimagf(matrices[i]->data[j]));
    }
    // end write
    fputc('\n', file_handle);
  }
  // close file
  fclose(file_handle);
}

MatrixT *copy_matrix(const MatrixT *matrix) {
  // boundary test: null pointer
  if (matrix == NULL) {
    log_error("panic: null pointer error at %s", __func__);
    exit(EXIT_FAILURE);
  }
  // init: copied matrix
  MatrixT *copied_matrix = new_matrix(matrix->size[0], matrix->size[1]);
  // copy all data from original matrix
  for (size_t i = 0; i < copied_matrix->size[0] * copied_matrix->size[1]; ++i) {
    copied_matrix->data[i] = matrix->data[i];
  }
  // return: copied matrix
  return copied_matrix;
}

extern void drop_matrix(MatrixT *matrix) {
  // if matrix is null, it's fine
  if (matrix == NULL) {
    return;
  }
  // data of matrix maybe null
  if (matrix->data != NULL) {
    free(matrix->data);
  }
  // free the matrix
  free(matrix);
}

void drop_matrices(MatrixT **matrices, size_t matrices_number) {
  for (size_t i = 0; i < matrices_number; ++i) {
    drop_matrix(matrices[i]);
  }
  free(matrices);
}

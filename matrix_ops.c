#include "header.h"

// Takes an int matrix and multiplies a scalar with it
struct floatMatrix intScalarMultiply (struct intMatrix m, double scalar, int thread_num) {
  omp_set_dynamic(0);
  omp_set_num_threads(thread_num);

  struct floatMatrix new_matrix;

  new_matrix.ia = m.ia;
  new_matrix.ja = m.ja;
  new_matrix.rows = m.rows;
  new_matrix.columns = m.columns;

  int dimensions = m.rows * m.columns;

  double *new_nnz = malloc(m.non_zeros * sizeof(double));
  double *new_vals = malloc(sizeof(double) * dimensions);

  #pragma omp parallel
  {
    #pragma omp for
    for (int i = 0; i < m.non_zeros; i++) {
      new_nnz[i] = scalar * ((double) m.nnz[i]);
      // printf("%f\n", new_nnz[i]);
    }

    #pragma omp for
    for (int j = 0; j < dimensions; j++) {
      new_vals[j] = scalar * (double) m.matrix_vals[j];
      printf("%f\n", new_vals[j]);
    }
  }

  new_matrix.nnz = new_nnz;
  new_matrix.matrix_vals =  new_vals;

  return new_matrix;
}

// takes a float matrix and does scalar multiplication
struct floatMatrix floatScalarMultiply (struct floatMatrix m, double scalar, int thread_num) {
  omp_set_dynamic(0);
  omp_set_num_threads(thread_num);

  struct floatMatrix new_matrix;

  new_matrix.ia = m.ia;
  new_matrix.ja = m.ja;
  new_matrix.rows = m.rows;
  new_matrix.columns = m.columns;

  int nnz_len = sizeof(m.nnz)/sizeof(double);
  int dimensions = m.rows * m.columns; 

  double *new_nnz = malloc(nnz_len * sizeof(double));
  double *new_vals = malloc(sizeof(double) * dimensions);

  #pragma omp parallel
  {
    #pragma omp for
    for (int i = 0; i < nnz_len; i++) {
      new_nnz[i] = scalar * (double) m.nnz[i];
    }

    #pragma omp for
    for (int j = 0; j < dimensions; j++) {
      new_vals[j] = scalar * (double) m.matrix_vals[j];
    }
  }

  new_matrix.nnz = new_nnz;


  return new_matrix;
}


struct intMatrix intTranspose (struct intMatrix m) {
  struct intMatrix transposed;

  transposed.columns = m.rows;
  transposed.rows = m.columns;

  int matrix_len = transposed.columns * transposed.rows;

  for (int i = 0; i < matrix_len; i++) {

  }

  

}
#include "header.h"

struct floatMatrix intScalarMultiply(struct intMatrix m, double scalar, int thread_num) {
  omp_set_dynamic(0);
  omp_set_num_threads(thread_num);

  struct floatMatrix new_matrix;

  new_matrix.ia = m.ia;
  new_matrix.ja = m.ja;
  new_matrix.rows = m.rows;
  new_matrix.columns = m.columns;

  int nnz_len = sizeof(m.nnz)/sizeof(int); 

  double *new_nnz = malloc(nnz_len * sizeof(double));

  #pragma omp parallel
  {
    #pragma omp for
    for (int i = 0; i < nnz_len; i++) {
      new_nnz[i] = scalar * m.nnz[i];
    }
  }

  new_matrix.nnz = new_nnz;

  return new_matrix;
}
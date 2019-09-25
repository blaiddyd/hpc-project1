#include "header.h"

struct Matrix scalarMultiply (struct Matrix m, double scalar, int thread_num) {
  omp_set_dynamic(0);
  omp_set_num_threads(thread_num);

  clock_t start = clock();

  struct Matrix new_matrix;

  char cmd[20];
  cmd[0] = '\0';

  sprintf(cmd, "sm %f", scalar);

  new_matrix.ia = m.ia;
  new_matrix.ja = m.ja;
  new_matrix.rows = m.rows;
  new_matrix.columns = m.columns;
  new_matrix.filename = m.filename;
  new_matrix.non_zeros = m.non_zeros;
  new_matrix.is_float = m.is_float;

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
    }
  }

  new_matrix.nnz = new_nnz;
  new_matrix.matrix_vals = new_vals;

  clock_t end = clock();
  clock_t elapsed = end - start;
  double time_taken = (double) elapsed / CLOCKS_PER_SEC;

  // printf("%f\n", time_taken);
  printSingleResult(new_matrix, cmd, "sm", thread_num, time_taken);

  return new_matrix;
}

struct Matrix trace (struct Matrix m, int num_threads) {
  if (m.rows != m.columns) {
    printf("Trace cannot be done for non-square matrices\n");
    exit(EXIT_FAILURE);
  }

  else {
    omp_set_dynamic(0);
    omp_set_num_threads(num_threads);

    clock_t start = clock();

    double trace_sum = 0.0;

    #pragma omp parallel for private(i) shared(m.matrix_vals, trace_sum, m.columns) reduction(+:trace_sum)
    for (int i = 0; i < m.columns; i++) {
      trace_sum += m.matrix_vals[i * m.columns + i];
    }

    clock_t end = clock();
    clock_t elapsed = end - start;
    double time_taken = (double) elapsed / CLOCKS_PER_SEC;

    printTraceResult(m, trace_sum, "tr", num_threads, time_taken);

    return m;
  }
}

struct Matrix matrixAddition (struct Matrix a, struct Matrix b, int num_threads) {
  if ((a.rows * a.columns) != (b.rows * b.columns)) {
    printf("Matrices must have the same dimensions in order to perfrom matrix addition.\n");
    exit(EXIT_FAILURE);
  }

  else {
    omp_set_dynamic(0);
    omp_set_num_threads(num_threads);

    clock_t start = clock();

    int dimensions = a.rows * b.rows;
    double* added_matrix = malloc(sizeof(double) * dimensions);
    struct Matrix result;

    result.columns = a.columns;
    result.rows = a.rows;

    int idx = 0;

    #pragma omp parallel for private(i,j) shared(added_matrix, a, b) reduction(+:idx)
    for (int i = 0; i < a.columns; i++) {
      for (int j = 0; j < a.rows; j++) {
        added_matrix[idx] = a.matrix_vals[j + a.rows * i] + b.matrix_vals[j + b.rows * i];
        idx++;
      }
    }

    result.matrix_vals = added_matrix;

    if (a.is_float == 0) {
      result.is_float = 0;
    }
    else {
      result.is_float = 1;
    }

    clock_t end = clock();
    clock_t elapsed = end - start;
    double time_taken = (double) elapsed / CLOCKS_PER_SEC;

    printDoubleResult(a, b, result, "ad", "ad", num_threads, time_taken);

    return result;
  }
}



struct Matrix transpose (struct Matrix m, int num_threads) {
  omp_set_dynamic(0);
  omp_set_num_threads(num_threads);
  struct Matrix transposed;


  clock_t start = clock();

  transposed.columns = m.rows;
  transposed.rows = m.columns;


  int matrix_len = transposed.columns * transposed.rows;

  double* new_matrix = malloc(sizeof(double) * matrix_len);

  #pragma omp parallel for private(i,j) shared(transposed.rows, transposed.columns, new_matrix)
  for (int i = 0; i < transposed.columns; i++) {
    for (int j = 0; j < transposed.rows; j++) {
      new_matrix[i * transposed.rows + j] = m.matrix_vals[j * transposed.columns + i];
    }
    
  }

  transposed.matrix_vals = new_matrix;
  transposed.is_float = m.is_float;
  transposed.filename = m.filename;

  clock_t end = clock();
  clock_t elapsed = end - start;
  double time_taken = (double) elapsed / CLOCKS_PER_SEC;

  printSingleResult(transposed, "ts", "ts", num_threads, time_taken);

  return transposed;
}

struct Matrix matrixMultiply (struct Matrix a, struct Matrix b, int num_threads) {
  omp_set_dynamic(0);
  omp_set_num_threads(num_threads);

  struct Matrix new_matrix;

  clock_t start = clock();

  int new_dimensions = a.rows * b.columns;
  double *mult_vals = malloc(sizeof(double) * new_dimensions);

  #pragma omp parallel for private(i,j,x) shared(a, b, a.rows, a.columns, b.rows, b.columns, new_matrix.matrix_vals, mult_vals) reduction(+:total)
  for (int i = 0; i < a.rows; i++) {
    for (int j = 0; j < b.columns; j++) {
      double total = 0.0;
      for (int x = 0; x < a.columns; x++) {
        total = total + a.matrix_vals[i * a.columns + x] * b.matrix_vals[x * b.columns + j];
      }
      mult_vals[i * b.columns + j] = total;
    }
  }

  clock_t end = clock();
  clock_t elapsed = end - start;
  double time_taken = (double) elapsed / CLOCKS_PER_SEC; 

  new_matrix.columns = a.rows;
  new_matrix.rows = b.columns;
  new_matrix.matrix_vals = mult_vals;

  if (a.is_float == 0) {
    new_matrix.is_float = 0;
  }

  else {
    new_matrix.is_float = 1;
  }

  printDoubleResult(a, b, new_matrix, "mm", "mm", num_threads, time_taken);

  return new_matrix;
}
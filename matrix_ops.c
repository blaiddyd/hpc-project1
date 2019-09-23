#include "header.h"

// Takes an int matrix and multiplies a scalar with it
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
    clock_t start = clock();

    double trace_sum = 0.0;

    #pragma omp parallel for private(i) shared(m.matrix_vals, trace_sum, m.columns) reduction(+:trace_sum)
    for (int i = 0; i < m.columns; i++) {
      trace_sum += m.matrix_vals[i * m.columns + i];
    }

    // printf("%f\n", trace_sum);

    clock_t end = clock();
    clock_t elapsed = end - start;
    double time_taken = (double) elapsed / CLOCKS_PER_SEC; 
    // printf("Time taken: %f\n", time_taken); 
  }
}

struct Matrix matrixAddition (struct Matrix a, struct Matrix b, int num_threads) {
  
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

  int counter = 0;

  #pragma omp parallel for private(i,j) shared(transposed.rows, transposed.columns) reductions(+:counter)
  for (int i = 0; i < transposed.rows; i++) {
    for (int j = 0; j < transposed.columns; j++) {
    //  printf("%d\n", m.matrix_vals[i + j * transposed.rows]);
      new_matrix[counter++] = m.matrix_vals[j * transposed.columns + i];
    }
    
  }

  transposed.matrix_vals = new_matrix;

  clock_t end = clock();
  clock_t elapsed = end - start;
  double time_taken = (double) elapsed / CLOCKS_PER_SEC;

  // printf("%f\n", time_taken);
  printf("works? %f\n", new_matrix[1]);
}

struct Matrix matrixMultiply (struct Matrix a, struct Matrix b, int num_threads) {
  omp_set_dynamic(0);
  omp_set_num_threads(num_threads);

  struct Matrix new_matrix;

  clock_t start = clock();

  #pragma omp parallel for private(i,j,x) shared(a, b, a.rows, a.columns, b.rows, b.columns, new_matrix.matrix_vals)
  for (int i = 0; i < a.rows; i++) {
    for (int j = 0; j < b.columns; j++) {
      double total = 0.0;
      for (int x = 0; x < b.rows; x++) {
        // printf("A value: %f\n", a.matrix_vals[i + x * a.columns]);
        // printf("B value: %f\n", b.matrix_vals[i + x * b.columns]);
        total = total + a.matrix_vals[i + x * a.columns] * b.matrix_vals[j + x * b.columns];
      }
      new_matrix.matrix_vals[i + j * b.columns] = total;
    }
  }

  for (int i = 0; i < (a.rows * b.columns); i++) {
    printf("%f\n", new_matrix.matrix_vals[i]);
  }

  clock_t end = clock();
  clock_t elapsed = end - start;
  double time_taken = (double) elapsed / CLOCKS_PER_SEC; 
  printf("Time taken: %f\n", time_taken); 

}
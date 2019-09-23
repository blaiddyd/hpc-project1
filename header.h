#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <omp.h>
#include <getopt.h>

// matrix in CSR format
struct Matrix {
  bool is_float;
  int rows;
  int columns;
  int non_zeros;
  double* matrix_vals;
  char* filename;
  // non-zero values stored in row-major order
  double* nnz;
  // ia[n] = ia[n - 2] + num non-zero elements in the previous row
  int* ia;
  // column index of each element starting from top row
  int* ja;

};

struct Matrix convertToCSR(char *filename);
struct Matrix scalarMultiply (struct Matrix m, double scalar, int thread_num);
void printSingleResult (struct Matrix m, char* op_type, char* op_name, int thread_num, double time_taken);
struct Matrix transpose (struct Matrix m, int num_threads);
struct Matrix matrixAddition (struct Matrix a, struct Matrix b, int num_threads);
struct Matrix trace (struct Matrix m, int num_threads);
struct Matrix matrixMultiply (struct Matrix a, struct Matrix b, int num_threads);
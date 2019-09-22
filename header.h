#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <omp.h>

// matrix in CSR format
struct intMatrix {
  // float or int
  int rows;
  int columns;
  // non-zero values stored in row-major order
  int* nnz;
  // ia[n] = ia[n - 2] + num non-zero elements in the previous row
  int* ia;
  // column index of each element starting from top row
  int* ja;

};

struct floatMatrix {
  bool integer;

  int rows;
  int columns;
  // non-zero values stored in row-major order
  double* nnz;
  // ia[n] = ia[n - 2] + num non-zero elements in the previous row
  int* ia;
  // column index of each element starting from top row
  int* ja;
};

struct intMatrix convertToCSR(char *filename);
struct floatMatrix intScalarMultiply(struct intMatrix m, double scalar, int thread_num);
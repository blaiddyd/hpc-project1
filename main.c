#include "header.h"

int main(int argc, char* argv[]) {
    
  struct Matrix m = convertToCSR("example.in");
  struct Matrix n = convertToCSR("example2.in");
  matrixMultiply(m, n, 4);
  scalarMultiply(m, 4.0, 4);

  
  return EXIT_SUCCESS;
}
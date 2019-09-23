#include "header.h"

int main(int argc, char* argv[]) {
    
  struct Matrix m = convertToCSR("example.in");
  trace(m, 4);
  struct Matrix x = scalarMultiply(m, 1.7, 4);
  transpose(x, 4);
  matrixMultiply(m, x, 4);

  
  return EXIT_SUCCESS;
}
#include "header.h"

int main(int argc, char* argv[]) {
    
  struct Matrix m = convertToCSR("example.in");
  scalarMultiply(m, 2.0, 4);

  
  return EXIT_SUCCESS;
}
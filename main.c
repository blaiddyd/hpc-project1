#include "header.h"

int main(int argc, char* argv[]) {
    
  struct Matrix m = convertToCSR("example.in");
  struct Matrix n = convertToCSR("example2.in");
  // struct Matrix n = convertToCSR("example2.in");
  trace(m, 4);

  
  return EXIT_SUCCESS;
}
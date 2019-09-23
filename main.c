#include "header.h"

int main(int argc, char* argv[]) {
    
    struct intMatrix m = convertToCSR("example.in");
    intScalarMultiply(m, 1.7, 4);

    return EXIT_SUCCESS;
}
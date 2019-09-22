#include "header.h"

int main(int argc, char* argv[]) {
    omp_set_num_threads(atoi(argv[1]));

    #pragma omp parallel
    {
        #pragma omp for 
        for (int i = 0; i < omp_get_num_threads(); i++) {
            printf("thread %d\n", i);
        }
    }

    return EXIT_SUCCESS;
}
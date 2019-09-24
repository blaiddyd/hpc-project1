#include "header.h"

int main(int argc, char* argv[]) {
  int op_type = 0;
  // op_type[0] = '\0';
  char* filename = malloc(sizeof(char) * 100);
  filename[0] = '\0';
  char* filename_2 = malloc(sizeof(char) * 100);
  filename_2[0] = '\0';
  double sm_factor = 0;
  int thread_num = 1;

  for (int i = 0; i < argc -1; i++) {

    if (i == 1) {
      if (strstr(argv[i], "sm")) {
        op_type = 1;
        sm_factor =  (double) strtol(argv[i + 1], NULL, 10);
      }
      else if (strstr(argv[i], "mm")) {
        op_type = 2;
      }
      else if (strstr(argv[i], "tr")) {
        op_type = 3;
      }
      else if (strstr(argv[i], "ad")) {
        op_type = 4;
      }
      else if (strstr(argv[i], "ts")) {
        op_type = 5;
      }
      else {
        printf("Invalid argument for operation type.\n");
        exit(EXIT_FAILURE);
      }
    }

    else if (i == 2) {
      if (strstr(argv[i], "-f") && argv[i + 1] != NULL) {
        strcpy(filename, argv[i + 1]);
        if (argv[i + 2] != NULL && strstr(argv[i + 2], ".in") && (op_type == 2 || op_type == 4)) {
          strcpy(filename_2, argv[i + 2]);
        }
      }
    }
    if (strcmp(argv[i], "-t") == 0) {
      thread_num = strtol(argv[i + 1], NULL, 10);
    }
  }

  switch(op_type) {
    case 1:
    {
      struct Matrix m = convertToCSR(filename);
      scalarMultiply(m, sm_factor, thread_num);
      break;
    }
    case 2:
    {
      struct Matrix a = convertToCSR(filename);
      struct Matrix b = convertToCSR(filename_2);
      matrixMultiply(a, b, thread_num);
      break;
    }
    case 3:
    {
      struct Matrix x = convertToCSR(filename);
      trace(x, thread_num);
      break;
    }
    case 4:
    {
      struct Matrix c = convertToCSR(filename);
      struct Matrix d = convertToCSR(filename_2);
      matrixAddition(c, d, thread_num);
      break;
    }
    case 5:
    {
      struct Matrix y = convertToCSR(filename);
      transpose(y, thread_num);
      break;
    }
    default:
    {
      printf("Sorry amigo but no can do.\n");
      return EXIT_FAILURE;
    }
  }
  
  return EXIT_SUCCESS;
}
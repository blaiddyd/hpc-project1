#include "header.h"

int* buildIntNNZ (int len, int *values) {
  int *nnz = malloc(sizeof(int) * len);
  for (int i = 0; i < len; i++) {
    if (values[i] != 0) {
      nnz[i] = values[i];
    }
  }
  return nnz;
}

int* buildIntIA (int rows, int columns, int num_values, int *values) {
  int *ia = malloc(sizeof(int) * (rows + 1));
  ia[0] = 0;
  int pos_count = 0;
  int cell_count = 1;
  int row_count = 1;
  for (int i = 0; i < num_values; i++) {
    if (values[i] != 0) {
      printf("%d\n", values[i]);
      pos_count++;
    }
    cell_count++;
    if (cell_count == columns) {
      ia[row_count] = ia[row_count - 1] + pos_count;
      pos_count = 0;
      cell_count = 1;
      row_count++;
    }
  }
  return ia;
}

int* buildIntJA (int len, int *values) {

}


struct intMatrix convertToCSR(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Could not open the file provided.\n");
    exit(EXIT_FAILURE);
  }

  else {
    char *line = NULL;
    int line_count = 0;
    size_t line_len = 0;
    int rows, columns, nnz_len;
    int *nnz, *ia, *ja;
    struct intMatrix matrix;

    while((getline(&line, &line_len, fp)) != -1) {
      line_count++;
      if (line_count == 2) {
        rows = atoi(line);
      }
      else if (line_count == 3) {
        columns = atoi(line);
      }
      else if (line_count > 3) {
        char *value = strtok(line, " ");
        int j = 0;
        int *matrix_vals = malloc(sizeof(int) * rows * columns);
        
        while (value != NULL) {
          if ((int) strtol(value, NULL, 10) != 0) {
            nnz_len++;
          }
          matrix_vals[j] = (int) strtol(value, NULL, 10);
          j++;
          value = strtok(NULL, " ");
        }
        nnz = buildIntNNZ(nnz_len, matrix_vals);
        ia = buildIntIA(rows, columns, (rows*columns), matrix_vals);
        free(matrix_vals);
        free(value);
      }
    }

    matrix.columns = columns;
    matrix.rows = rows;
    matrix.nnz = nnz;

    free(line);


    return matrix;
  }
}
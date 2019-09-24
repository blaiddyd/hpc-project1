#include "header.h"

double* buildIntNNZ (int len, int dimensions, double *values) {
  double *nnz = malloc(sizeof(double) * len);
  int nnz_idx = 0;
  for (int i = 0; i < dimensions; i++) {
    // printf("%d\n", values[i]);
    if (values[i] != 0) {
      nnz[nnz_idx] = values[i];
      nnz_idx++;
    }
    else {
      continue;
    }
  }

  return nnz;

  free(nnz);
}

int* buildIntIA (int rows, int columns, int num_values, double *values) {
  int ia_len = rows + 1;
  int* ia = malloc(sizeof(int) * ia_len);
  ia[0] = 0;
  int pos_count = 0;
  int cell_count = 1;
  int row_count = 1;
  for (int i = 0; i < num_values; i++) {
    if (values[i] != 0) {
      // printf("%d\n", values[i]);
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

  free(ia);
}

int* buildIntJA (int columns, int rows, int nnz_len, double *values) {
  int *ja = malloc(nnz_len * sizeof(int));
  int column_count = 0;

  #pragma omp parallel
  {
    #pragma omp for
    for (int i = 0; i < nnz_len; i++) {
      if (values[i] != 0) {
        ja[i] = column_count;
        column_count++;
      }

      if (column_count == (columns - 1)) {
        column_count = 0;
      }

      column_count++;
    }
  }

  return ja;

  free(ja);
}


struct Matrix convertToCSR(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Could not open the file provided.\n");
    exit(EXIT_FAILURE);
  }

  else {
    char *line = NULL;
    int line_count = 0, non_zeroes = 0;
    size_t line_len = 0;
    int rows = 0, columns = 0, nnz_len = 0;
    double *nnz;
    int *ia, *ja;
    double *vals;
    struct Matrix matrix;

    while((getline(&line, &line_len, fp)) != -1) {
      line_count++;
      if (line_count == 1) {
        if (strstr(line, "int")) {
          matrix.is_float = false;
        }
        else if (strstr(line, "float")) {
          matrix.is_float = true;
        }
      }
      else if (line_count == 2) {
        rows = atoi(line);
      }
      else if (line_count == 3) {
        columns = atoi(line);
      }
      else if (line_count > 3) {
        char *value = strtok(line, " ");
        int j = 0, dimensions = columns * rows;
        double *matrix_vals = malloc(sizeof(double) * rows * columns);
        
        while (value != NULL) {
          if ((double) strtol(value, NULL, 10) != 0) {
            nnz_len++;
          }
          matrix_vals[j] = (double) strtol(value, NULL, 10);
          j++;
          value = strtok(NULL, " ");
        }
        nnz = buildIntNNZ(nnz_len, dimensions, matrix_vals);
        ia = buildIntIA(rows, columns, dimensions, matrix_vals);
        ja = buildIntJA(rows, columns, nnz_len, matrix_vals);
        vals = matrix_vals;
        non_zeroes = nnz_len;
      }
    }

    matrix.columns = columns;
    matrix.rows = rows;
    matrix.nnz = nnz;
    matrix.ia = ia;
    matrix.ja = ja;
    matrix.matrix_vals = vals;
    matrix.non_zeros = non_zeroes;
    matrix.filename = filename;

    fclose(fp);

    return matrix;

    free(nnz);
    free(ja);
    free(ia);
    free(vals);
    free(line);
  }
}

char* getResultsString(double *results, int dimensions, bool is_float) {

  char *res = malloc(10000000 * sizeof(char));

  res[0] = '\0';

  for (int i = 0; i < dimensions; i++) {
    char num_char[20];
    num_char[0] = '\0';
    if (is_float == 0) {
      sprintf(num_char, "%d ", (int) results[i]);
      strcat(res, num_char);
      num_char[0] = '\0';
      
    }

    else {
      sprintf(num_char, "%f ", results[i]);
      strcat(res, num_char);
      num_char[0] = '\0';
    }
  }

  return res;

  free(res);
}


void printSingleResult (struct Matrix m, char* op_type, char* op_name, int thread_num, double time_taken) {
  time_t t = time(NULL);
  struct tm now = *localtime(&t);
  
  char *date = malloc(sizeof(char) * 200);
  char *filename = malloc(sizeof(char) * 500);
  int dimensions = m.columns * m.rows;

  sprintf(date, "%d%d%d_%d%d", now.tm_mday, now.tm_mon + 1, now.tm_year + 1900, now.tm_hour, now.tm_min);

  sprintf(filename, "22412569_%s_%s.out", date, op_name);

  FILE *outfile = fopen(filename, "w");

  char* results = getResultsString(m.matrix_vals, dimensions, m.is_float);

  if (m.is_float == 0) {
    fprintf(outfile, "%s\n%s\n%d\n%s\n%d\n%d\n%s\n%f", op_type, m.filename, thread_num, "int", m.rows, m.columns, results, time_taken);
  }

  else {
    fprintf(outfile, "%s\n%s\n%d\n%s\n%d\n%d\n%s\n%f", op_type, m.filename, thread_num, "float", m.rows, m.columns, results, time_taken);
  }

  fclose(outfile);
  
  free(results);
  free(date);
  free(filename);
}

void printDoubleResult (struct Matrix a, struct Matrix b, struct Matrix result, char* op_type, char* op_name, int thread_num, double seconds) {
  time_t t = time(NULL);
  struct tm now = *localtime(&t);
  
  char *date = malloc(sizeof(char) * 200);
  char *filename = malloc(sizeof(char) * 500);
  int dimensions = result.columns * result.rows;

  sprintf(date, "%d%d%d_%d%d", now.tm_mday, now.tm_mon + 1, now.tm_year + 1900, now.tm_hour, now.tm_min);

  sprintf(filename, "22412569_%s_%s.out", date, op_name);

  FILE *outfile = fopen(filename, "a");

  char *results = getResultsString(result.matrix_vals, dimensions, result.is_float);

  if (result.is_float == 0) {
    fprintf(outfile, "%s\n%s\n%s\n%d\n%s\n%d\n%d\n%s\n%f", op_type, a.filename, b.filename, thread_num, "int", result.rows, result.columns, results, seconds);
  }

  else {
    fprintf(outfile, "%s\n%s\n%s\n%d\n%s\n%d\n%d\n%s\n%f", op_type, a.filename, b.filename, thread_num, "float", result.rows, result.columns, results, seconds);
  }

  fclose(outfile);
  
  free(results);
  free(date);
  free(filename);
}

void printTraceResult(struct Matrix m, double result, char* op_type, int num_threads, double time_taken) {
  time_t t = time(NULL);
  struct tm now = *localtime(&t);
  
  char *date = malloc(sizeof(char) * 200);
  char *filename = malloc(sizeof(char) * 500);
  int dimensions = m.columns * m.rows;

  sprintf(date, "%d%d%d_%d%d", now.tm_mday, now.tm_mon + 1, now.tm_year + 1900, now.tm_hour, now.tm_min);

  sprintf(filename, "22412569_%s_%s.out", date, op_type);

  FILE *outfile = fopen(filename, "a");

  if (m.is_float == 0) {
    int int_result = (int) result;
    fprintf(outfile, "%s\n%s\n%d\n%s\n%d\n%d\n%d\n%f", op_type, m.filename, num_threads, "int", m.rows, m.columns, int_result, time_taken);
  }

  else {
    fprintf(outfile, "%s\n%s\n%d\n%s\n%d\n%d\n%f\n%f", op_type, m.filename, num_threads, "float", m.rows, m.columns, result, time_taken);
  }

  fclose(outfile);

  free(date);
  free(filename);
}
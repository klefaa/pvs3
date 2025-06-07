#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void matrix_operations_seq(double **a, double **b, double **add, double **sub, double **mul, double **div, int rows, int cols) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            add[i][j] = a[i][j] + b[i][j];
            sub[i][j] = a[i][j] - b[i][j];
            mul[i][j] = a[i][j] * b[i][j];
            div[i][j] = (b[i][j] != 0.0) ? a[i][j] / b[i][j] : 0.0;
        }
}

double **allocate_matrix(int rows, int cols) {
    double **mat = malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++)
        mat[i] = malloc(cols * sizeof(double));
    return mat;
}

void free_matrix(double **mat, int rows) {
    for (int i = 0; i < rows; i++)
        free(mat[i]);
    free(mat);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    int rows = atoi(argv[1]);
    int cols = atoi(argv[1]);

    double **a = allocate_matrix(rows, cols);
    double **b = allocate_matrix(rows, cols);
    double **add = allocate_matrix(rows, cols);
    double **sub = allocate_matrix(rows, cols);
    double **mul = allocate_matrix(rows, cols);
    double **div = allocate_matrix(rows, cols);

    srand(time(NULL));
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            a[i][j] = (rand() % 1000) / 10.0;
            b[i][j] = (rand() % 1000) / 10.0;
        }

    clock_t start = clock();
    matrix_operations_seq(a, b, add, sub, mul, div, rows, cols);
    clock_t end = clock();

    printf("Операции завершены (последовательно).\n");
    printf("Время: %.4f секунд\n", (double)(end - start) / CLOCKS_PER_SEC);

    free_matrix(a, rows); free_matrix(b, rows);
    free_matrix(add, rows); free_matrix(sub, rows);
    free_matrix(mul, rows); free_matrix(div, rows);
    return 0;
}

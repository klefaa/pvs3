#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void elementwise_operations_seq(double *a, double *b, double *add, double *sub, double *mul, double *div, long n) {
    for (long i = 0; i < n; i++) {
        add[i] = a[i] + b[i];
        sub[i] = a[i] - b[i];
        mul[i] = a[i] * b[i];
        div[i] = (b[i] != 0.0) ? a[i] / b[i] : 0.0;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Использование: %s <размер_массивов>\n", argv[0]);
        return 1;
    }

    long n = atol(argv[1]);
    double *a = malloc(n * sizeof(double));
    double *b = malloc(n * sizeof(double));
    double *add = malloc(n * sizeof(double));
    double *sub = malloc(n * sizeof(double));
    double *mul = malloc(n * sizeof(double));
    double *div = malloc(n * sizeof(double));

    if (!a || !b || !add || !sub || !mul || !div) {
        printf("Ошибка выделения памяти\n");
        return 1;
    }

    srand(time(NULL));
    for (long i = 0; i < n; i++) {
        a[i] = (rand() % 1000) / 10.0;
        b[i] = (rand() % 1000) / 10.0;
    }

    clock_t start = clock();
    elementwise_operations_seq(a, b, add, sub, mul, div, n);
    clock_t end = clock();

    printf("Операции завершены (последовательно).\n");
    printf("Время: %.4f секунд\n", (double)(end - start) / CLOCKS_PER_SEC);

    free(a); free(b); free(add); free(sub); free(mul); free(div);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Использование: %s <количество_элементов>\n", argv[0]);
        return 1;
    }

    long n = atol(argv[1]);
    if (n <= 0) {
        printf("Ошибка: количество элементов должно быть положительным числом\n");
        return 1;
    }

    // Выделяем память под массив
    double *array = (double *)malloc(n * sizeof(double));
    if (array == NULL) {
        printf("Ошибка выделения памяти\n");
        return 1;
    }

    // Инициализируем генератор случайных чисел
    srand(time(NULL));
    for (long i = 0; i < n; i++) {
        array[i] = (double)(rand() % 1000) / 10.0;  // 0.0 до 99.9
    }

    // Засекаем время начала
    clock_t start = clock();

    // Вычисление суммы
    double sum = 0.0;
    for (long i = 0; i < n; i++) {
        sum += array[i];
    }

    // Засекаем время конца
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    // Вывод результатов
    printf("Сумма элементов: %.2f\n", sum);
    printf("Время выполнения: %.4f секунд\n", elapsed);

    free(array);
    return 0;
}

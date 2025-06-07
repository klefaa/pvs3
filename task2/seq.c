#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubble_sort(int* arr, int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (arr[j] > arr[j+1]) {
                int tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Введите количество элементов массива\n");
        return 0;
    }

    int N = atoi(argv[1]);
    int* arr = malloc(N * sizeof(int));
    srand(time(NULL));

    for (int i = 0; i < N; i++)
        arr[i] = rand() % 1000;

    clock_t start = clock();
    bubble_sort(arr, N);
    clock_t end = clock();

    printf("Последовательная пузырьковая сортировка завершена.\n");
    printf("Время выполнения: %.3f секунд\n", (double)(end - start) / CLOCKS_PER_SEC);

    free(arr);
    return 0;
}
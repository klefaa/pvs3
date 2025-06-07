#include <mpi.h>      // Используем MPI только ради таймера
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubble_sort(int* arr, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);  // инициализация MPI (только ради таймера)

    if (argc < 2) {
        printf("Введите количество элементов массива\n");
        MPI_Finalize();
        return 1;
    }

    int N = atoi(argv[1]);
    if (N < 100000) {
        printf("Размер массива должен быть больше 100000\n");
        MPI_Finalize();
        return 1;
    }

    int* arr = malloc(N * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < N; i++)
        arr[i] = rand() % 1000;

    double start = MPI_Wtime();
    bubble_sort(arr, N);
    double end = MPI_Wtime();

    printf("Последовательная сортировка завершена.\n");
    printf("Время выполнения: %.6f секунд\n", end - start);

    free(arr);
    MPI_Finalize();
    return 0;
}

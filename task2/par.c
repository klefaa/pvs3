#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void local_sort(int* arr, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
}

int main(int argc, char** argv) {
    int size, rank, N;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0) printf("Введите количество элементов массива\n");
        MPI_Finalize();
        return 0;
    }

    N = atoi(argv[1]);
    int chunk = N / size;
    int* data = NULL;
    int* local = malloc(chunk * sizeof(int));

    if (rank == 0) {
        data = malloc(N * sizeof(int));
        srand(time(NULL));
        for (int i = 0; i < N; i++) data[i] = rand() % 1000;
    }

    double start = MPI_Wtime();
    MPI_Scatter(data, chunk, MPI_INT, local, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    local_sort(local, chunk);

    int* sorted = NULL;
    if (rank == 0) sorted = malloc(N * sizeof(int));

    MPI_Gather(local, chunk, MPI_INT, sorted, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        local_sort(sorted, N);  // Финальная сортировка
        double end = MPI_Wtime();
        printf("Параллельная пузырьковая сортировка завершена.\n");
        printf("Время выполнения: %f секунд\n", end - start);
        free(sorted);
        free(data);
    }

    free(local);
    MPI_Finalize();
    return 0;
}
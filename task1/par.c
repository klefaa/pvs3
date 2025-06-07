#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    int size, rank, N;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc < 2) {
        if (rank == 0) printf("Введите количество элементов массива\n");
        MPI_Finalize();
        return 0;
    }

    N = atoi(argv[1]);
    int chunk = N / size;
    int* full_array = NULL;
    int* local_array = (int*)malloc(chunk * sizeof(int));
    srand(time(NULL) + rank);

    if (rank == 0) {
        full_array = (int*)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++)
            full_array[i] = rand() % 100;
    }

    MPI_Scatter(full_array, chunk, MPI_INT, local_array, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    start = MPI_Wtime();
    long local_sum = 0;
    for (int i = 0; i < chunk; i++) local_sum += local_array[i];

    long global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0) {
        printf("Сумма элементов массива: %ld\n", global_sum);
        printf("Время выполнения: %f секунд\n", end - start);
    }

    free(local_array);
    if (rank == 0) free(full_array);
    MPI_Finalize();
    return 0;
}
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    int rank, size, N = 0, ITER = 100;
    double total_time = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0) printf("Укажите размер массива как аргумент\n");
        MPI_Finalize();
        return 1;
    }

    N = atoi(argv[1]);
    if (N < size) {
        if (rank == 0) printf("Размер массива должен быть >= числу процессов\n");
        MPI_Finalize();
        return 1;
    }

    int base_chunk = N / size;
    int remainder = N % size;
    int* full_array = NULL;
    int* local_array = NULL;
    int* sendcounts = NULL;
    int* displs = NULL;

    int local_size = base_chunk + (rank < remainder ? 1 : 0);
    local_array = (int*)malloc(local_size * sizeof(int));

    if (rank == 0) {
        full_array = (int*)malloc(N * sizeof(int));
        sendcounts = (int*)malloc(size * sizeof(int));
        displs = (int*)malloc(size * sizeof(int));
        for (int i = 0, offset = 0; i < size; i++) {
            sendcounts[i] = base_chunk + (i < remainder ? 1 : 0);
            displs[i] = offset;
            offset += sendcounts[i];
        }
    }

    for (int iter = 0; iter < ITER; iter++) {
        if (rank == 0) {
            srand(time(NULL) + iter);
            for (int i = 0; i < N; i++) full_array[i] = rand() % 100;
        }

        MPI_Scatterv(full_array, sendcounts, displs, MPI_INT,
            local_array, local_size, MPI_INT, 0, MPI_COMM_WORLD);

        double start = MPI_Wtime();

        long local_sum = 0;
        for (int i = 0; i < local_size; i++) local_sum += local_array[i];

        long global_sum = 0;
        MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

        double end = MPI_Wtime();

        if (rank == 0) {
            total_time += (end - start);
            printf("Сумма элементов массива: %ld\n", global_sum);
            printf("Время выполнения: %f секунд\n", end - start);
        }
    }

    if (rank == 0) {
        printf("Среднее время выполнения за %d запусков: %f секунд\n", ITER, total_time / ITER);
    }

    free(local_array);
    if (rank == 0) {
        free(full_array);
        free(sendcounts);
        free(displs);
    }

    MPI_Finalize();
    return 0;
}

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubble_sort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int main(int argc, char** argv) {
    int rank, size, N;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0) printf("Укажите размер массива как аргумент\n");
        MPI_Finalize();
        return 1;
    }

    N = atoi(argv[1]);
    int chunk = N / size;
    int *data = NULL;
    int *local = malloc(chunk * sizeof(int));

    if (rank == 0) {
        data = malloc(N * sizeof(int));
        srand(time(NULL));
        for (int i = 0; i < N; i++)
            data[i] = rand() % 100000;
    }

    double start = MPI_Wtime();

    MPI_Scatter(data, chunk, MPI_INT, local, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    bubble_sort(local, chunk);

    int *neighbor = malloc(chunk * sizeof(int));

    for (int phase = 0; phase < size; phase++) {
        int partner = (phase % 2 == 0) ? (rank % 2 == 0 ? rank + 1 : rank - 1)
                                       : (rank % 2 == 0 ? rank - 1 : rank + 1);

        if (partner < 0 || partner >= size) {
            continue;
        }

        MPI_Sendrecv(local, chunk, MPI_INT, partner, 0,
                     neighbor, chunk, MPI_INT, partner, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int *temp = malloc(chunk * sizeof(int));
        int i = 0, j = 0, k = 0;

        if (rank < partner) {
            while (k < chunk && i < chunk && j < chunk) {
                temp[k++] = (local[i] <= neighbor[j]) ? local[i++] : neighbor[j++];
            }
            while (k < chunk && i < chunk) temp[k++] = local[i++];
            while (k < chunk && j < chunk) temp[k++] = neighbor[j++];
            for (int x = 0; x < chunk; x++) local[x] = temp[x];
        } else {
            while (k < chunk && i < chunk && j < chunk) {
                temp[k++] = (local[i] >= neighbor[j]) ? local[i++] : neighbor[j++];
            }
            while (k < chunk && i < chunk) temp[k++] = local[i++];
            while (k < chunk && j < chunk) temp[k++] = neighbor[j++];
            for (int x = 0; x < chunk; x++) local[x] = temp[x];
        }

        free(temp);
    }

    int *result = NULL;
    if (rank == 0) result = malloc(N * sizeof(int));

    MPI_Gather(local, chunk, MPI_INT, result, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0) {
        printf("Time spent: %f seconds\n", end - start);
        free(data);
        free(result);
    }

    free(local);
    free(neighbor);

    MPI_Finalize();
    return 0;
}

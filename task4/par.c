#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define IDX(i,j,N) ((i)*(N)+(j))

int main(int argc, char** argv) {
    int rank, size, N;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0) printf("Введите размерность матриц N (NxN)\n");
        MPI_Finalize();
        return 0;
    }

    N = atoi(argv[1]);
    int total = N * N;
    int chunk = total / size;
    int* A = NULL;
    int* B = NULL;
    int* local_A = malloc(chunk * sizeof(int));
    int* local_B = malloc(chunk * sizeof(int));
    int* res_add = malloc(chunk * sizeof(int));
    int* res_sub = malloc(chunk * sizeof(int));
    int* res_mul = malloc(chunk * sizeof(int));
    int* res_div = malloc(chunk * sizeof(int));

    if (rank == 0) {
        A = malloc(total * sizeof(int));
        B = malloc(total * sizeof(int));
        srand(time(NULL));
        for (int i = 0; i < total; i++) {
            A[i] = rand() % 100 + 1;
            B[i] = rand() % 100 + 1;
        }
    }

    double start = MPI_Wtime();
    MPI_Scatter(A, chunk, MPI_INT, local_A, chunk, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, chunk, MPI_INT, local_B, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < chunk; i++) {
        res_add[i] = local_A[i] + local_B[i];
        res_sub[i] = local_A[i] - local_B[i];
        res_mul[i] = local_A[i] * local_B[i];
        res_div[i] = local_A[i] / local_B[i];
    }
    double end = MPI_Wtime();

    if (rank == 0) {
        printf("Параллельные операции над матрицами завершены.\n");
        printf("Время выполнения: %f секунд\n", end - start);
    }

    free(local_A); free(local_B);
    free(res_add); free(res_sub); free(res_mul); free(res_div);
    if (rank == 0) { free(A); free(B); }

    MPI_Finalize();
    return 0;
}
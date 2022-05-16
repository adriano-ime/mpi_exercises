// Exercício 2: Dado uma matriz 4x4 de números inteiros, usar MPI_Scatter para distribuir cada linha
// da matriz para um nó. O número total de nós deve ser 4 e cada nó imprime a sua linha depois de 
// recebe-la.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int SIZE = 4;
    int ROOT_RANK = 0;
    int my_id, ierr, num_procs;
    ierr = MPI_Init(&argc, &argv);
      
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int matrix[SIZE][SIZE];
    int vec[SIZE];

    srand(my_id + 1);
    if (my_id == ROOT_RANK) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                matrix[i][j] = ((rand()%100) + 1)%10;
            }
        }
    }

    MPI_Scatter(&matrix, SIZE, MPI_INT, &vec, SIZE, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);

    printf("Linha do processo %d: \n", my_id);
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", vec[i]);
    }
    printf("\n");

    MPI_Finalize();

    return 0;
}
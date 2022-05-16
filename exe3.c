// Exercício 3: Cada nó inicializa um vetor de 4 inteiros. Usar MPI_Gather de forma que um nó
// receba os vetores dos 4 nós montando uma matriz 4x4. Este nó imprime a matriz recebida.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int SIZE = 4;
    int ROOT_RANK = 0;
    int my_id, root_process, ierr, num_procs;
    ierr = MPI_Init(&argc, &argv);
      
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if(num_procs != SIZE) {
        printf("A ideia desse exercicio é que se tenham 4 processadores \n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    // Inicializa vetores dos nós aleatoriamente
    int rand_vec[SIZE];
    srand(my_id + 1);
    for (int i = 0; i < SIZE; i++) {
        rand_vec[i] = ((rand()%100) + 1)%10;
    }

    // Realização do GATHER
    int matrix[SIZE][SIZE];
    MPI_Gather(&rand_vec, SIZE, MPI_INT, &matrix, SIZE, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);

    if (my_id == ROOT_RANK) {
        printf("Matriz recebida por nó mestre: \n");
        for (int i = 0; i < SIZE; i ++) {
            for (int j = 0; j < SIZE; j++) {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}
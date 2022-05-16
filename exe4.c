// Exercício 4: Fazer um programa paralelo para somar os elementos de um vetor de 1.000.000 de
// números. Usar a operação MPI_Scatter para distribuir o vetor e usar a operação
// MPI_Reduce para somar as somas parciais. Executar a versão sequencial e a
// versão paralela com 2, 4 e 8 processadores medindo o tempo de execução e o speedup.

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define TOTAL_ELEMENTS 1000000
// #define TEMPO_SEQUENCIAL 0.002100

long long int sum_elem_array(int *array, int num_elem) {
    long long int sum = 0;
    int i;
    for (i = 0; i < num_elem; i++) {
        sum += (long long int)array[i];
    }
    return sum;
}

int main(int argc, char *argv[]) {
    struct timeval init_time, end_time;
	long long int total_sum;
    int npes, rank;
    double TEMPO_SEQUENCIAL;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int array[TOTAL_ELEMENTS];
    int n_elem_proc = (TOTAL_ELEMENTS / npes);
    int sub_array_proc[n_elem_proc];

    if (rank == 0) {
        int i;
        for (i = 0; i < TOTAL_ELEMENTS; i++) {
            array[i] = i + 1;
        }
		gettimeofday(&init_time, NULL); 
    }
	
    MPI_Scatter(array, n_elem_proc, MPI_INT, sub_array_proc,
                n_elem_proc, MPI_INT, 0, MPI_COMM_WORLD);

    long long int process_sum;
    process_sum = sum_elem_array(sub_array_proc, n_elem_proc);
    MPI_Reduce(&process_sum, &total_sum, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        gettimeofday(&end_time, NULL);
        
        printf("%d processadores: \n\n", npes);
        printf("Soma: %lld \n", total_sum);
        double execution_time = end_time.tv_sec + end_time.tv_usec / 1e6 - (init_time.tv_sec + init_time.tv_usec / 1e6);

        gettimeofday(&init_time, NULL);
        int sum = 0;
        for (int i = 0; i < TOTAL_ELEMENTS; i++) {
            sum += array[i];
        }
        gettimeofday(&end_time, NULL);
        double TEMPO_SEQUENCIAL = end_time.tv_sec + end_time.tv_usec / 1e6 - (init_time.tv_sec + init_time.tv_usec / 1e6);

        printf("Tempo de Execução Sequencial: %fs\n", TEMPO_SEQUENCIAL);
        printf("Tempo de Execução Paralelo: %fs\n", execution_time);
        printf("Speedup: %f \n", TEMPO_SEQUENCIAL / execution_time);
    }

    MPI_Finalize();
    return 0;
}
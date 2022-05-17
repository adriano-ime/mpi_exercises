#include <mpi.h>
#include <stdio.h>
 
int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);
    int total_process, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &total_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	int colors[3];
	int color;
	char * color_map[3];
    if(rank == 0){
		int i;
		for(i = 0; i < 3; ++i)
			colors[i] = i;
		color_map[0] = "Branco";
		color_map[1] = "Vermelho";
		color_map[2] = "Verde";
    }

    MPI_Bcast(&colors, 3, MPI_INT, 0, MPI_COMM_WORLD);
    
    if(rank != 0){
		//Verde
        color = 2;
        MPI_Send(&color, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
    }
    else{
        for(int i = 1; i < total_process; i++){
            int c;
            MPI_Status status;
            MPI_Recv(&c, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Cor do processo %d: ", i);
            printf("%s\n", color_map[c]);
            printf("\n");
        }
    }
    
    if(rank != 0){
		color = (rank & 1);
        MPI_Send(&color, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
    }
    else{

        for(int i = 1; i < total_process; i++){
            int c;
            MPI_Status status;
            MPI_Recv(&c, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Nova cor do processo %d: ", i);
			printf("%s\n", color_map[c]);
            printf("\n");
        }
    }

    MPI_Finalize();
}
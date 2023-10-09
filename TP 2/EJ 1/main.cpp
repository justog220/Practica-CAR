#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <vector>
#include "modulos/funciones.h"

using namespace std;



int main(int argc, char **argv) {


    int ierror, rank, size, entrada = 8;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Paso por referencia rank y la modifica
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double tInicio, tFin;

    int megas = 400;
    int N = megas * 1024 * 1024 /sizeof(int);
    vector<int> mensaje(N, 0);
    if (rank == 0)
    {
        for (int i = 0; i < mensaje.size() ; i++)
            mensaje[i] = i;
    }
    if (rank == 0)
        printf("Broadcast,Tiempo,Nro Procesadores\n");

    // if(!rank) printf("N = %d", size);
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 0)
        tInicio = MPI_Wtime();

    My_BcastPtoPto(&mensaje[0], N, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        tFin = MPI_Wtime();
        printf("Punto a punto,%f,%d\n", tFin-tInicio, size);
    }
        
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 0)
        tInicio = MPI_Wtime();
    My_BcastTree(&mensaje[0], N, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank == 0)
    {
        tFin = MPI_Wtime();
        printf("Arbol,%f,%d\n", tFin-tInicio, size);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 0)
        tInicio = MPI_Wtime();
    MPI_Bcast(&mensaje[0], N, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank == 0)
    {
        tFin = MPI_Wtime();
        printf("MPICH,%f,%d\n", tFin-tInicio, size);
    }
    


    MPI_Finalize();


}
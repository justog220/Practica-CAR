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

    int mensaje = 8;
    if(rank == 0)
        mensaje = 10;

    My_BcastTree(&mensaje, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Soy %d y mi valor es %d\n", rank, mensaje);

    MPI_Finalize();


}
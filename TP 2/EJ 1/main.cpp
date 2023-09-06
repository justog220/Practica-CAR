#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char **argv) {

    freopen("salida.csv", "w", stdout);

    int ierror, rank, size, entrada = 8;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Paso por referencia rank y la modifica
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double tInicio, tFin;

    // Se define un vector de tamaño {megas}. 
    int megas = 300;
    int numElementos = megas * 1024 * 1024 / sizeof(int);
    vector<int> mensaje;
    mensaje.resize(numElementos);
    for (int i = 0 ; i < numElementos ; i++)
        mensaje[i] = i;

    
    MPI_Finalize();


}
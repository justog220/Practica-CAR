#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char **argv) {

    freopen("salida.csv", "w", stdout);

    int ierror, rank, size;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); (MPI_COMM_WORLD, &size);
    double tInicio, tFin;

    // Se define un vector de tamaño {megas}. 
    int megas = 300;
    int numElementos = megas * 1024 * 1024 / sizeof(int);
    vector<int> mensaje;
    mensaje.resize(numElementos);
    for (int i = 0 ; i < numElementos ; i++)
        mensaje[i] = i;

    if(rank == 0)
        printf("De,A,Tiempo\n");

    // Se define un número de repeticiones para representar mejor el caso
    for (int repeticiones = 0 ; repeticiones < 10 ; repeticiones++)
        for (int i = 0 ; i<size; i++) 
        {
            MPI_Barrier(MPI_COMM_WORLD); 
              
            if (rank == i)
            {
                // Se envía a todos los procesos y se miden los distintos tiempos \
                para luego conformar la matriz con todos los tiempos.
                for (int j = 0 ; j < size; j++)
                    if (i!=j)
                    {
                        tInicio = MPI_Wtime();
                        MPI_Send(&mensaje[0], numElementos, MPI_INT, j, 0, MPI_COMM_WORLD);
                        MPI_Recv(&tFin, 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, &status);
                        printf("%d,%d,%f\n", rank, j, tFin-tInicio);
                    }            
            }
            else
            {
                MPI_Recv(&mensaje[0], numElementos, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                tFin = MPI_Wtime();
 
                MPI_Send(&tFin, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }
        }


    MPI_Finalize();


}
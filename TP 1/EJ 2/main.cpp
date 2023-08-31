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

    int megas = 10;
    int numElementos = megas * 1024 * 1024 / sizeof(int);
    vector<int> mensaje;

    mensaje.resize(numElementos);

    for (int i = 0 ; i < numElementos ; i++)
        mensaje[i] = i;

    // std::cout<<"Declaró el vector\n";

    // int mensaje = 8;

    if(rank == 0)
        printf("De,A,Tiempo\n");

    for (int repeticiones = 0 ; repeticiones < 1 ; repeticiones++)
        for (int i = 0 ; i<size; i++) 
        {
            MPI_Barrier(MPI_COMM_WORLD); 
              
            if (rank == i)
            {
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
                // MPI_Barrier(MPI_COMM_WORLD);
                MPI_Recv(&mensaje[0], numElementos, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                tFin = MPI_Wtime();
 
                MPI_Send(&tFin, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }
            
            

        }


    MPI_Finalize();


}
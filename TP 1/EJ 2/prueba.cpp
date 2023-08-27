#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char **argv) {

    // freopen("salida.txt", "a", stdout);

    int ierror, rank, size, entrada = 8;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Paso por referencia rank y la modifica
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double tInicio, tFin;

    int megas = 500;
    int numElementos = megas * 1024 * 1024 / sizeof(int);
    vector<int> mensaje;

    mensaje.resize(numElementos);

    for (int i = 0 ; i < numElementos ; i++)
        mensaje[i] = i;

    // std::cout<<"Declaró el vector\n";

    // int mensaje = 8;

    uint nroIteraciones = 1;

    for (uint rep = 0 ; rep < nroIteraciones; rep ++)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        for (int i = 0 ; i < size ; i++)
        {
            if (rank == i)
            {
                for (int j = 0 ; j < size ; j++)
                {
                    if (j != rank)
                    {
                        // MPI_Barrier(MPI_COMM_WORLD);
                        tInicio = MPI_Wtime();
                        MPI_Send(&mensaje[0], numElementos, MPI_INT, j, 0, MPI_COMM_WORLD);
                        printf("%d envió a %d\n", i, j);

                        MPI_Recv(&tFin, 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        printf("%d;%d;%f\n", rank, j, tFin-tInicio);
                    }
                }
            } else 
            {
                // MPI_Barrier(MPI_COMM_WORLD);
                MPI_Recv(&mensaje[0], numElementos, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("%d recibió de %d\n", rank, i);

                tFin = MPI_Wtime();
                MPI_Send(&tFin, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }

        }
    }
    MPI_Finalize();


}
#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>

int main(int argc, char **argv) {
    
    freopen("salida.txt", "w", stdout);
    
    int ierror, rank, size;
    int mtag = 0;

    // Defino el máximo exponente al cual quiero que se eleve dos.
    int max = 30;
    double tiempo;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Paso por referencia rank y la modifica
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> vectorGrande;
    double tInicio, tFin;
    if(rank == 0)
        printf("Tiempo,Tamaño\n");

    // Implemento un ciclo para que se realice muchas veces y así \
    representar mejor
    for (int repeticion = 0 ; repeticion < 10 ; repeticion++)
        for(int i=0; i<=max; i++)
        {
            // Se calculo el número de elementos del vector
            unsigned long tamaño = pow(2, i);
            vectorGrande.resize(tamaño);
            for(int i = 0; i<tamaño; i++)
            {
                vectorGrande[i] = i;
            }
            
            // Si es el proceso 0 se envía al otro proceso y recibe el tiempo \
            de fin para luego imprimirlo.
            if(rank == 0)
            {
                MPI_Barrier(MPI_COMM_WORLD);
                tInicio = MPI_Wtime();
                MPI_Send(&vectorGrande[0], tamaño, MPI_INT, 1, mtag, MPI_COMM_WORLD);

                MPI_Recv(&tFin, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
                tiempo = tFin-tInicio;
                std::printf("%f,%ld\n", tiempo, tamaño);
            }
            else if (rank == 1)
            {
                // El segundo proceso recibe el vector, calcula el tiempo \
                y lo envía
                MPI_Barrier(MPI_COMM_WORLD);
                MPI_Recv(&vectorGrande[0], tamaño, MPI_INT, 0, mtag, MPI_COMM_WORLD, &status);
                tFin = MPI_Wtime();

                MPI_Send(&tFin, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            }
        }
    


    MPI_Finalize();

    return 0;
    
}
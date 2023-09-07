#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char **argv) {

    freopen("salida.csv", "w", stdout);

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double tInicio, tFin;
    int mtag = 0;

    // Define un arreglo de 350 MB
    int megas = 350;
    int numElementos = megas * 1024 * 1024 / sizeof(int);
    vector<int> paquete(numElementos);
    for(int i = 0 ; i < paquete.size() ; i++)
        paquete[i] = i;

    if(rank == 0)
        printf("Numero de procesadores,Tiempo\n");

    // Declaro un ciclo para representar mejor la situación
    int nroRepeticiones = 10;
    for(int i = 0; i < nroRepeticiones ; i++)
    {
        int mitad;
        int tamaño = 2;

        /* Implemento un ciclo hasta que una variable sea menor o igual al
        número de procesos */
        while (tamaño <= size)
        {
            // Calcula la mitad
            mitad = tamaño/2;
            MPI_Barrier(MPI_COMM_WORLD);
            if (rank == 0)
                tInicio = MPI_Wtime();
            if (rank <= tamaño-1)
            {   
                // Si es menor a la mitad envía y si no recibe.
                if(rank <= mitad-1)
                {
                // printf("\t%d envia a %d\n", rank, tamaño-rank-1);
                MPI_Send(&paquete[0], numElementos, MPI_INT, tamaño-rank-1, mtag, MPI_COMM_WORLD);
                } 
                else
                {
                // printf("\t%d recibe de %d\n", rank, tamaño-rank-1);
                MPI_Recv(&paquete[0], numElementos, MPI_INT, tamaño-rank-1, mtag, MPI_COMM_WORLD, &status);
                }

            }
            
            MPI_Barrier(MPI_COMM_WORLD);
            if(rank == 0)
            {
                tFin = MPI_Wtime();
                printf("%d,%f\n", tamaño, tFin-tInicio);
            }
            tamaño += 2;
        } 
    }

    
    MPI_Finalize();

    return 0;


}
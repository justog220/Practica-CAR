#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>

int main(int argc, char **argv) {
    
    freopen("salida.txt", "w", stdout);
    
    int ierror, rank, size;
    int mtag = 0;
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

    for (int repeticion = 0 ; repeticion < 10 ; repeticion++)
        for(int i=0; i<=max; i++)
        {
            unsigned long tamaño = pow(2, i);
            // printf("\n------------\nRepresentó %lld \n", tamaño);
            
            // int* vectorGrande = new int[tamaño];
            vectorGrande.resize(tamaño);
            for(int i = 0; i<tamaño; i++)
            {
                vectorGrande[i] = i;
            }
            
            // printf("Lo declaró \n");
            if(rank == 0)
            {
                MPI_Barrier(MPI_COMM_WORLD);
                tInicio = MPI_Wtime();
                MPI_Send(&vectorGrande[0], tamaño, MPI_INT, 1, mtag, MPI_COMM_WORLD);
                
                //printf("Tardó %f \n",tFin-tInicio);
                // std::cout<<"\t Tamaño = "<<sizeof(vectorGrande)<<std::endl;
                MPI_Recv(&tFin, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
                tiempo = tFin-tInicio;
                std::printf("%f,%ld\n", tiempo, tamaño);
            }
            else if (rank == 1)
            {
                MPI_Barrier(MPI_COMM_WORLD);
                MPI_Recv(&vectorGrande[0], tamaño, MPI_INT, 0, mtag, MPI_COMM_WORLD, &status);
                tFin = MPI_Wtime();

                MPI_Send(&tFin, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            }
            // delete [] vectorGrande;
        }
    


    MPI_Finalize();

    return 0;
    
}
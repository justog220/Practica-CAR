#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <vector>
#include <fstream>
// #include "modulos/funciones.h"

using namespace std;



int main(int argc, char **argv) {

    freopen("datos/salida.csv", "a", stdout);

    int ierror, rank, size, entrada = 8;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Paso por referencia rank y la modifica
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double valores[size];

    if(!rank)
    {
        ifstream archivo("datos/datosInit.txt");
        if(!archivo.is_open()) printf("\nAbrio\n");
        for(int i = 0; i<size ; i++)
            archivo>>valores[i];
    }

    
    MPI_Bcast(valores, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double valorMax = valores[rank];
    double rankMaxV = rank;

    double paqueteMax[2] = {valorMax, rankMaxV};
    double paqueteCompar[2];



    for(int paso = 1 ; paso < size ; paso *=2)
    {
        if(rank % (2*paso) == 0)
        {
            int rankCompar = rank + paso;
            if(rankCompar < size)
            {
                double valorCompar;
                // printf("%d va a recibir de %d\n", rank, rankCompar);
                MPI_Recv(&paqueteCompar, 2, MPI_DOUBLE, rankCompar, 0, MPI_COMM_WORLD, &status);
                if(paqueteCompar[0] >= paqueteMax[0])
                {
                    paqueteMax[0] = paqueteCompar[0];
                    paqueteMax[1] = paqueteCompar[1];
                    // printf("Nuevo valor maximo %f y rank %f en %d\n", paqueteMax[0], paqueteMax[1], rank);
                }
            }
        }
        else
        {
            int rankEnviar = rank - paso;
            if (rankEnviar >= 0)
            {
                // printf("%d va a enviar a %d\n", rank, rankEnviar);
                MPI_Send(&paqueteMax, 2, MPI_DOUBLE, rankEnviar, 0, MPI_COMM_WORLD);
            }
            
            
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    if (rank == 0)
    {
        // cout<<"El valor máximo es "<<valorMax<<" y está en "<<rankMaxV<<endl;
        printf("%f,%f\n", paqueteMax[0], paqueteMax[1]);
    }




    MPI_Finalize();


}
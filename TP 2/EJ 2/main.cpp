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

        // for(int i = 0; i<size ; i++)
        //     cout<<"\t"<<valores[i]<<endl;

    }
    
    MPI_Bcast(valores, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double valorMax = valores[rank];
    int rankMaxV = rank;

    // for(int i = 0 ; i < size ; i++)
    // {
    //     if(rank == i) cout<<"Soy "<<rank<<" y mi valor es "<<valores[rank]<<endl;
    //     MPI_Barrier(MPI_COMM_WORLD);
    // }

    for(int paso = 1 ; paso < size ; paso *=2)
    {
        // int rankCompar = rank + paso;

        if(rank % (2*paso) == 0)
        {
            int rankCompar = rank + paso;
            if(rankCompar < size)
            {
                double valorCompar;
                // printf("%d va a recibir de %d\n", rank, rankCompar);
                MPI_Recv(&valorCompar, 1, MPI_DOUBLE, rankCompar, 0, MPI_COMM_WORLD, &status);
                if(valorCompar >= valorMax)
                {
                    valorMax = valorCompar;
                    rankMaxV = rankCompar;
                }
            }
        }
        else
        {
            int rankEnviar = rank - paso;
            if (rankEnviar >= 0)
            {
                // printf("%d va a enviar a %d\n", rank, rankEnviar);
                MPI_Send(&valorMax, 1, MPI_DOUBLE, rankEnviar, 0, MPI_COMM_WORLD);
            }
            
            
        }
        // MPI_Barrier(MPI_COMM_WORLD);
    }

    if (rank == 0)
    {
        // cout<<"El valor máximo es "<<valorMax<<" y está en "<<rankMaxV<<endl;
        printf("%f,%d\n", valorMax, rankMaxV);
    }




    MPI_Finalize();


}
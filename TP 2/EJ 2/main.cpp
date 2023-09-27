#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <vector>
// #include "modulos/funciones.h"

using namespace std;



int main(int argc, char **argv) {


    int ierror, rank, size, entrada = 8;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Paso por referencia rank y la modifica
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double valores[size] = {1.1, 1.15, 1.05, 1.01, 1.12};

    // valores[rank] = (size - rank) * 1.5;
    

    double valorMax = valores[rank];
    int rankMaxV;

    for(int i = 0 ; i < size ; i++)
    {
        if(rank == i) cout<<"Soy "<<rank<<" y mi valor es "<<valores[rank]<<endl;
        MPI_Barrier(MPI_COMM_WORLD);
    }

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
                if(valorCompar > valorMax)
                {
                    valorMax = valorCompar;
                    rankMaxV = rankCompar+1;
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
        cout<<"El valor máximo es "<<valorMax<<" y está en "<<rankMaxV<<endl;
    }




    MPI_Finalize();


}
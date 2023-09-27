#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>
// #include "modulos/funciones.h"

using namespace std;

int esPrimo(int n)
{
    int m = int(sqrt(n));
    for (int j = 2 ; j <= m ; j++)
        if (!(n%j)) return 0;
    return 1;
}


int main(int argc, char **argv) {

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Paso por referencia rank y la modifica
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // int tope =1*pow(10, 7);

    // int n1s[size];
    // int n2s[size]; 
    // vector<int> tamaniosParticiones(size);

    // /*Quiero que particiones y envíe*/
    // if(!rank)
    // {
    //     int particion = 10;
    //     int tamProm = tope/size;

    //     int residuo = tope % size;

        
    //     for(int i = 0 ; i < size ; ++i) tamaniosParticiones[i] = tamProm; 

    //     for(int i = 0; i<residuo; ++i) ++tamaniosParticiones[i];
        
    // }
    // MPI_Bcast(&tamaniosParticiones[0], size, MPI_INT, 0, MPI_COMM_WORLD);

    // int n1 = 0;
    // for(int i = 0 ; i < rank ; ++i) n1 += tamaniosParticiones[i];
    // int n2 = n1+tamaniosParticiones[rank];

    


    // for(int i = 0 ; i < size ; ++i)
    // {
    //     if(rank == i)
    //     {
    //         printf("\nRank %d:\n\tArranca en: %d\n\tTermina en: %d\n", rank, n1, n2);
        
    //     }
    //     MPI_Barrier(MPI_COMM_WORLD);
    // }

    int maxExp = 2;
    int tope = 1*pow(10, maxExp);
    for(int i = 1 ; i < maxExp ; ++i)
    {
        int n2, primesh = 0 , primes = 0, chunk=1*pow(10, i), n1=rank*chunk;

        while(1)
        {
            n2 = n1 + chunk;
            for(int n = n1 ; n < n2 ; n++)
            {
                if(esPrimo(n)) primesh++;
            }
            MPI_Reduce(&primesh, &primes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
            n1 += size*chunk;
            if(n1>=tope) break;
            if(!rank) printf("pi(%d) = %d\n", n1, primes);
        }
    }
        


    MPI_Finalize();
}
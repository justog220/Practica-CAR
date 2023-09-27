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
    

    int n2, primesh=0, primes, chunk = pow(10, 5), n1 = rank*chunk;
    while(1)
    {        
        n2 = n1 + chunk;
        for(int n = n1 ; n < n2 ; n++)
        {
            if(esPrimo(n)) primesh++;
        }
        MPI_Reduce(&primesh, &primes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        n1 += size*chunk;
        if(!rank) printf("pi(%d) = %d\n", n1, primes);
    }
    
        


    MPI_Finalize();
}
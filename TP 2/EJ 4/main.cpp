#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>
// #include "modulos/funciones.h"

using namespace std;

int esPrimo(int n)
{
    if(n <= 1) return 0;
    
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
    
    double chunk;
    double chunks[5] = {pow(10, 3), pow(10, 4), pow(10, 5), pow(10, 6), 2*pow(10, 6)};
    double limite = pow(10, 7);

    double t1, tCalculo, tComm;
    if(!rank) printf("Procesador,tCalculo,tComm,Chunk\n");
    for (int ch = 0 ; ch < 5 ; ch++)
    {
        t1 = MPI_Wtime();
        chunk = chunks[ch];
        int n2=0, primesh=0, primes, n1=rank*chunk;
        
        vector<int> extremos;

        while (n1<limite)
        {
            n2 = n1 + chunk;
            extremos.push_back(n1);
            extremos.push_back(n2);
            n1 += size*chunk;
        }

        for(int i = 0 ; i < extremos.size() ; i=i+2)
        {
            n1 = extremos[i];
            n2 = extremos[i+1];
            for(int n = n1 ; n < n2 ; n++)
            {
                if(n >= limite) break;
                if(esPrimo(n)) primesh++;
            }
        }
        tCalculo = MPI_Wtime()-t1;

        t1 = MPI_Wtime();
        MPI_Reduce(&primesh, &primes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        tComm = MPI_Wtime()-t1;

        if(!rank) printf("Primos hasta %f con chunk = %f: %d\n", limite, chunk, primes);
        // printf("%d,%f,%f,%f\n", rank, tCalculo, tComm, chunk);
    }
    MPI_Finalize();
}
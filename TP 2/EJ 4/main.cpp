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
    
    float chunk = 10000;
    float chunks[5] = {pow(10, 3), pow(10, 4), pow(10, 5), pow(10, 6), 2*pow(10, 6)};
    for (int ch = 0 ; ch < 5 ; ch++)
    {
        chunk = chunks[ch];
        int n2=0, primesh=0, primes, n1=rank*chunk;
        int limite = 1000000;
        
        vector<vector<int>> extremos;

        while (n1<limite)
        {
            n2 = n1 + chunk;
            extremos.push_back({n1, n2});
            n1 += size*chunk;
        }

        for(int i = 0 ; i < extremos.size() ; i++)
        {
            n1 = extremos[i][0];
            n2 = extremos[i][1];
            for(int n = n1 ; n < n2 ; n++)
            {
                if(n >= limite) break;
                if(esPrimo(n)) primesh++;
            }
        }

        MPI_Reduce(&primesh, &primes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        if(!rank) printf("Primos hasta %d con chunk = %f: %d\n", limite, chunk, primes);
    }
    MPI_Finalize();
}
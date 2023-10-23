#include <stdio.h>
#include <iostream>
#include <vector>
#include <omp.h>
#include <cmath>
#include <ctime>

using namespace std;

int esPrimo(int n)
{
    if(n <= 1) return 0;
    
    int m = int(sqrt(n));
    
    for (int j = 2 ; j <= m ; j++)
        if (!(n%j)) return 0;
    return 1;
}


int main(int argc, char **argv) 
{
    freopen("salida.csv", "w", stdout);
    int limite = pow(10, 7), n=2, primes = 0, maxThreads = omp_get_max_threads()/2;
    double wt0, wt1;

    int nroChunks = 6;
    vector<int> chunks(nroChunks, 10);
    for(int i = 0 ; i < nroChunks ; i++) chunks[i] = pow(chunks[i], (i+2));

    printf("Hilos,Schedule,Chunk,Limite,Primes,Tiempo\n");

    int nroIteraciones = 5;

    for(int it = 0 ; it < 3 ; it++)
    {
        for(int hilos = 1 ; hilos <= maxThreads ; hilos++)
        {
            omp_set_num_threads(hilos);
            for(int i = 0 ; i < nroChunks ; i++)
            {
                primes = 0;
                int chunk = chunks[i];
                wt0 = omp_get_wtime();
                #pragma omp parallel default(none) shared(limite, chunk) private(n) reduction(+:primes) 
                {
                    #pragma omp for schedule(static,chunk)
                    for(n = 2; n <= limite ; n++)
                    {
                        if(esPrimo(n)) primes++;
                    }
                }
                wt1 = omp_get_wtime();
                printf("%d,Static,%d,%d,%d,%f\n",hilos,chunk,limite,primes,wt1-wt0);


                primes = 0;
                wt0 = omp_get_wtime();
                #pragma omp parallel default(none) shared(limite, chunk) private(n) reduction(+:primes) 
                {
                    #pragma omp for schedule(dynamic,chunk)
                    for(n = 2; n <= limite ; n++)
                    {
                        if(esPrimo(n)) primes++;
                    }
                }
                wt1 = omp_get_wtime();
                printf("%d,Dynamic,%d,%d,%d,%f\n",hilos,chunk,limite,primes,wt1-wt0);
            }
        }
    }

}
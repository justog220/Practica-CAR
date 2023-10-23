#include <stdio.h>
#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>

using namespace std;

int main(int argc, char **argv)
{
    freopen("salida.csv", "w", stdout);
    int num_iteraciones = 5;
    vector<int> lista_de_filas(num_iteraciones, 500);
    vector<int> lista_de_columnas(num_iteraciones, 500);
    for(int i = 0 ; i < lista_de_filas.size(); i++)
    {
        lista_de_filas[i] = lista_de_filas[i]+(i*2000);
        lista_de_columnas[i] = lista_de_columnas[i]+(i*2000);
    }
    int maxThreads = omp_get_max_threads()/2;
    printf("hilos,f,c,acceso,tiempo\n");

    omp_set_num_threads(2);

    for(int it = 0 ; it < num_iteraciones ; it ++)
    {
        for(int hilos = 1 ; hilos <= maxThreads ; hilos++)
        {   
            omp_set_num_threads(hilos);
            for(int iteracion = 0 ; iteracion < num_iteraciones ; iteracion++)
            {
                int filas = lista_de_filas[iteracion];
                int columnas = lista_de_columnas[iteracion];
    
                double wt0, wt1;

                

                vector<vector<int>> matriz(filas, vector<int>(columnas, 1));

                // Accediendo a filas
                int i = 0, j = 0, suma = 0;
                wt0 = omp_get_wtime();
                #pragma omp parallel default(none) shared(matriz) private(i, j) reduction(+:suma)
                {
                    // printf("%d\n", omp_get_num_threads());
                    #pragma omp for
                    for(i = 0 ; i < matriz.size(); i++)
                    {
                        for(j = 0 ; j < matriz[0].size(); j++)
                        {
                            suma += matriz[i][j];
                        }
                    }
                }
                wt1 = omp_get_wtime();
                printf("%d,%d,%d,Filas,%f\n",  hilos,filas, columnas, wt1-wt0);

                suma = 0;

                //Accediendo a columnas
                i = 0, j=0;
                wt0 = omp_get_wtime();
                #pragma omp parallel default(none) shared(matriz) private(i, j) reduction(+:suma)
                {
                    // printf("%d\n", omp_get_num_threads());
                    #pragma omp for
                    for(i = 0 ; i < matriz[0].size(); i++)
                    {
                        for(j = 0 ; j < matriz.size(); j++)
                        {
                            suma += matriz[j][i];
                        }
                    }
                }
                wt1 = omp_get_wtime();

                // printf("%d,%d,Columnas,%f\n", filas, columnas, double(t1-t0)/CLOCKS_PER_SEC);
                printf("%d,%d,%d,Columnas,%f\n", hilos, filas, columnas, wt1-wt0);

                // printf("%d,%d,ColumnasSec,%f\n", filas, columnas, double(t1-t0)/CLOCKS_PER_SEC);
            }
        }
    }

    
    
}
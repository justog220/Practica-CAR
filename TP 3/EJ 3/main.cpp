#include <stdio.h>
#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>

using namespace std;

int main(int argc, char **argv)
{
    freopen("salida.csv", "w", stdout);
    int num_iteraciones = 5, maxThreads = omp_get_max_threads()/2;
    vector<int> lista_de_filas(num_iteraciones, 500);
    vector<int> lista_de_columnas(num_iteraciones, 500);
    
    for(int i = 0 ; i < lista_de_filas.size(); i++)
    {
        lista_de_filas[i] = lista_de_filas[i]+(i*200);
        lista_de_columnas[i] = lista_de_columnas[i]+(i*200);
    }
    
    double t0, t1;

    printf("NroThreads,Filas,Columnas,Tiempo\n");
    for(int it = 0 ; it < 5 ; it ++)
    {
        for(int numThreads = 1 ; numThreads <= maxThreads ; numThreads++)
        {
            omp_set_num_threads(numThreads);
            for(int tamanio = 0 ; tamanio < lista_de_filas.size() ; tamanio++)
            {
                int filas = lista_de_filas[tamanio];
                int columnas = lista_de_columnas[tamanio];


                //Declaro la matriz
                vector<vector<int>> matrizA(filas, vector<int>(columnas, 0));
                vector<vector<int>> matrizB(filas, vector<int>(columnas, 0));
                vector<vector<int>> matrizResultante(filas, vector<int>(columnas, 0));


                //Relleno la matriz
                int f, c;
                #pragma omp parallel shared(matrizA, matrizB) private(f, c)
                {
                    #pragma omp for
                    for(f = 0 ; f < filas ; f++)
                    {
                        for(c = 0 ; c < columnas ; c++)
                        {
                            matrizA[f][c] = f+c+1;
                            matrizB[f][c] = (filas-f)+(columnas-c)+1;
                        }
                    }
                }


                int i, j, k;
                t0 = omp_get_wtime();
                #pragma omp parrallel default(none) shared(matrizA, matrizB, matrizResultante) private(i, j, k)
                {
                    #pragma omp for
                    for (i = 0; i < filas; i++)
                    {
                        for (j = 0; j < columnas; j++) 
                        {
                            for (k = 0; k < columnas; k++) 
                            {
                                matrizResultante[i][j] += matrizA[i][k] * matrizB[k][j];
                            }
                        }
                    }
                }
                t1 = omp_get_wtime();
                printf("%d,%d,%d,%f\n",numThreads,filas, columnas, t1-t0);


                // printf("Matriz A:\n");
                // for(int i = 0 ; i < filas ; i++)
                // {
                //     for(int j = 0 ; j < columnas ; j++)
                //     {
                //         cout<<matrizA[i][j]<<" ";
                //     }
                //     cout<<endl;
                // }

                // printf("Matriz B:\n");
                // for(int i = 0 ; i < filas ; i++)
                // {
                //     for(int j = 0 ; j < columnas ; j++)
                //     {
                //         cout<<matrizB[i][j]<<" ";
                //     }
                //     cout<<endl;
                // }

                // printf("Matriz Resultante:\n");
                // for(int i = 0 ; i < filas ; i++)
                // {
                //     for(int j = 0 ; j < columnas ; j++)
                //     {
                //         cout<<matrizResultante[i][j]<<" ";
                //     }
                //     cout<<endl;
                // }
            }
        }
    }

    


}
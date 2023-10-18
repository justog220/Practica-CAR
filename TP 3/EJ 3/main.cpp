#include <stdio.h>
#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>

using namespace std;

int main(int argc, char **argv)
{
    int num_iteraciones = 5, maxThreads = omp_get_max_threads();
    vector<int> lista_de_filas(num_iteraciones, 500);
    vector<int> lista_de_columnas(num_iteraciones, 500);
    
    for(int i = 0 ; i < lista_de_filas.size(); i++)
    {
        lista_de_filas[i] = lista_de_filas[i]+(i*2000);
        lista_de_columnas[i] = lista_de_columnas[i]+(i*2000);
    }
        
    bool seImprimio = false;
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



    }
    

}
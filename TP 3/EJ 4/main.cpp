#include <stdio.h>
#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>

using namespace std;

int main(int argc, char **argv)
{

    printf("NumThreads,Estrategia,Tiempo\n");
    float n1 = 0.0, n2 = 1.0, particiones = 900000, paso = n2/particiones;

    float area = 0.0;
    float n = 0.0;

    unsigned t0, t1;

    /*
    Declaro un vector de centros para que OpenMP pueda
    trabajar con el iterador entero.
    */

    int i;
    vector<float> centros(particiones);
    #pragma omp parrallel default(none) shared(centros) private(i)
    {
        #pragma omp for
        for(i = 0 ; i <= centros.size() ; i++)
        {
            centros[i] = n1 + paso*float(i);
        }
    }

    int num_iteraciones = 50; 

    for(int it = 0 ; it < num_iteraciones ; it++)
    {

        t0 = clock();
        #pragma omp parallel default(none) shared(centros, paso) private(i) reduction(+:area)
        {    
            #pragma omp for
            for(i = 0 ; i <= centros.size() ; i++)
            {
                area += (1.0/(1.0+(centros[i]*centros[i])))*paso;
            }
        }
        t1 = clock();
        printf(",Reduction,%f\n", double(t1-t0)/CLOCKS_PER_SEC);

        area = 0.0;
        t0 = clock();
        #pragma omp parallel default(none) shared(area,centros, paso) private(i)
        {    
            #pragma omp for
            for(i = 0 ; i <= centros.size() ; i++)
            {
                #pragma omp atomic
                area += (1.0/(1.0+(centros[i]*centros[i])))*paso;
            }
        }
        t1 = clock();
        printf(",Critical,%f\n",double(t1-t0)/CLOCKS_PER_SEC);

        area = 0.0;

        t0 = clock();
        #pragma omp parallel default(none) shared(area,centros, paso) private(i)
        {    
            #pragma omp for ordered
            for(i = 0 ; i <= centros.size() ; i++)
            {
                #pragma omp ordered
                {
                    area += (1.0/(1.0+(centros[i]*centros[i])))*paso;
                }
            }
        }
        t1 = clock();
        printf(",Ordered,%f\n", double(t1-t0)/CLOCKS_PER_SEC);

    }

    
}
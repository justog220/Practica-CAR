#include <stdio.h>
#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

int main(int argc, char **argv) {

    int componentes = 1000;
    vector<int> arreglo(componentes, 1);

    cout<<"Max threads antes de setear: "<<omp_get_max_threads()<<endl;
    int suma = 0;

    int i = 0;

    omp_set_num_threads(2);
    cout<<"Max threads después de setear: "<<omp_get_max_threads()<<endl;
    cout<<"Num afuera: "<<omp_get_num_threads()<<endl;
    #pragma omp parallel default(shared) private(i) reduction(+:suma)
    {
        if(omp_get_thread_num()==1)
            cout<<"Num dentro: "<<omp_get_num_threads()<<endl;
        #pragma omp for
        for(i = 0; i < arreglo.size(); i++)
            suma += arreglo[i];
    }
    
    printf("Suma = %d\n", suma);

    /*
    Cuando lo compilo con la bandera -fopenmp se va a compilar
    teniendo en cuenta las directivas codificadas. 

    Sino tengo el código en secuencial. 
    */
}
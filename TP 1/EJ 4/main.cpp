#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/// @brief Función auxiliar para pasar por referencia un fragmento \
    de secuencia y calcule el número de cada ocurrencia.
/// @param secuencia 
/// @return [nro A, nro T, nro C, nro G, nro otro]
vector<int> analizarSecuencia(string *secuencia)
{
    vector<int> recuento(5, 0);
    for (int i = 0 ; i < secuencia->length() ; i++)
    {
        switch (secuencia->at(i))
        {
        case 'A':
            recuento[0]++;
            break;
        case 'T':
            recuento[1]++;
            break;
        case 'C':
            recuento[2]++;
            break;
        case 'G':
            recuento[3]++;
            break;
        default:
            recuento[4]++;
            break;
        }
    }

    return recuento;
}

int main(int argc, char **argv) {
    
    int ierror, rank, size;
    int entrada = 8;
    int mtag = 0;

    freopen("salida.csv", "w", stdout);
    string ruta = "data/homo_sapiens_chromosome_1.fasta";
    
    uint largo;
    
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Paso por referencia rank y la modifica
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    vector<int> recuento;
    vector<int> recuentoTotal(5);

    /* Hago que el proceso 0 se encargue de cargar la secuencia,
    fragmentarla y enviar estos fragmentos a quién corresponda*/
    if (rank == 0)
    {
        ifstream archivo(ruta);
        string secuencia, linea;

        while(getline(archivo, linea))
        {
            secuencia += linea;
        }
        string fragmentoAnalisis;

        // Calcula el largo del fragmento.
        int largoSecuencia = secuencia.length();
        int largoFragmento = largoSecuencia / size;
        printf(largoSecuencia);
        for (int i = 0; i < size ; i++)
        {
            string fragmento;
            int posInicio = i * largoFragmento;
            int posFinal = (i == size - 1) ? largoSecuencia : (i+1) * largoFragmento;
            fragmento = secuencia.substr(posInicio, posFinal-posInicio);
            largo = fragmento.size();

            /* Envía primero el largo de la secuencia y luego el fragmento.
            Analizando el código en frío podría haber hecho un broadcast del largo.
            (o un Scatter, pero al momento de su realización no lo vimos)*/ 
            if (i!=rank)
            {
                MPI_Send(&largo, 1, MPI_INT, i, mtag, MPI_COMM_WORLD);
                MPI_Send(fragmento.c_str(), largo, MPI_CHAR, i, mtag, MPI_COMM_WORLD);
            }
            else
            {
                fragmentoAnalisis = fragmento;
            }
        }

        recuento = analizarSecuencia(&fragmentoAnalisis);

    } 
    else
    {
        int largo;
        MPI_Recv(&largo, 1, MPI_INT, 0, mtag, MPI_COMM_WORLD, &status);
        
        char *buf = new char[largo];
        MPI_Recv(buf, largo, MPI_CHAR, 0, mtag, MPI_COMM_WORLD, &status);
        string fragmento(buf, largo);
        delete [] buf;

        recuento = analizarSecuencia(&fragmento);
        
    }
    /* Luego de haber analizado los fragmentos con la función auxiliar
    hago un reduce para sumar los resultados*/
    MPI_Reduce(&recuento[0], &recuentoTotal[0], 5, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    /* Imprimo en formato csv para su posterior análisis */
    if (rank==0){
        printf("Base,Ocurrencia\n");
        printf("A,%d\n", recuentoTotal[0]);
        printf("T,%d\n", recuentoTotal[1]);
        printf("C,%d\n", recuentoTotal[2]);
        printf("G,%d\n", recuentoTotal[3]);
        printf("Otro,%d\n", recuentoTotal[4]);
    }

    MPI_Finalize();
    
}
#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

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
    // string ruta = "data/prueba.txt";
    
    uint largo;
    
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Paso por referencia rank y la modifica
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<int> recuento;
    vector<int> recuentoTotal(5);

    
    if (rank == 0)
    {
        ifstream archivo(ruta);
        string secuencia, linea;

        while(getline(archivo, linea))
        {
            secuencia += linea;
        }
        string fragmentoAnalisis;

        

        int largoSecuencia = secuencia.length();
        int largoFragmento = largoSecuencia / size;

        for (int i = 0; i < size ; i++)
        {
            string fragmento;
            int posInicio = i * largoFragmento;
            int posFinal = (i == size - 1) ? largoSecuencia : (i+1) * largoFragmento;
            fragmento = secuencia.substr(posInicio, posFinal-posInicio);
            largo = fragmento.size();

            if (i!=rank)
            {
                MPI_Send(&largo, 1, MPI_INT, i, mtag, MPI_COMM_WORLD);
                // cout<<"Se envio el largo "<<largo<<endl;
                // printf("Enviará a %d\n", i);
                MPI_Send(fragmento.c_str(), largo, MPI_CHAR, i, mtag, MPI_COMM_WORLD);
                // cout<<"Se envio el fragmento a "<<i<<endl;
            }
            else
            {
                fragmentoAnalisis = fragmento;
            }
        }

        recuento = analizarSecuencia(&fragmentoAnalisis);

        for (int i = 1; i < rank ; i++)
        {
            vector<int> bufRecuento;
            MPI_Recv(&bufRecuento, 1, MPI_INT, i, mtag, MPI_COMM_WORLD, &status);
        }
    } 
    else
    {
        int largo;
        MPI_Recv(&largo, 1, MPI_INT, 0, mtag, MPI_COMM_WORLD, &status);
        // printf("Obtuvo el largo %d\n", largo);
        
        char *buf = new char[largo];
        // printf("Se creo el buffer de largo %d\n", largo);
        MPI_Recv(buf, largo, MPI_CHAR, 0, mtag, MPI_COMM_WORLD, &status);
        // printf("Recibió el buffer\n");
        string fragmento(buf, largo);
        // printf("Se creo la string\n");
        // cout<<fragmento;
        delete [] buf;

        recuento = analizarSecuencia(&fragmento);
        
    }
    MPI_Reduce(&recuento[0], &recuentoTotal[0], 5, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank==0){
        // printf("\n\n\n");
        // for (int i = 0 ; i < recuentoTotal.size() ; i ++)
        //     printf("%d = %d\n", i+1, recuentoTotal[i]);

        printf("Base,Ocurrencia\n");
        printf("A,%d\n", recuentoTotal[0]);
        printf("T,%d\n", recuentoTotal[1]);
        printf("C,%d\n", recuentoTotal[2]);
        printf("G,%d\n", recuentoTotal[3]);
        printf("Otro,%d\n", recuentoTotal[4]);
    }

    MPI_Finalize();
    
}
#include "funciones.h"

void My_BcastPtoPto(void *sendbuff, int largo, MPI_Datatype tipoDeDato, int origen, MPI_Comm communicator)
{
    int tam;
    MPI_Comm_size(communicator, &tam);
    MPI_Status status;
    int mtag = 0;
    int rank;
    MPI_Comm_rank(communicator, &rank);
    if(rank == origen)
    {
        for(int i = 0 ; i < tam ; i++)
        {
            if(i!=origen)
                MPI_Send(sendbuff, largo, tipoDeDato, i, mtag, communicator);
        }
    }
    else
    {   
        MPI_Recv(sendbuff, largo, tipoDeDato, origen, mtag, communicator, &status);
    }
}

void My_BcastTree(void *sendbuff, int largo, MPI_Datatype tipoDeDato, int origen, MPI_Comm communicator)
{
    int tam;
    MPI_Comm_size(communicator, &tam);
    MPI_Status status;
    int mtag = 0;
    int rank;
    MPI_Comm_rank(communicator, &rank);

    int n1 = 0, n2 = tam;

    while(true)
    {
        int medio = (n1+n2)/2;
        if(rank == n1)
        {
            MPI_Send(sendbuff, largo, tipoDeDato, medio, mtag, communicator);
        }
        else if (rank == medio)
        {
            MPI_Recv(sendbuff, largo, tipoDeDato, n1, mtag, communicator, &status);
        }
        

        if (rank < medio)
        {
            n2 = medio;
        }
        else
        {
            n1 = medio;
        }

        if((n2-n1)==1)
        {
            break;
        }
        
    }
}


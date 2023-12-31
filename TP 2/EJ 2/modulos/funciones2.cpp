#include "funciones2.h"


void My_ReduceMin(const void *sendbuf, void *recvbuf, int largo, MPI_Datatype tipoDeDato, int destino, MPI_Comm communicator)
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
            MPI_Recv(sendbuff, largo, tipoDeDato, medio, mtag, communicator, &status);            
        }
        else if (rank == medio)
        {
            
            MPI_Send(sendbuff, largo, tipoDeDato, n1, mtag, communicator);
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

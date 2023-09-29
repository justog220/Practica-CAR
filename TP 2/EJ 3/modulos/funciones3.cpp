#include "funciones3.h"
#include <iostream>
#include <vector>

using namespace std;

void MostrarConMPI(const void *sendbuf, int sendcnt, MPI_Datatype tipoDeDatoEnv, void *recvbuf, const int *recvcnt, const int *displs, MPI_Datatype tipoDeDatoRecv, int raiz, MPI_Comm comm)
{
    int rank;
    MPI_Comm_rank(comm, &rank);
    MPI_Gatherv(sendbuf, sendcnt, tipoDeDatoEnv, recvbuf, recvcnt, displs, tipoDeDatoRecv, raiz, comm);
    // int tam;
    // MPI_Comm_size(comm, &tam);
    // if(rank == raiz)
    // {
    //     cout<<"Buffer:\n";
    //     for(int i = 0 ; i < tam ; ++i) cout<<"\t"<<recvbuf<<endl;
    // }
}

void MostrarSinMPI(double *sendbuf, int sendcnt, MPI_Datatype tipoDeDatoEnv, double *recvbuf, const int *recvcnt, const int *displs, MPI_Datatype tipoDeDatoRecv, int raiz, MPI_Comm comm)
{
    int rank, mtag = 0;
    MPI_Comm_rank(comm, &rank);
    if(rank == raiz)
    {
        int tam;
        MPI_Comm_size(comm, &tam);
        int cont = 0;
        vector<double> buffIntermedio;
        for(int i = 0 ; i < sendcnt ; i++) 
        {   
            recvbuf[i] = sendbuf[i];
            cont++;
        }

        // recvbuf[raiz] = buffIntermedio[raiz];
        for(int i = 0 ; i < tam ; i++)
        {
            if(i!=rank)
            {
                buffIntermedio.resize(recvcnt[i]);
                MPI_Recv(&buffIntermedio[0], recvcnt[i], tipoDeDatoRecv, i, mtag, comm, MPI_STATUS_IGNORE);
                for(int j = 0 ; j < recvcnt[i] ; j++) 
                {  
                    recvbuf[cont] = buffIntermedio[j];
                    cont++;
                }
            }
        }            
    }
    else
    {
        MPI_Send(sendbuf, sendcnt, tipoDeDatoEnv, raiz, mtag, comm);
    }
    
}

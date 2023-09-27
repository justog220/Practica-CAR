#include "funciones3.h"

void MostrarConMPI(const void *sendbuf, const void *sendcnt, MPI_Datatype tipoDeDatoEnv, void *recvbuf, const void *recvcnt, const void *displs, MPI_Datatype tipoDeDatoRecv, int raiz, MPI_Comm comm)
{
    MPI_Gatherv(sendbuf, sendcnt, sendtype, recv)
}
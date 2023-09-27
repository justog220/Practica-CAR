#ifndef FUNCIONES3_H
#define FUNCIONES3_H

#include <mpi.h>


/// @brief 
/// @param sendbuf 
/// @param sendcnt 
/// @param tipoDeDatoEnv 
/// @param recvbuf 
/// @param recvcnt 
/// @param displs 
/// @param tipoDeDatoRecv 
/// @param raiz 
/// @param comm 
void MostrarConMPI(const void *sendbuf, const void *sendcnt, MPI_Datatype tipoDeDatoEnv, void *recvbuf, const void *recvcnt, const void *displs, MPI_Datatype tipoDeDatoRecv, int raiz, MPI_Comm comm);

#endif
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
void MostrarConMPI(const void *sendbuf, int sendcnt, MPI_Datatype tipoDeDatoEnv, void *recvbuf, const int *recvcnt, const int *displs, MPI_Datatype tipoDeDatoRecv, int raiz, MPI_Comm comm);


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
void MostrarSinMPI(double *sendbuf, int sendcnt, MPI_Datatype tipoDeDatoEnv, double *recvbuf, const int *recvcnt, const int *displs, MPI_Datatype tipoDeDatoRecv, int raiz, MPI_Comm comm);


#endif

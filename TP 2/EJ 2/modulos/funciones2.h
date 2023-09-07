#ifndef FUNCIONES2_H
#define FUNCIONES2_H

#include <mpi.h>

void My_ReduceMin(const void *sendbuf, void *recvbuf, int largo, MPI_Datatype tipoDeDato, int destino, MPI_Comm communicator);

#endif
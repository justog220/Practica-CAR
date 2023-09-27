#ifndef FUNCIONES2_H
#define FUNCIONES2_H

#include <mpi.h>

/// @brief Busca el valor máximo de una variable contenida en todos los procesos y lo devuelve junto con el proceso que lo contiene
/// @param sendbuf Variable cuto valor máximo se desea encontrar
/// @param recvbuff
/// @param largo 
/// @param tipoDeDato 
/// @param destino 
/// @param communicator 
void My_ReduceMin(const void *sendbuf, void *recvbuf, int largo, MPI_Datatype tipoDeDato, int destino, MPI_Comm communicator);

#endif
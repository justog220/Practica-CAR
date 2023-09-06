#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <mpi.h>

/// @brief Función que permite hacer el broadcast con comunicación punto a punto
/// @param sendbuff Dirección del buffer a enviar
/// @param largo Largo del buffer a enviar
/// @param tipoDeDato Tipo de dato a enviar
/// @param origen Rank del proceso que tiene el dato
/// @param communicator Communicator de MPI dentro del cual se da la comunciación
void My_BcastPtoPto(void *sendbuff, int largo, MPI_Datatype tipoDeDato, int origen, MPI_Comm communicator);

void My_BcasTree();

#endif
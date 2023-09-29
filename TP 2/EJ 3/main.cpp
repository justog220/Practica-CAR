#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <vector>
#include "modulos/funciones3.h"

using namespace std;



int main(int argc, char **argv) {


    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Paso por referencia rank y la modifica
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sendcnt = rank+1;
    double *sbuff = new double[rank+1];
    for(int j = 0 ; j < sendcnt ; j++) sbuff[j] = rank*1000+j;

    int rsize = size*(size+1)/2;
    int *recvcnts = NULL;
    int *displs = NULL;
    double *rbuff = NULL;
    
    if (!rank) {
        rbuff = new double[rsize]; 
        recvcnts = new int[size];
        displs = new int[size];
        for (int j=0; j<size; j++) recvcnts[j] = (j+1);
        displs[0]=0;
        for (int j=1; j<size; j++)
            displs[j] = displs[j-1] + recvcnts[j-1];
    }

    if(!rank) printf("Cada uno va a tener:\n");


    for(int i = 0 ; i < size ; i++)
    {
        if(rank == i)
        {   
            printf("Rank: %d\n",i);
            for(int i = 0 ; i < rank+1 ; i++) cout<<"\t"<<sbuff[i]<<endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    if(!rank) printf("\n----------\n");
    MPI_Barrier(MPI_COMM_WORLD);

    MostrarSinMPI(sbuff, sendcnt, MPI_DOUBLE, rbuff, recvcnts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    
    if(!rank)
    {
        printf("Luego del gather vectorizado propio:\n");
        for(int i = 0 ; i < rsize ; i++) cout<<"\t"<<rbuff[i]<<endl;
    }

    double *rbuff2 = NULL;
    if(!rank) rbuff2 = new double[rsize]; 

    MPI_Barrier(MPI_COMM_WORLD);
    
    MostrarConMPI(sbuff, sendcnt, MPI_DOUBLE, rbuff2, recvcnts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if(!rank)
    {
        printf("\n----------\n");
        printf("Luego del gather vectorizado de MPI:\n");
        for(int i = 0 ; i < rsize ; i++) cout<<"\t"<<rbuff2[i]<<endl;
    }

    MPI_Finalize();


}
#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char **argv) {

    // freopen("salida.txt", "a", stdout);

    int ierror, rank, size, entrada = 8;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Paso por referencia rank y la modifica
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double tInicio, tFin;

    int megas = 300;
    int numElementos = megas * 1024 * 1024 / sizeof(int);
    vector<int> mensaje;

    mensaje.resize(numElementos);

    for (int i = 0 ; i < numElementos ; i++)
        mensaje[i] = i;

    // std::cout<<"Declaró el vector\n";

    // int mensaje = 8;

    for (int repeticiones = 0 ; repeticiones < 1 ; repeticiones++)
        for (int i = 0 ; i<size; i++) 
        {
            std::cout<<"I = "<<i<<" | rank = "<<rank<<endl;
            bool envio = false;
              
            if (rank == i)
            {   printf("%d llego a la barrera\n", rank);  
                MPI_Barrier(MPI_COMM_WORLD); 
                printf("%d paso la barrera\n", rank);  
                // tInicio = MPI_Wtime();        
                for (int j = 0 ; j < size; j++)
                    if (i!=j){
                        MPI_Send(&mensaje[0], numElementos, MPI_INT, j, 0, MPI_COMM_WORLD);
                        printf("%d envió %d elemento", rank, numElementos);
                        // MPI_Send(&mensaje, 1, MPI_INT, j, 0, MPI_COMM_WORLD);
                        envio = true;
                        printf("%d envió\n", i);
                    } 
                        
            } 

            if(envio)
            {
                std::cout<<"Van a entrar al segundo ciclo\n";
                for (int j = 0 ; j<size; j++)
                {
                    if(j!= i and rank==j)
                    {
                        printf("%d está esperando %d elementos", rank, numElementos);
                        MPI_Recv(&mensaje[0], numElementos, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                        // MPI_Recv(&mensaje, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                        printf("%d recibió\n", rank);
                        // tFin = MPI_Wtime();
                        printf("De %d a %d tardó: %f\n", i, j, tFin-tInicio);    
                    }

                }
            }
            


        }


    MPI_Finalize();


}
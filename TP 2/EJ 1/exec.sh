#!/bin/bash
mpicxx.mpich -o main.bin main.cpp modulos/funciones.cpp
for i in 2 4 8 16 32
do
    mpiexec.mpich -np $i ./main.bin
done

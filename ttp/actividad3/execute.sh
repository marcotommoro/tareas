#!/bin/bash
# Script para ejecutar el programa


13.19;4,710793E-11

# Execute the programs with diferentes valores de N
for N in 1000 2000 3000
do
    echo "Ejecutando con N = $N"
    ./ex_escalar $N $N
    echo ""
done

# Execute the programs with diferentes valores de N
for N in 1000 2000 3000
do
    echo "Ejecutando con N = $N"
    ./ex_bloques $N $N
    for TAM in 2 4 8 16 24 48
    do
        OPM_NUM_THREADS=$TAM ./ex_bloques_opt $N $N
    done
    echo ""
done
#!/bin/bash

for N in 1 2 4 8 16 24 48
do
       OMP_SCHEDULE=dynamic OMP_NUM_THREADS=$N ./ex 20000 20000
done
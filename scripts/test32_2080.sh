#!/bin/bash
#SBATCH --job-name=fr_2080_32
#SBATCH --output=mpi_2080_32.out
#SBATCH --error=mpi_pi_2080_32.err
#SBATCH --ntasks=32
#SBATCH -x lab2p[1-20]
srun ./foxes-rabbits 20 80000 10000 1000000 120000000 3 20000000 5 10 101010
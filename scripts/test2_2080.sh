#!/bin/bash
#SBATCH --job-name=fr_2080_2
#SBATCH --output=mpi_2080_2.out
#SBATCH --error=mpi_pi_2080_2.err
#SBATCH --ntasks=2
#SBATCH -x lab2p[1-20]
srun ./foxes-rabbits 20 80000 10000 1000000 120000000 3 20000000 5 10 101010
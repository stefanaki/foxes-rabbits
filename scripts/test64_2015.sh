#!/bin/bash
#SBATCH --job-name=fr_2015_64
#SBATCH --output=mpi_2015_64.out
#SBATCH --error=mpi_pi_2015_64.err
#SBATCH --ntasks=64
#SBATCH -x lab2p[1-20]
srun ./foxes-rabbits 20 15000 70000 1000000 300000000 3 50000000 5 10 101010
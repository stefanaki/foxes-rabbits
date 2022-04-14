#include "mpi-implementation.h"

#define MASTER_RANK 0

uint32_t generations;
uint32_t M;
uint32_t N;
uint32_t n_rocks;
uint32_t n_rabbits;
uint32_t n_foxes;
uint32_t rabbit_breeding;
uint32_t fox_breeding;
uint32_t fox_starvation;
uint32_t seed;

// Print result

int main(int argc, char **argv)
{
    // World world = (World){};
    // double exec_time;
    int rank, num_procs;
    // foxes-rabbits <# generations> <M> <N> <# rocks> <# rabbits>
    //  <rabbit breeding> <# foxes> <fox breeding> <fox starvation> <seed>

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // example of initial args
    // 10 5 5 10 3 6 2 6 8 10
    generations = atoi(argv[1]);
    M = atoi(argv[2]);
    N = atoi(argv[3]);
    n_rocks = atoi(argv[4]);
    n_rabbits = atoi(argv[5]);
    rabbit_breeding = atoi(argv[6]);
    n_foxes = atoi(argv[7]);
    fox_breeding = atoi(argv[8]);
    fox_starvation = atoi(argv[9]);
    seed = atoi(argv[10]);

    // define the debug variable for internal tests
    // debug = (argc == 12) ? atoi(argv[11]) : 0;

    generate_world_subgrid(rank, num_procs);

    // // Printing board
    // if (debug) {
    //   print_board(&world, -1, 0);
    // }

    MPI_Finalize();
    return 0;
}
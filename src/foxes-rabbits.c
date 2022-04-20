#include "message-cell.h"
#include "mpi-implementation.h"
#include "stdio.h"

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

int main(int argc, char **argv) {
    // World world = (World){};
    // double exec_time;
    int rank, num_procs;
    // foxes-rabbits <# generations> <M> <N> <# rocks> <# rabbits>
    //  <rabbit breeding> <# foxes> <fox breeding> <fox starvation> <seed>

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // MPI types
    MPI_Datatype animal_dt;
    int animal_blocklen[4] = {1, 1, 1, 1};
    MPI_Aint animal_disp[4] = {
        offsetof(struct Animal, type),
        offsetof(struct Animal, starvation_age),
        offsetof(struct Animal, breeding_age),
        offsetof(struct Animal, modified_by_red)};
    MPI_Datatype animal_datatypes[4] = {MPI_CHAR, MPI_UINT16_T, MPI_UINT16_T, MPI_C_BOOL};
    MPI_Type_create_struct(4, animal_blocklen, animal_disp, animal_datatypes, &animal_dt);
    MPI_Type_commit(&animal_dt);

    MPI_Datatype message_cell_dt;
    int message_cell_blocklen[4] = {4, 1, 1, 1};
    MPI_Aint message_cell_disp[4] = {
        offsetof(struct MessageCell, incoming_animals),
        offsetof(struct MessageCell, animal),
        offsetof(struct MessageCell, new_animals),
        offsetof(struct MessageCell, type)};
    MPI_Datatype message_cell_datatypes[4] = {animal_dt, animal_dt, MPI_INT, MPI_CHAR};
    MPI_Type_create_struct(4, message_cell_blocklen, message_cell_disp, message_cell_datatypes, &message_cell_dt);
    MPI_Type_commit(&message_cell_dt);

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

    int global_sum[3] = {0, 0, 0};

    Cell **subgrid = generate_world_subgrid(rank, num_procs);

    mpi_implementation(subgrid, rank, num_procs, message_cell_dt, global_sum);

    // // Printing board
    // if (debug) {
    //   print_board(&world, -1, 0);
    // }

    if (rank == MASTER_RANK) {
        printf("%d %d %d\n", global_sum[0], global_sum[1], global_sum[2]);
        fflush(stdout);
    }

    MPI_Type_free(&message_cell_dt);
    MPI_Type_free(&animal_dt);

    MPI_Finalize();
    return 0;
}
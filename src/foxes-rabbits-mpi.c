#include "cell.h"
#include "mpi.h"
#include "world.h"
#include "worldgen.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define RANK_MASTER 0

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

void print_board(Cell **grid, int chunk) {
  for (int l = 0; l <= N; l++)
    printf("---");
  printf("\n   ");
  for (int l = 0; l < N; l++)
    printf("%02d|", l);
  for (int k = 0; k < chunk; k++) {
    printf("\n");
    printf("0%d:", k);
    for (int l = 0; l < N; l++) {
      if (grid[k][l].type == ANIMAL && grid[k][l].animal->type == FOX)
        printf(" F");
      // printf("%d%d", world->grid[k][l].animal->breeding_age,
      // world->grid[k][l].animal->starvation_age);
      else if (grid[k][l].type == ANIMAL && grid[k][l].animal->type == RABBIT)
        printf(" R");
      //  printf("0%d", world->grid[k][l].animal->breeding_age);
      else if (grid[k][l].type == ROCK)
        printf(" *");
      else
        printf("  ");
      printf("|");
    }
  }
  printf("\n");
  for (int l = 0; l <= N; l++)
    printf("---");
  printf("\n");
}

void generate_element_mpi(int n, char atype, uint32_t *seed, Cell **grid,
                          int rank, int chunk) {
  int i, j, k;

  for (k = 0; k < n; k++) {
    i = M * r4_uni(seed);
    j = N * r4_uni(seed);

    int element_rank = i / chunk;
    int i_local = i - rank * chunk;

    if (rank == element_rank && position_empty(&grid[i_local][j])) {
      insert_element(&grid[i_local][j], atype);
    }
  }
}

// Generate world subgrid based on process id
Cell **generate_world_subgrid(int rank, int procs) {
  Cell **subgrid;
  int chunk, chunk_board, parity_rows;

  chunk_board = M / procs;
  parity_rows = M - chunk_board * (procs - 1);

  if (parity_rows && rank == procs - 1) {
    chunk = parity_rows;
  } else {
    chunk = chunk_board;
  }

  subgrid = (Cell *)malloc(sizeof(Cell) * chunk);
  for (int i = 0; i < M; ++i) {
    subgrid[i] = (Cell *)malloc(sizeof(Cell) * N);
  }

  for (int i = 0; i < chunk; ++i) {
    for (int j = 0; j < N; ++j) {
      modify_cell(&subgrid[i][j], EMPTY, NULL);
      subgrid[i][j].new_animals = 0;
    }
  }

  generate_element_mpi(n_rocks, ROCK, &seed, subgrid, rank, chunk_board);
  generate_element_mpi(n_rabbits, RABBIT, &seed, subgrid, rank, chunk_board);
  generate_element_mpi(n_foxes, FOX, &seed, subgrid, rank, chunk_board);

  printf("I am process %d. My allocated rows are:\n", rank);
  print_board(subgrid, chunk);
  return subgrid;
}

// Ask for data from cell that doesn't belong in the same process
// Modify cell that doesn't belong to the same process
// Implementation and process reduction
// Print result

int main(int argc, char **argv) {
  World world = (World){};
  double exec_time;
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
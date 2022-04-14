#include "mpi-implementation.h"
#include "mpi.h"
#include <stdio.h>

#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define BLOCK_HIGH(id, p, n) (BLOCK_LOW((id) + 1, p, n) - 1)
#define BLOCK_SIZE(id, p, n) (BLOCK_HIGH(id, p, n) - BLOCK_LOW(id, p, n) + 1)
#define BLOCK_OWNER(j, p, n) (((p) * ((j) + 1) - 1) / (n))

#define MASTER_RANK 0
#define ROW_SEND_PREV 123
#define ROW_SEND_NEXT 456

extern uint32_t generations;
extern uint32_t M;
extern uint32_t N;
extern uint32_t n_rocks;
extern uint32_t n_rabbits;
extern uint32_t n_foxes;
extern uint32_t rabbit_breeding;
extern uint32_t fox_breeding;
extern uint32_t fox_starvation;
extern uint32_t seed;

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
                          int rank, int procs) {
  int i, j, k, local_i;

  for (k = 0; k < n; k++) {
    i = M * r4_uni(seed);
    j = N * r4_uni(seed);

    int size = BLOCK_SIZE(rank, procs, M);
    for (int l = 0; l < size; l++) {
      local_i = i - BLOCK_LOW(rank, procs, M);
    }

    int element_rank = BLOCK_OWNER(i, procs, M);

    if (rank == element_rank && position_empty(&grid[local_i][j])) {
      insert_element(&grid[local_i][j], atype);
    }
  }
}

// Generate world subgrid based on process id
Cell **generate_world_subgrid(int rank, int procs) {
  Cell **subgrid;

  int chunk = BLOCK_SIZE(rank, procs, M);

  subgrid = (Cell **)malloc(sizeof(Cell) * chunk);
  for (int i = 0; i < M; ++i) {
    subgrid[i] = (Cell *)malloc(sizeof(Cell) * N);
  }

  for (int i = 0; i < chunk; ++i) {
    for (int j = 0; j < N; ++j) {
      modify_cell(&subgrid[i][j], EMPTY, NULL);
      subgrid[i][j].new_animals = 0;
    }
  }

  generate_element_mpi(n_rocks, ROCK, &seed, subgrid, rank, procs);
  generate_element_mpi(n_rabbits, RABBIT, &seed, subgrid, rank, procs);
  generate_element_mpi(n_foxes, FOX, &seed, subgrid, rank, procs);

  printf("I am process %d. My allocated rows are:\n", rank);
  print_board(subgrid, chunk);

  return subgrid;
}

bool fox_sees_rabbit(Cell *cell, char type) {
  return type == FOX && cell->animal && animal_type(cell->animal, RABBIT);
}
// Compute next position modified
void compute_next_position(Cell **grid, int i, int j, char animal_type,
                           int rank, int procs, Cell *upper_row,
                           Cell *bottom_row, int *landing_pos) {
  int C = i * N + j;
  int p = 0;
  int rabbit_p = 0;
  int available_cells[4][2];
  int available_rabbit_cells[4][2];
  int gi, size;
  Cell c[4];
  bool c_exists[4] = {false, false, false, false};

  gi = BLOCK_LOW(rank, procs, M) + i;

  if (gi - 1 >= 0) {
    c[0] =
        BLOCK_OWNER(gi - 1, procs, M) == rank ? grid[i - 1][j] : upper_row[j];
    c_exists[0] = true;
  }
  if (j + 1 <= N - 1) {
    c[1] = grid[i][j + 1];
    c_exists[1] = true;
  }
  if (gi + 1 <= M - 1) {
    c[2] =
        BLOCK_OWNER(gi + 1, procs, M) == rank ? grid[i + 1][j] : bottom_row[j];
    c_exists[2] = true;
  }
  if (j - 1 >= 0) {
    c[3] = grid[i][j - 1];
    c_exists[3] = true;
  }

  for (int k = 0; k < 4; ++k) {
    if (!c_exists[k])
      continue;

    if (c[k].type == EMPTY) {
      available_cells[p][0] = k == 0 ? gi - 1 : k == 2 ? gi + 1 : gi;
      available_cells[p][1] = k == 1 ? j + 1 : k == 3 ? j - 1 : j;
      p++;
    } else if (fox_sees_rabbit(&c[k], animal_type)) {
      available_rabbit_cells[rabbit_p][0] = k == 0   ? gi - 1
                                            : k == 2 ? gi + 1
                                                     : gi;
      available_rabbit_cells[rabbit_p][1] = k == 1 ? j + 1 : k == 3 ? j - 1 : j;
      rabbit_p++;
    }
  }

  if (p == 0 && rabbit_p == 0) {
    landing_pos[0] = -1;
    landing_pos[1] = -1;
    return;
  }

  int n = rabbit_p > 0 ? rabbit_p : p;
  int res = C % n;

  if (rabbit_p > 0) {
    landing_pos[0] = available_rabbit_cells[res][0];
    landing_pos[1] = available_rabbit_cells[res][1];
  } else {
    landing_pos[0] = available_cells[res][0];
    landing_pos[1] = available_cells[res][1];
  }
  return;
}

// Ask for data from cell that doesn't belong in the same process
Cell get_cell_from_rank(int rank, int i, int j) {
  return *(Cell *)malloc(sizeof(Cell) * N);
}

// Modify cell that doesn't belong to the same process
void modify_cell_from_rank(int rank, int i, int j, Cell data) { return; }

// Resolve conflicts modified
void resolve_conflicts(Cell *cell) { return; }

// Send generation result to master
void send_result_to_master(Cell **grid, int rank, int procs, int totals[]) {
  return;
}

// Implementation and process reduction
void mpi_implementation(Cell **grid, int rank, int procs) {
  bool col_offset;
  int i, j, gen, turn;
  int landing_pos[2];
  int total_elements[3] = {0, 0, 0}; // foxes, rabbits, rocks

  Cell recv_rows[2][N];
  Cell sent_rows[2][N];

  MPI_Request requests[4];
  MPI_Status statuses[4];

  for (gen = 0; gen < generations; ++gen) {
    for (turn = 0; turn < 2; ++turn) {
      // Receive previous and next rows
      if (rank - 1 >= 0) {
        MPI_IRecv(recv_rows[0], N * sizeof(Cell), MPI_BYTE, rank - 1,
                  ROW_SEND_PREV, MPI_COMM_WORLD, requests[0]);
        MPI_Isend(grid[0], N * sizeof(Cell), MPI_BYTE, rank - 1, ROW_SEND_NEXT,
                  MPI_COMM_WORLD, requests[2]);
      }
      if (rank + 1 <= procs - 1) {
        MPI_IRecv(recv_rows[1], N * sizeof(Cell), MPI_BYTE, rank + 1,
                  ROW_SEND_NEXT, MPI_COMM_WORLD, requests[1]);
        MPI_Isend(grid[BLOCK_SIZE(rank, procs, M) - 1], N * sizeof(Cell),
                  MPI_BYTE, rank + 1, ROW_SEND_NEXT, MPI_COMM_WORLD,
                  requests[3]);
      }

      MPI_Waitall(4, requests, statuses);

      col_offset = turn;
      for (i = 0; i < BLOCK_SIZE(rank, procs, M); ++i) {
        for (j = col_offset; j < N; j += 2) {
          if (grid[i][j].type != ANIMAL)
            continue;
          else if (turn && grid[i][j].animal->modified_by_red) {
            grid[i][j].animal->modified_by_red = false;
            continue;
          }

          landing_pos[0] = landing_pos[1] = -1;

          compute_next_position(grid, i, j, grid[i][j].animal->type, rank,
                                procs, recv_rows[0], recv_rows[1], landing_pos);

          grid[i][j].animal->starvation_age++;
          grid[i][j].animal->breeding_age++;

          if (landing_pos[0] != -1 && landing_pos[1] != -1) {
            grid[i][j].animal->modified_by_red = !turn;

            if (breeding_status(grid[i][j].animal)) {
              Animal *aux = create_animal(grid[i][j].type);
              aux->modified_by_red = !turn;

              change_breeding_age(grid[i][j].animal, 0);
              grid[i][j].incoming_animals[grid[i][j].new_animals++] = aux;
            }

            if (rank == BLOCK_OWNER(landing_pos[0], procs, M)) {
              grid[landing_pos[0] - BLOCK_LOW(rank, procs, M)][landing_pos[1]]
                  .incoming_animals[grid[landing_pos[0]][landing_pos[1]]
                                        .new_animals++] = grid[i][j].animal;
            } else {
              if (BLOCK_OWNER(landing_pos[0], procs, M) == rank - 1) {
                recv_rows[0][j]
                    .incoming_animals[recv_rows[0][j].new_animals++] =
                    grid[i][j].animal;
              } else if (BLOCK_OWNER(landing_pos[0], procs, M) == rank + 1) {
                recv_rows[1][j]
                    .incoming_animals[recv_rows[1][j].new_animals++] =
                    grid[i][j].animal;
              }
            }
            modify_cell(&grid[i][j], EMPTY, NULL);
          }
        }
      }

      for (int k = 0; k < BLOCK_SIZE(rank, procs, M); ++k) {
        for (int l = 0; l < N; ++l) {
          if (grid[k][l].type != ROCK) {
            resolve_conflicts(&grid[k][l]);
            grid[k][l].new_animals = 0;

            if (turn && grid[k][l].animal) {
              if (starvation_status(grid[k][l].animal))
                kill_animal(&grid[k][l]);

              if (grid[k][l].animal)
                grid[k][l].animal->modified_by_red = false;
            }
          }
        }
      }
    }

    send_result_to_master(grid, rank, procs, total_elements);
  }

  if (rank == MASTER_RANK) {
    // output_final_population(total_elements);
  }
}
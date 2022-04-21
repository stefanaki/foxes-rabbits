#include "mpi-implementation.h"

#include <stdio.h>

#include "message-cell.h"
#include "mpi.h"

#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define BLOCK_HIGH(id, p, n) (BLOCK_LOW((id) + 1, p, n) - 1)
#define BLOCK_SIZE(id, p, n) (BLOCK_HIGH(id, p, n) - BLOCK_LOW(id, p, n) + 1)
#define BLOCK_OWNER(j, p, n) (((p) * ((j) + 1) - 1) / (n))

#define MASTER_RANK 0
#define ROW_PREV 111
#define ROW_NEXT 222

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

void print_board(Cell **grid, int chunk, int gen, int turn) {
    printf("Generation %d, %s\n", gen + 1, (!turn) ? "red" : "black");
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

void generate_element_mpi(int n, char atype, uint32_t *seed, Cell **grid, int rank, int procs) {
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

    return subgrid;
}

bool fox_sees_rabbit(Cell *cell, char type) {
    return type == FOX && cell->animal && animal_type(cell->animal, RABBIT);
}
// Compute next position modified
void compute_next_position(Cell **grid, int i, int j, char animal_type,
                           int rank, int procs, Cell *missing_row, int *landing_pos) {
    int p = 0;
    int rabbit_p = 0;
    int available_cells[4][2];
    int available_rabbit_cells[4][2];
    int gi;
    Cell c[4];
    bool c_exists[4] = {false, false, false, false};

    gi = BLOCK_LOW(rank, procs, M) + i;
    int C = gi * N + j;

    if (gi - 1 >= 0) {
        c[0] =
            BLOCK_OWNER(gi - 1, procs, M) == rank ? grid[i - 1][j] : missing_row[j];
        c_exists[0] = true;
    }
    if (j + 1 <= N - 1) {
        c[1] = grid[i][j + 1];
        c_exists[1] = true;
    }
    if (gi + 1 <= M - 1) {
        c[2] =
            BLOCK_OWNER(gi + 1, procs, M) == rank ? grid[i + 1][j] : missing_row[j];
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
            int result_gi = gi;
            int result_j = j;

            if (k == 0) {
                result_gi = gi - 1;
            } else if (k == 2) {
                result_gi = gi + 1;
            }

            if (k == 1) {
                result_j = j + 1;
            } else if (k == 3) {
                result_j = j - 1;
            }

            available_cells[p][0] = result_gi;
            available_cells[p][1] = result_j;
            p++;
        } else if (c[k].animal != NULL && fox_sees_rabbit(&c[k], animal_type)) {
            int result_gi = gi;
            int result_j = j;

            if (k == 0) {
                result_gi = gi - 1;
            } else if (k == 2) {
                result_gi = gi + 1;
            }

            if (k == 1) {
                result_j = j + 1;
            } else if (k == 3) {
                result_j = j - 1;
            }

            available_rabbit_cells[rabbit_p][0] = result_gi;
            available_rabbit_cells[rabbit_p][1] = result_j;
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
}

// Resolve conflicts modified
void resolve_conflicts(Cell *cell) {   // Function that resolves conflicts that
    // might appear on a cell

    Animal *incoming;
    if (!cell->new_animals) {
        cell->type = (cell->animal) ? ANIMAL : EMPTY;
        return;
    }

    for (int i = 0; i < cell->new_animals; i++) {
        incoming = cell->incoming_animals[i];
        if (!cell->animal) {
            modify_cell(cell, ANIMAL, incoming);
            continue;
        }

        if (cell->animal->type == FOX && incoming->type == RABBIT) {
            free(incoming);
            incoming = NULL;
            cell->animal->starvation_age = 0;
        } else if (cell->animal->type == FOX && incoming->type == FOX) {
            if (!cell->animal->starvation_age) {
                free(incoming);
                incoming = NULL;
                cell->animal->starvation_age = 0;
            } else if (!incoming->starvation_age) {
                cell->animal = incoming;
                incoming->starvation_age = 0;
            } else if (cell->animal->breeding_age > incoming->breeding_age) {
                free(incoming);
                incoming = NULL;
            } else if (incoming->breeding_age > cell->animal->breeding_age) {
                cell->animal = incoming;
            } else if (cell->animal->starvation_age < incoming->starvation_age) {
                free(incoming);
                incoming = NULL;
            } else if (incoming->starvation_age < cell->animal->starvation_age) {
                cell->animal = incoming;
            }
        } else if (cell->animal->type == RABBIT && incoming->type == FOX) {
            cell->animal = incoming;
            incoming->starvation_age = 0;
        } else if (cell->animal->type == RABBIT && incoming->type == RABBIT) {
            if (cell->animal->breeding_age >= incoming->breeding_age) {
                free(incoming);
                incoming = NULL;
            } else {
                cell->animal = incoming;
            }
        }
    }
}

// Send generation result to master
void send_result_to_master(Cell **grid, int rank, int procs, int *global_sum) {
    int counters[3] = {0, 0, 0};

    for (int i = 0; i < BLOCK_SIZE(rank, procs, M); ++i) {
        for (int j = 0; j < N; ++j) {
            if (grid[i][j].type == ROCK) {
                counters[0]++;
            } else if (grid[i][j].type == ANIMAL) {
                if (grid[i][j].animal->type == FOX)
                    counters[1]++;
                else
                    counters[2]++;
            }
        }
    }

    printf("%d %d %d , rank: %d\n", counters[0], counters[1], counters[2], rank);
    // reduce to master
    MPI_Reduce(&counters, global_sum, 3, MPI_INT, MPI_SUM, MASTER_RANK, MPI_COMM_WORLD);
}

// Implementation and process reduction
void mpi_implementation(Cell **grid, int rank, int procs, MPI_Datatype message_cell_dt, int *global_sum) {
    bool col_offset;
    int i, j, gen, turn, block_size;
    int landing_pos[2];
    int wait_counter = 0;
    Cell missing_row[N];

    MessageCell ghost_row_prev[N];
    MessageCell ghost_row_next[N];
    MessageCell local_row_prev[N];
    MessageCell local_row_next[N];

    MPI_Request requests[4];

    block_size = BLOCK_SIZE(rank, procs, M);

    for (gen = 0; gen < generations; ++gen) {
        for (turn = 0; turn < 2; ++turn) {
            col_offset = turn;
            // Receive previous and next rows
            if (rank - 1 >= 0) {
                MPI_Irecv(&ghost_row_prev, N, message_cell_dt, rank - 1, ROW_NEXT, MPI_COMM_WORLD, &requests[wait_counter++]);
                printf("rank:%d, i:%d j:%d turn:%d 1\n", rank, i, j, turn);
                init_message_cell_buffer(local_row_prev, grid[0]);
                printf("rank:%d, i:%d j:%d turn:%d 2\n", rank, i, j, turn);
                MPI_Isend(&local_row_prev, N, message_cell_dt, rank - 1, ROW_PREV, MPI_COMM_WORLD, &requests[wait_counter++]);
            }
            if (rank + 1 <= procs - 1) {
                MPI_Irecv(&ghost_row_next, N, message_cell_dt, rank + 1, ROW_PREV, MPI_COMM_WORLD, &requests[wait_counter++]);

                init_message_cell_buffer(local_row_next, grid[block_size - 1]);
                MPI_Isend(&local_row_next, N, message_cell_dt, rank + 1, ROW_NEXT, MPI_COMM_WORLD, &requests[wait_counter++]);
            }

            MPI_Waitall(wait_counter, requests, MPI_STATUSES_IGNORE);
            wait_counter = 0;
            for (i = 0; i < block_size; ++i) {
                // compute missing row
                if (i == 0 && rank - 1 >= 0) {
                    convert_buffer_to_row(ghost_row_prev, missing_row, 1);
                } else if (i == block_size - 1 && rank + 1 <= procs - 1) {
                    convert_buffer_to_row(ghost_row_next, missing_row, 1);
                }

                for (j = col_offset; j < N; j += 2) {
                    if (grid[i][j].type != ANIMAL)
                        continue;
                    else if (turn && grid[i][j].animal->modified_by_red) {
                        grid[i][j].animal->modified_by_red = false;
                        continue;
                    }

                    landing_pos[0] = landing_pos[1] = -1;
                    compute_next_position(grid, i, j, grid[i][j].animal->type, rank, procs, missing_row, landing_pos);
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
                                .incoming_animals[grid[landing_pos[0] - BLOCK_LOW(rank, procs, M)][landing_pos[1]].new_animals++] = grid[i][j].animal;
                        } else {
                            if (BLOCK_OWNER(landing_pos[0], procs, M) == rank - 1) {
                                ghost_row_prev[j]
                                    .incoming_animals[ghost_row_prev[j].new_animals++] = *grid[i][j].animal;
                            } else if (BLOCK_OWNER(landing_pos[0], procs, M) == rank + 1) {
                                ghost_row_next[j]
                                    .incoming_animals[ghost_row_next[j].new_animals++] = *grid[i][j].animal;
                            }
                        }
                        modify_cell(&grid[i][j], EMPTY, NULL);
                    }
                }
                col_offset = !col_offset;
            }

            // done with computing movements and stuff
            // send back the missing cell
            MPI_Barrier(MPI_COMM_WORLD);   // check if we can remove this after
            // rank before
            if (rank - 1 >= 0) {
                MPI_Irecv(&local_row_prev, N, message_cell_dt, rank - 1, ROW_NEXT, MPI_COMM_WORLD, &requests[wait_counter++]);

                MPI_Isend(&ghost_row_prev, N, message_cell_dt, rank - 1, ROW_PREV, MPI_COMM_WORLD, &requests[wait_counter++]);
            }
            // rank after
            if (rank + 1 < procs) {
                MPI_Irecv(&local_row_next, N, message_cell_dt, rank + 1, ROW_PREV, MPI_COMM_WORLD, &requests[wait_counter++]);

                MPI_Isend(&ghost_row_next, N, message_cell_dt, rank + 1, ROW_NEXT, MPI_COMM_WORLD, &requests[wait_counter++]);
            }
            MPI_Waitall(wait_counter, requests, MPI_STATUSES_IGNORE);
            wait_counter = 0;

            // merge message_cells with cell
            if (rank - 1 >= 0) {
                convert_buffer_to_row(local_row_prev, grid[0], 0);
            } else if (rank + 1 <= procs - 1) {
                convert_buffer_to_row(local_row_next, grid[block_size - 1], 0);
            }
            // compute merge conflicts
            for (int k = 0; k < block_size; ++k) {
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
            if (rank == 0) {
                print_board(grid, block_size, gen, turn);
            }
        }
    }
    send_result_to_master(grid, rank, procs, global_sum);
}
#include "mpi.h"
#include "mpi-implementation.h"
#include <stdio.h>

#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define BLOCK_HIGH(id, p, n) (BLOCK_LOW((id) + 1, p, n) - 1)
#define BLOCK_SIZE(id, p, n) (BLOCK_HIGH(id, p, n) - BLOCK_LOW(id, p, n) + 1)
#define BLOCK_OWNER(j, p, n) (((p) * ((j) + 1) - 1) / (n))

#define MASTER_RANK 0

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

void print_board(Cell **grid, int chunk)
{
    for (int l = 0; l <= N; l++)
        printf("---");
    printf("\n   ");
    for (int l = 0; l < N; l++)
        printf("%02d|", l);
    for (int k = 0; k < chunk; k++)
    {
        printf("\n");
        printf("0%d:", k);
        for (int l = 0; l < N; l++)
        {
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

void generate_element_mpi(int n, char atype, uint32_t *seed, Cell **grid, int rank, int procs)
{
    int i, j, k, local_i;

    for (k = 0; k < n; k++)
    {
        i = M * r4_uni(seed);
        j = N * r4_uni(seed);

        int size = BLOCK_SIZE(rank, procs, M);
        for (int l = 0; l < size; l++)
        {
            local_i = i - BLOCK_LOW(rank, procs, M);
        }

        int element_rank = BLOCK_OWNER(i, procs, M);

        if (rank == element_rank && position_empty(&grid[local_i][j]))
        {
            insert_element(&grid[local_i][j], atype);
        }
    }
}

// Generate world subgrid based on process id
Cell **generate_world_subgrid(int rank, int procs)
{
    Cell **subgrid;

    int chunk = BLOCK_SIZE(rank, procs, M);

    subgrid = (Cell **)malloc(sizeof(Cell) * chunk);
    for (int i = 0; i < M; ++i)
    {
        subgrid[i] = (Cell *)malloc(sizeof(Cell) * N);
    }

    for (int i = 0; i < chunk; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
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

// Compute next position modified
void compute_next_position(Cell **grid, int i, int j, int rank, int procs, int *landing_pos)
{
    return;
}

// Ask for data from cell that doesn't belong in the same process
Cell get_cell_from_rank(int rank, int i, int j)
{
    return *(Cell *)malloc(sizeof(Cell) * N);
}

// Modify cell that doesn't belong to the same process
void modify_cell_from_rank(int rank, int i, int j, Cell data)
{
    return;
}

// Resolve conflicts modified
void resolve_conflicts(Cell *cell)
{
    return;
}

// Send generation result to master
void send_result_to_master(Cell **grid, int rank, int procs, int totals[])
{
    return;
}

// Implementation and process reduction
void mpi_implementation(Cell **grid, int rank, int procs)
{
    bool col_offset;
    int i, j, gen, turn;
    int landing_pos[2];
    int total_elements[3] = {0, 0, 0}; // foxes, rabbits, rocks

    for (gen = 0; gen < generations; ++gen)
    {
        for (turn = 0; turn < 2; ++turn)
        {
            col_offset = turn;
            for (i = 0; i < BLOCK_SIZE(rank, procs, M); ++i)
            {
                for (j = col_offset; j < N; j += 2)
                {
                    if (grid[i][j].type != ANIMAL)
                        continue;
                    else if (turn && grid[i][j].animal->modified_by_red)
                    {
                        grid[i][j].animal->modified_by_red = false;
                        continue;
                    }

                    landing_pos[0] = landing_pos[1] = -1;
                    compute_next_position(grid, i, j, rank, procs, landing_pos);

                    grid[i][j].animal->starvation_age++;
                    grid[i][j].animal->breeding_age++;

                    if (landing_pos[0] != -1 && landing_pos[1] != -1)
                    {
                        grid[i][j].animal->modified_by_red = !turn;

                        if (breeding_status(grid[i][j].animal))
                        {
                            Animal *aux = create_animal(grid[i][j].type);
                            aux->modified_by_red = !turn;

                            change_breeding_age(grid[i][j].animal, 0);
                            grid[i][j].incoming_animals[grid[i][j].new_animals++] = aux;
                        }

                        if (rank == BLOCK_OWNER(landing_pos[0], procs, M))
                        {
                            grid[landing_pos[0]][landing_pos[1]].incoming_animals[grid[landing_pos[0]][landing_pos[1]].new_animals++] = grid[i][j].animal;
                            modify_cell(&grid[i][j], EMPTY, NULL);
                        }
                        else
                        {
                            // tbh i need to think about this more
                            // Cell temp = get_cell_from_rank(BLOCK_OWNER(landing_pos[0], procs, M), landing_pos[0], landing_pos[1]);
                            // move_animal_to_cell();
                        }
                    }
                }
            }

            for (int k = 0; k < BLOCK_SIZE(rank, procs, M); ++k)
            {
                for (int l = 0; l < N; ++l)
                {
                    if (grid[k][l].type != ROCK)
                    {
                        resolve_conflicts(&grid[k][l]);
                        grid[k][l].new_animals = 0;

                        if (turn && grid[k][l].animal)
                        {
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

    if (rank == MASTER_RANK)
    {
        // output_final_population(total_elements);
    }
}
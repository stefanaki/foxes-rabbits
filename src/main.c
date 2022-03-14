#include <stdio.h>
#include <stdlib.h>
#include "world.h"
#include "worldgen.h"
#include "serial.h"

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

int main(int argc, char **argv)
{
    // foxes-rabbits <# generations> <M> <N> <# rocks> <# rabbits>
    //  <rabbit breeding> <# foxes> <fox breeding> <fox starvation> <seed>

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

    World world = (World){ .row_size = M, .column_size = N };
    world.grid = (Cell **)malloc(sizeof(Cell) * M);
    
    for (int i = 0; i < M; i++)
    {
        world.grid[i] = (Cell *)malloc(sizeof(Cell) * N);
    }

    for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; ++j)
            world.grid[i][j].board_index = i * N + j;

    generate_element(2, FOX, &seed, &world);
    generate_element(3, RABBIT, &seed, &world);
    generate_element(10, ROCK, &seed, &world);

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d\t", world.grid[i][j].board_index);
        }
        printf("\n");
    }

    serial_implementation(&world);

    return 0;
}
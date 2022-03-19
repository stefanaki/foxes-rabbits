#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
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
    World world = (World){};
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

    init_world(&world);

    // Printing board
    print_board(&world, -1,0);
    
    serial_implementation(&world);

    // count rocks, rabbits and foxes
    int rocks = 0, rabbits = 0, foxes = 0;
    for (int k = 0; k < M; k++)
    {
        for (int l = 0; l < N; l++)
        {
            if (world.grid[k][l].type == EMPTY)
            {
                continue;
            }
            else if (world.grid[k][l].type == ROCK)
            {
                rocks++;
            }
            else if (world.grid[k][l].animal->type == FOX)
            {
                foxes++;
            }
            else
            {
                rabbits++;
            }
        }
    }

    printf("%d %d %d\n", rocks, rabbits, foxes);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "worldgen.h"
#include "worldstructs.h"

int main(int argc, char **argv)
{
    // foxes-rabbits <# generations> <M> <N> <# rocks> <# rabbits>
    //  <rabbit breeding> <# foxes> <fox breeding> <fox starvation> <seed>

    // example of initial args
    // 10 5 5 10 3 6 2 6 8 10
    uint32_t generations = atoi(argv[1]);
    uint32_t M = atoi(argv[2]);
    uint32_t N = atoi(argv[3]);
    uint32_t n_rocks = atoi(argv[4]);
    uint32_t n_rabbits = atoi(argv[5]);
    uint32_t rabbit_breeding = atoi(argv[6]);
    uint32_t n_foxes = atoi(argv[7]);
    uint32_t fox_breeding = atoi(argv[8]);
    uint32_t fox_starvation = atoi(argv[9]);
    uint32_t seed = atoi(argv[10]);

    Board board = (Board){ .row_size = M, .column_size = N };
    board.grid = (Cell **)malloc(sizeof(Cell) * M);
    for (int i = 0; i < M; i++)
    {
        board.grid[i] = (Cell *)malloc(sizeof(Cell) * N);
    }

    generate_element(2, FOX, &seed, &board);
    generate_element(3, RABBIT, &seed, &board);
    generate_element(10, ROCK, &seed, &board);

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d\t", board.grid[i][j].element.type);
        }
        printf("\n");
    }

    return 0;
}
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "worldstructs.h"

float r4_uni(uint32_t *seed)
{
    int seed_input, sseed;
    float r;

    seed_input = *seed;
    *seed ^= (*seed << 13);
    *seed ^= (*seed >> 17);
    *seed ^= (*seed << 5);
    sseed = *seed;
    r = 0.5 + 0.2328306e-09 * (seed_input + sseed);

    return r;
}

void insert_animal(int i, int j, char atype, Board *board)
{
    board->cells[i][j].element.type = atype;
}

bool position_empty(int i, int j, Board *board)
{
    return board->cells[i][j].element.type == EMPTY;
}

void generate_element(int n, char atype, uint32_t *seed, Board *board, int M, int N)
{
    int i, j, k;

    for (k = 0; k < n; k++)
    {
        i = M * r4_uni(seed);
        j = N * r4_uni(seed);
        if (position_empty(i, j, board))
        {
            insert_animal(i, j, atype, board);
        }
    }
}

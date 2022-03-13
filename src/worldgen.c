#include <stdint.h>
#include <stdbool.h>
#include "world.h"
#include "worldgen.h"

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

void generate_element(int n, char atype, uint32_t *seed, World *world)
{
    int i, j, k;

    for (k = 0; k < n; k++)
    {
        i = world->row_size * r4_uni(seed);
        j = world->column_size * r4_uni(seed);
        if (position_empty(&world->grid[i][j]))
        {
            insert_animal(&world->grid[i][j], atype);
        }
    }
}

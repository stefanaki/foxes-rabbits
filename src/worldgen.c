#include "worldgen.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "world.h"

extern uint32_t M;
extern uint32_t N;
extern uint32_t n_rocks;
extern uint32_t n_rabbits;
extern uint32_t n_foxes;
extern uint32_t seed;

float r4_uni(uint32_t *seed) {
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

void generate_element(int n, char atype, uint32_t *seed, World *world) {
    int i, j, k;

    for (k = 0; k < n; k++) {
        i = world->row_size * r4_uni(seed);
        j = world->column_size * r4_uni(seed);
        if (position_empty(&world->grid[i][j])) {
            insert_element(&world->grid[i][j], atype);
        }
    }
}

void init_world(World *world) {
    world->row_size = M;
    world->column_size = N;
    world->grid = (Cell **)malloc(sizeof(Cell) * M);

    for (int i = 0; i < M; ++i) {
        world->grid[i] = (Cell *)malloc(sizeof(Cell) * N);
    }

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            modify_cell(&world->grid[i][j], EMPTY, NULL);
            world->grid[i][j].board_index = i * N + j;
            world->grid[i][j].new_animals = 0;
        }
    }
    generate_element(n_rocks, ROCK, &seed, world);
    generate_element(n_rabbits, RABBIT, &seed, world);
    generate_element(n_foxes, FOX, &seed, world);
}

void world_cleanup(World *world) {
    free(world->grid);
}
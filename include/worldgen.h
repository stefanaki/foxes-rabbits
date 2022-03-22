#include <stdint.h>
#include <stdbool.h>
#include "world.h"

#ifndef WORLDGEN_LIB_H__
#define WORLDGEN_LIB_H__

float r4_uni(uint32_t *);
void generate_element(int, char, uint32_t *, World *);
void init_world(World *);
void world_cleanup(World *);

#endif /* WORLDGEN_LIB_H__ */
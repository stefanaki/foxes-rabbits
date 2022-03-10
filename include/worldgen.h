#include <stdint.h>
#include <stdbool.h>
#include "worldstructs.h"

#ifndef WORLDGEN_LIB_H__
#define WORLDGEN_LIB_H__

float r4_uni(uint32_t *);
void generate_element(int, char, uint32_t *, Board *, int, int);
void insert_animal(int, int, char, Board *);
bool position_empty(int, int, Board *)

#endif /* WORLDGEN_LIB_H__ */
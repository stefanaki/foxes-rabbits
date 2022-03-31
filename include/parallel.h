#ifndef PARALLEL_LIB_H__
#define PARALLEL_LIB_H__

#include "world.h"
#include <stdlib.h>

Cell *compute_next_position(World *, int, int, char);
void resolve_conflicts(Cell *);
void parallel_implementation(World *);
void print_board(World *, int, int);

#endif /* PARALLEL_LIB_H__ */
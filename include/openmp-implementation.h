#ifndef OMP_IMPL_LIB_H__
#define OMP_IMPL_LIB_H__

#include <stdlib.h>

#include "world.h"

Cell *compute_next_position(World *, int, int, char);
void resolve_conflicts(Cell *);
void parallel_implementation(World *);
void print_board(World *, int, int);

#endif /* OMP_IMPL_LIB_H__ */
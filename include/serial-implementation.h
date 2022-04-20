#ifndef SERIAL_LIB_H__
#define SERIAL_LIB_H__

#include <stdlib.h>

#include "world.h"

Cell *compute_next_position(World *, int, int, char);
void resolve_conflicts(Cell *);
void serial_implementation(World *);
void print_board(World *, int, int);

#endif /* SERIAL_LIB_H__ */
#ifndef SERIAL_LIB_H__
#define SERIAL_LIB_H__

#include "world.h"
#include <stdlib.h>

Cell *compute_next_position(World *, int, int, char);
void resolve_conflicts(Cell *, int);
void serial_implementation(World *);
void print_board(World*,  int,  int);

#endif /* SERIAL_LIB_H__ */
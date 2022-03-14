#ifndef SERIAL_LIB_H__
#define SERIAL_LIB_H__

#include "world.h"

Cell *compute_next_position(World *, int, int);
void resolve_conflicts(Cell *, Cell *);
void serial_implementation(World *);

#endif /* SERIAL_LIB_H__ */
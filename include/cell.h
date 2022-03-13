#include "worldstructs.h"

#ifndef CELL_LIB_H__
#define CELL_LIB_H__

void modify_cell(Cell*, enum ElementType, Animal*, bool);
void insert_animal(int, int, enum ElementType, Board *);
bool position_empty(int, int, Board *);

#endif /* CELL_LIB_H__ */
#include "worldstructs.h"

#ifndef CELL_LIB_H__
#define CELL_LIB_H__

void modify_cell(Cell*, enum ElementType, Animal*, bool);
bool position_empty(int, int, Board *);
bool position_processed(int, int, Board *);

#endif /* CELL_LIB_H__ */
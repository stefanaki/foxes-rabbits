#include "cell.h"

void modify_cell(Cell *cell, enum ElementType type, Animal *animal, bool modified){
  cell->type = type;
  cell->animal = animal;
  cell->modified = modified;
}

void insert_animal(int i, int j, enum ElementType atype, Board *board)
{
  board->grid[i][j].type = atype;
}

bool position_empty(int i, int j, Board *board)
{
  return board->grid[i][j].type == EMPTY;
}

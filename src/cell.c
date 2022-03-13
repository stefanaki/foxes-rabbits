#include "cell.h"

void modify_cell(Cell *cell, enum ElementType type, Animal *animal, bool modified){
  cell->type = type;
  cell->animal = animal;
  cell->modified = modified;
}

bool position_empty(int i, int j, Board *board)
{
  return board->grid[i][j].type == EMPTY;
}

bool position_processed(int i, int j, Board *board){
  //function to see if cell has already been processed (modified==true)
  return false;
}
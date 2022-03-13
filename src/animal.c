#include "worldstructs.h"
#include "cell.h"

void insert_animal(int i, int j, enum AnimalType atype, Board *board)
{
  Animal animal = {.type = atype, .age=0, .breeding_age=0, .starvation_age=0, .has_moved=0};

  Cell* cell = &board->grid[i][j];
  cell->type = ANIMAL;
  cell->animal = &animal;
  cell->modified = true;
  //new_animals?
}

bool breeding_status(Animal *animal){
  return animal->breeding_age==animal->age;
}

void move(int i, int j, Board *board, int n_i, int n_j){
  if (position_empty(n_i, n_j, board)) return;


  modify_cell(&board->grid[n_i][n_j], board->grid[i][j].type, board->grid[i][j].animal, true);

  if (breeding_status(board->grid[i][j].animal)){
    insert_animal(i, j, board->grid[i][j].type, board);
  } else{
    modify_cell(&board->grid[i][j], EMPTY, NULL, true);
  }

}

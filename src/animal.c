#include "worldstructs.h"
#include "cell.h"

extern uint32_t fox_breeding;
extern uint32_t fox_starvation;
extern uint32_t rabbit_breeding;

void insert_animal(int i, int j, enum AnimalType atype, Board *board)
{
  Animal animal = {.type = atype, .breeding_age=0, .has_moved=0};
  if (atype == FOX){
    animal.starvation_age = malloc(sizeof(uint16_t));
    animal.starvation_age = 0;
  } else animal.starvation_age = NULL;

  Cell* cell = &board->grid[i][j];
  cell->type = ANIMAL;
  cell->animal = &animal;
  cell->modified = true;
  //new_animals?
}

bool breeding_status(Animal *animal){
  uint32_t limit = (animal->type == RABBIT) ? rabbit_breeding: fox_breeding;
  return animal->breeding_age==limit;
}

void move(int i, int j, Board *board, int n_i, int n_j){
  if (position_empty(n_i, n_j, board) && 
      position_processed(n_i, n_j, board)) return;

  modify_cell(&board->grid[n_i][n_j], board->grid[i][j].type, board->grid[i][j].animal, true);

  if (breeding_status(board->grid[i][j].animal)){
    insert_animal(i, j, board->grid[i][j].type, board);
  } else{
    modify_cell(&board->grid[i][j], EMPTY, NULL, true);
  }

}

void die(Cell *cell){

}

void eat(Animal *animal, Cell *cell){

}

void give_baby(int i, int j, Board *board, int n_i, int n_j){
  
}

bool check_life(Animal *animal){
  return false;
}

Cell* find_space(int i, int j, Board *board){
  return NULL;
}

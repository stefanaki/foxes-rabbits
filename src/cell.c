#include "cell.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdlib.h>

void insert_element(Cell *cell, char atype) {
  if (atype == ROCK) {
    cell->type = ROCK;
    return;
  }

  Animal *animal = (Animal *)malloc(sizeof(Animal));

  animal->type = atype;
  animal->breeding_age = 0;
  animal->starvation_age = 0;
  animal->has_moved = false;

  cell->type = ANIMAL;
  cell->animal = animal;
  cell->incoming_animal = NULL;
}

void modify_cell(Cell *cell, char type, Animal *animal) {
  cell->type = type;
  cell->animal = animal;
}

void kill_animal(Cell *cell) {
  free(cell->animal);
  modify_cell(cell, EMPTY, NULL);
}

void move_animal(Cell *initial_cell, Cell *final_cell) {
  if (position_empty(final_cell) && position_processed(final_cell))
    return;

  modify_cell(final_cell, initial_cell->type, initial_cell->animal);

  if (breeding_status(initial_cell->animal)) {
    insert_element(initial_cell, initial_cell->animal->type);
  } else {
    modify_cell(initial_cell, EMPTY, NULL);
  }
}

bool position_empty(Cell *cell) { return cell->type == EMPTY; }

bool position_processed(Cell *cell) { return cell->modified_by_red; }
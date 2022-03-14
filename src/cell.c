#include "cell.h"
#include <stddef.h>

void insert_animal(Cell *cell, char atype) {
  Animal animal = {.type = atype, .breeding_age = 0, .has_moved = 0, .age = 0};

  cell->type = ANIMAL;
  cell->animal = animal;
  cell->incoming_animal = NULL;
}

void modify_cell(Cell *cell, char type, Animal *animal, bool modified) {
  cell->type = type;
  cell->animal = animal;
  cell->modified_by_red = modified;
}

void kill_animal(Cell *cell) {
  free(cell->animal);
  modify_cell(cell, EMPTY, NULL, true);
}

void move_animal(Cell *initial_cell, Cell *final_cell) {
  if (position_empty(final_cell) && position_processed(final_cell))
    return;

  modify_cell(final_cell, initial_cell->type, initial_cell->animal, true);

  if (breeding_status(initial_cell->animal)) {
    insert_animal(initial_cell, initial_cell->animal->type);
  } else {
    modify_cell(initial_cell, EMPTY, NULL, true);
  }
}

bool position_empty(Cell *cell) { return cell->type == EMPTY; }

bool position_processed(Cell *cell) { return cell->modified_by_red; }
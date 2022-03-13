#include "cell.h"
#include <stddef.h>

void insert_animal(Cell *cell, char atype) {
  Animal animal = {.type = atype, .breeding_age = 0, .has_moved = 0, .age = 0};

  cell->type = ANIMAL;
  cell->animal = animal;
  // new_animals?
}

void modify_cell(Cell *cell, char type, Animal animal, bool modified) {
  cell->type = type;
  cell->animal = animal;
  cell->modified = modified;
}

void kill_animal(Cell *cell) {
  // TODO
}

void move_animal(Cell *initial_cell, Cell *final_cell) {
  if (position_empty(final_cell) && position_processed(final_cell))
    return;

  modify_cell(final_cell, initial_cell->type, initial_cell->animal, true);

  if (breeding_status(&initial_cell->animal)) {
    insert_animal(initial_cell, initial_cell->type);
  } else {
    Animal dummy_animal = {
        .type = RABBIT, .breeding_age = 0, .has_moved = 0, .age = 0};
    modify_cell(initial_cell, EMPTY, dummy_animal, true);
  }
}

bool position_empty(Cell *cell) { return cell->type == EMPTY; }

bool position_processed(Cell *cell) { return cell->modified; }
#include "cell.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdlib.h>

void insert_element(Cell *cell, char atype)
{
  if (atype == ROCK)
  {
    modify_cell(cell, ROCK, NULL);
    return;
  }

  Animal *animal = create_animal(atype);
  modify_cell(cell, ANIMAL, animal);
}

void modify_cell(Cell *cell, char type, Animal *animal)
{
  cell->type = type;
  cell->animal = animal;
}

void kill_animal(Cell *cell)
{
  cell->animal = NULL;
  modify_cell(cell, EMPTY, NULL);
}

void move_animal(Cell *initial_cell, Cell *final_cell)
{
  if (animal_type(final_cell->animal, RABBIT) && breeding_status(final_cell->animal))
  {
    modify_cell(final_cell, initial_cell->type, initial_cell->animal);
    insert_element(initial_cell, RABBIT);
    reset_animal(initial_cell->animal);
  }
  else if (animal_type(final_cell->animal, FOX) && breeding_status(final_cell->animal))
  {
    modify_cell(final_cell, initial_cell->type, initial_cell->animal);
    insert_element(initial_cell, FOX);
    reset_animal(final_cell->animal);
  }
  else
  { // Animal moves to neighbouring cell but does not breed
    modify_cell(final_cell, initial_cell->type, initial_cell->animal);
    modify_cell(initial_cell, EMPTY, NULL);
    final_cell->animal->breeding_age++;
    // starvation in the other file
  }
}

bool position_empty(Cell *cell) { return cell->type == EMPTY; }

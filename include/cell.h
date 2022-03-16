#include "animal.h"

#ifndef CELL_LIB_H__
#define CELL_LIB_H__

enum ElementType
{
  EMPTY,
  ANIMAL,
  ROCK
};

typedef struct Cell
{
  char type;
  Animal *animal;
  Animal *incoming_animals[4];
  uint16_t new_animals;
  uint32_t board_index;
} Cell;

void modify_cell(Cell *, char, Animal *);
void insert_element(Cell *, char);
void kill_animal(Cell *);
void move_animal(Cell *, Cell *);
void breed(Cell *, Animal *);
bool position_empty(Cell *);

#endif /* CELL_LIB_H__ */
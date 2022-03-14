#include "animal.h"

#ifndef CELL_LIB_H__
#define CELL_LIB_H__

enum ElementType { EMPTY, ANIMAL, ROCK };

typedef struct Cell {
  enum ElementType type;
  Animal *animal;
  Animal *incoming_animal;
  bool modified_by_red;
  uint32_t board_index;
} Cell;

void modify_cell(Cell *, char, Animal*, bool);
void insert_animal(Cell *, char);
void kill_animal(Cell *);
void move_animal(Cell *, Cell *);
void breed(Cell *, Animal *);
bool position_empty(Cell *);
bool position_processed(Cell *);

#endif /* CELL_LIB_H__ */
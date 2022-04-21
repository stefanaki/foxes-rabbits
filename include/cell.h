#include <stdint.h>

#include "animal.h"

#ifndef CELL_LIB_H__
#define CELL_LIB_H__

enum ElementType {
    EMPTY,
    ANIMAL,
    ROCK
};

typedef struct Cell {
    char type;
    Animal *animal;
    Animal *incoming_animals[4];
    uint16_t new_animals;
} Cell;

void modify_cell(Cell *, char, Animal *);
void insert_element(Cell *, char);
void kill_animal(Cell *);
bool position_empty(Cell *);
void cell_cleanup(Cell *);
Animal get_animal(Cell *);

#endif /* CELL_LIB_H__ */
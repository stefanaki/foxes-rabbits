#include "cell.h"

#include <stddef.h>
#include <stdlib.h>

void insert_element(Cell *cell, char type) {
    if (type == ROCK) {
        modify_cell(cell, ROCK, NULL);
        return;
    }

    Animal *animal = create_animal(type);
    cell->new_animals = 0;
    modify_cell(cell, ANIMAL, animal);
}

void modify_cell(Cell *cell, char type, Animal *animal) {
    cell->type = type;
    cell->animal = animal;
}

void kill_animal(Cell *cell) {
    free(cell->animal);
    cell->animal = NULL;
    modify_cell(cell, EMPTY, NULL);
}

bool position_empty(Cell *cell) { return cell->type == EMPTY; }

void cell_cleanup(Cell *cell) {
    free(cell);
}

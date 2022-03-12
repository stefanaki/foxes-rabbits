#include <stdint.h>
#include <stdbool.h>
#include "worldstructs.h"

#ifndef ANIMAL_LIB_H__
#define ANIMAL_LIB_H__

void die(Cell *);
void eat(Animal *, Cell *);
void move(int, int, Board *, int, int);
void give_baby(int, int, Board *, int, int);

bool check_life(Animal*);
bool check_breeding_status(Animal*);
Cell* find_space(int, int, Board *);

#endif /* ANIMAL_LIB_H__ */
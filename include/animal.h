#include "worldstructs.h"
#include "cell.h"

#ifndef ANIMAL_LIB_H__
#define ANIMAL_LIB_H__

void insert_animal(int, int, enum ElementType, Board *);


void die(Cell *);
void eat(Animal *, Cell *);
void move(int, int, Board *, int, int);
void give_baby(int, int, Board *, int, int);

bool check_life(Animal*);
bool breeding_status(Animal*);
Cell* find_space(int, int, Board *);

#endif /* ANIMAL_LIB_H__ */
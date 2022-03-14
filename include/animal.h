#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef ANIMAL_LIB_H__
#define ANIMAL_LIB_H__

enum AnimalType { RABBIT, FOX };

typedef struct Animal {
  char type;
  uint16_t starvation_age;
  uint16_t breeding_age;
  bool has_moved;
} Animal;

Animal* create_animal(char);
bool check_life(Animal *);
void change_breedingAge(Animal *, int);
bool breeding_status(Animal *);
bool animal_type(Animal*, char);
// Cell *find_space(int, int, Board *);
// This function will be in the coordinator functions not here

#endif /* ANIMAL_LIB_H__ */
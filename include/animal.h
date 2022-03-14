#include <stdbool.h>
#include <stdint.h>

#ifndef ANIMAL_LIB_H__
#define ANIMAL_LIB_H__

enum AnimalType { RABBIT, FOX };

typedef struct Animal {
  char type;
  uint16_t starvation_age;
  uint16_t breeding_age;
  bool has_moved;
} Animal;

bool check_life(Animal *);
bool breeding_status(Animal *);
// Cell *find_space(int, int, Board *);
// This function will be in the coordinator functions not here

#endif /* ANIMAL_LIB_H__ */
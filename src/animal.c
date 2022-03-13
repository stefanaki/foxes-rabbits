#include "animal.h"
#include "cell.h"

extern uint32_t fox_breeding;
extern uint32_t fox_starvation;
extern uint32_t rabbit_breeding;

bool breeding_status(Animal *animal) {
  uint32_t limit = (animal->type == RABBIT) ? rabbit_breeding : fox_breeding;
  return animal->breeding_age == limit;
}

bool check_life(Animal *animal) { return false; }
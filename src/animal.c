#include "animal.h"
#include "cell.h"

extern uint32_t fox_breeding;
extern uint32_t fox_starvation;
extern uint32_t rabbit_breeding;

void change_age(Animal *animal, int n_age) {
  animal->age=n_age;
}

void reset_animal(Animal *animal) {
  change_age(animal, 0);
}

bool starvation_status(Animal *animal) {
  return animal->breeding_age == fox_starvation;
}

bool breeding_status(Animal *animal) {
  uint32_t limit = (animal->type == RABBIT) ? rabbit_breeding : fox_breeding;
  return animal->breeding_age == limit;
}

bool check_life(Animal *animal) {
  return (animal->type == FOX) && (animal->age == fox_starvation);
}
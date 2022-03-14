#include "animal.h"
#include "cell.h"

extern uint32_t fox_breeding;
extern uint32_t fox_starvation;
extern uint32_t rabbit_breeding;

bool animal_type(Animal *animal, char atype) {
  return animal->type==atype;
}

void change_starvationAge(Animal *animal, int n_age) {
  animal->starvation_age=n_age;
}

void change_breedingAge(Animal *animal, int n_age) {
  animal->breeding_age=n_age;

Animal* create_animal(char type){
  Animal animal = {.type = type, .starvation_age=0, .breeding_age=0, .has_moved=false};
  return &animal;
}

void reset_animal(Animal *animal) {
  change_breedingAge(animal, 0);
  change_starvationAge(animal, 0);
}

bool starvation_status(Animal *animal) {
  return animal->starvation_age == fox_starvation;
}

bool breeding_status(Animal *animal) {
  uint32_t limit = (animal->type == RABBIT) ? rabbit_breeding : fox_breeding;
  return animal->breeding_age >= limit;
}

bool check_life(Animal *animal) {
  return (animal->type == FOX) && (animal->starvation_age == fox_starvation);
}
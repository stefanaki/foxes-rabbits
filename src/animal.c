#include <stdio.h>
#include "animal.h"
#include "cell.h"

extern uint32_t fox_breeding;
extern uint32_t fox_starvation;
extern uint32_t rabbit_breeding;

bool animal_type(Animal *animal, char type)
{
    return animal->type == type;
}

void change_breeding_age(Animal *animal, int n_age)
{
    animal->breeding_age = n_age;
}

Animal *create_animal(char type)
{
    Animal *animal = malloc(sizeof(Animal));
    animal->type = type;
    animal->starvation_age = 0;
    animal->breeding_age = 0;

    return animal;
}

bool starvation_status(Animal *animal)
{
    return animal_type(animal, FOX) && (animal->starvation_age >= fox_starvation);
}

bool breeding_status(Animal *animal)
{
    uint32_t limit = (animal->type == RABBIT) ? rabbit_breeding : fox_breeding;
    return animal->breeding_age >= limit;
}

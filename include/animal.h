#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef ANIMAL_LIB_H__
#define ANIMAL_LIB_H__

enum AnimalType
{
    RABBIT,
    FOX
};

typedef struct Animal
{
    char type;
    uint16_t starvation_age;
    uint16_t breeding_age;
    bool modified_by_red;
} Animal;

Animal *create_animal(char);
void change_breeding_age(Animal *, int);
bool starvation_status(Animal *);
bool breeding_status(Animal *);
bool animal_type(Animal *, char);

#endif /* ANIMAL_LIB_H__ */
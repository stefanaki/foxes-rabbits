#include <stdint.h>
#include <stdbool.h>

#ifndef WORLDSTRUCTS_LIB_H__
#define WORLDSTRUCTS_LIB_H__

enum ElementType
{
    EMPTY,
    ANIMAL,
    ROCK
};

enum AnimalType
{
    RABBIT,
    FOX
};

typedef struct Animal
{
    enum AnimalType type;
    uint16_t breeding_age;
    uint16_t starvation_age; //null if type==Rabbit
    bool has_moved;
} Animal;

typedef struct Cell
{
    enum ElementType type;
    Animal *animal; //null if type!=animal
    bool modified;
    Animal new_animals[4];
    uint32_t board_index;
} Cell;

typedef struct Board
{
    Cell **grid;
    int column_size;
    int row_size;
} Board;

#endif
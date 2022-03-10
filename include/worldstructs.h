#include <stdint.h>
#include <stdbool.h>

#ifndef WORLDSTRUCTS_LIB_H__
#define WORLDSTRUCTS_LIB_H__

enum ElementType
{
    EMPTY,
    RABBIT,
    FOX,
    ROCK
};

typedef struct Element
{
    char type;
    uint16_t breeding_age;
    uint16_t starvation_age;
    bool has_moved;
} Element;

typedef struct Cell
{
    Element element;
    bool modified;
    Element new_elements[4];
    u_int32_t board_index;
} Cell;

typedef struct Board
{
    Cell **cells;
    int column_size;
    int row_size;
} Board;

#endif
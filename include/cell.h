#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef CELL_LIB_H__
#define CELL_LIB_H__

typedef struct Element
{
    uint16_t breedingAge;
    uint16_t starvationAge;
    bool hasMoved;
} Element;

typedef struct Cell
{
    Element element;
    bool modified;
    Element foxes[4];
    Element rabbits[4];
    u_int32_t boardIndex;
} Cell;

Cell *createCell(u_int32_t boardIndex);
void generate_element(int, char, uint32_t *);

#endif
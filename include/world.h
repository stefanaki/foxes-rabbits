#include "cell.h"

#ifndef WORLD_LIB_H__
#define WORLD_LIB_H__

typedef struct World
{
    Cell **grid;
    int column_size;
    int row_size;
} World;

#endif /* WORLD_LIB_H__ */
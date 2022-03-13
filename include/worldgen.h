#include <stdint.h>
#include <stdbool.h>
#include "worldstructs.h"
#include "cell.h"
#include "animal.h"

#ifndef WORLDGEN_LIB_H__
#define WORLDGEN_LIB_H__

float r4_uni(uint32_t *);
void generate_element(int, enum ElementType, uint32_t *, Board *);

#endif /* WORLDGEN_LIB_H__ */
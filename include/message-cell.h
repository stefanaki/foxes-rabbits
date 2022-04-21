#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "animal.h"
#include "cell.h"

#ifndef MESSAGE_CELL_LIB_H__
#define MESSAGE_CELL_LIB_H__

typedef struct MessageCell {
    Animal incoming_animals[4];
    Animal animal;
    uint16_t new_animals;
    char type;
} MessageCell;

void init_message_cell(MessageCell *, Cell *);
void init_message_cell_buffer(MessageCell *, Cell *);
void convert_buffer_to_row(MessageCell *, Cell *, int);

#endif /* MESSAGE_CELL_LIB_H__ */
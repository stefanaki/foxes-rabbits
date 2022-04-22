#include "message-cell.h"

#include <stdio.h>

#include "cell.h"

extern uint32_t generations;
extern uint32_t M;
extern uint32_t N;
extern uint32_t n_rocks;
extern uint32_t n_rabbits;
extern uint32_t n_foxes;
extern uint32_t rabbit_breeding;
extern uint32_t fox_breeding;
extern uint32_t fox_starvation;
extern uint32_t seed;

void init_message_cell(MessageCell *message, Cell *cell) {
    message->type = cell->type;
    message->new_animals = cell->new_animals;
    // printf("cell new animals:%d\n", cell->new_animals);
    for (int i = 0; i < cell->new_animals; ++i) {
        message->incoming_animals[i].breeding_age = cell->incoming_animals[i]->breeding_age;
        message->incoming_animals[i].modified_by_red = cell->incoming_animals[i]->modified_by_red;
        message->incoming_animals[i].starvation_age = cell->incoming_animals[i]->starvation_age;
        message->incoming_animals[i].type = cell->incoming_animals[i]->type;
    }

    if (cell->animal != NULL && cell->type == ANIMAL) {
        message->animal.type = cell->animal->type;
        message->animal.breeding_age = cell->animal->breeding_age;
        message->animal.starvation_age = cell->animal->starvation_age;
        message->animal.modified_by_red = cell->animal->modified_by_red;
    }
};

void init_message_cell_buffer(MessageCell *buff, Cell *row) {
    for (int j = 0; j < N; ++j) {
        init_message_cell(&buff[j], &row[j]);
    }
}

void convert_buffer_to_row(MessageCell *buff, Cell *row, int returning) {
    for (int j = 0; j < N; ++j) {
        if (returning != 0) {
            row[j].type = buff[j].type;
            row[j].new_animals = 0;
        }

        if (returning != 0 && buff[j].type == ANIMAL) {
            row[j].animal = malloc(sizeof(Animal));
            row[j].animal->breeding_age = buff[j].animal.breeding_age;
            row[j].animal->modified_by_red = buff[j].animal.modified_by_red;
            row[j].animal->starvation_age = buff[j].animal.starvation_age;
            row[j].animal->type = buff[j].animal.type;
        }

        if (returning != 0 && buff[j].type != ANIMAL){
            row[j].animal = NULL;
        }

        for (int i = 0; i < buff[j].new_animals; ++i) {
            row[j].incoming_animals[row[j].new_animals] = malloc(sizeof(Animal));
            row[j].incoming_animals[row[j].new_animals]->type = buff[j].incoming_animals[i].type;
            row[j].incoming_animals[row[j].new_animals]->breeding_age = buff[j].incoming_animals[i].breeding_age;
            row[j].incoming_animals[row[j].new_animals]->starvation_age = buff[j].incoming_animals[i].starvation_age;
            row[j].incoming_animals[row[j].new_animals]->modified_by_red = buff[j].incoming_animals[i].modified_by_red;
    
            row[j].new_animals++;
        }
    }
}
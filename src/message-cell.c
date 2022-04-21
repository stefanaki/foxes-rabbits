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
        Animal *temp = malloc(sizeof(Animal));
        temp->breeding_age = cell->incoming_animals[i]->breeding_age;
        temp->modified_by_red = cell->incoming_animals[i]->modified_by_red;
        temp->starvation_age = cell->incoming_animals[i]->starvation_age;
        temp->type = cell->incoming_animals[i]->type;

        message->incoming_animals[i] = *temp;
    }
    Animal *cell_animal = malloc(sizeof(Animal));
    if (cell->animal != NULL && cell->type == ANIMAL) {
        cell_animal->breeding_age = cell->animal->breeding_age;
        cell_animal->modified_by_red = cell->animal->modified_by_red;
        cell_animal->starvation_age = cell->animal->starvation_age;
        cell_animal->type = cell->animal->type;

        message->animal = *cell_animal;
    }
};

void init_message_cell_buffer(MessageCell *buff, Cell *row) {
    for (int j = 0; j < N; ++j) {
        init_message_cell(&buff[j], &row[j]);
    }
}

void convert_buffer_to_row(MessageCell *buff, Cell *row, int returning) {
    Animal *temp;
    for (int j = 0; j < N; ++j) {
        if (returning != 0) {
            row[j].type = buff[j].type;
            row[j].new_animals = 0;
        }

        if (returning != 0 && buff[j].type == ANIMAL) {
            temp = malloc(sizeof(Animal));
            temp->breeding_age = buff[j].animal.breeding_age;
            temp->modified_by_red = buff[j].animal.modified_by_red;
            temp->starvation_age = buff[j].animal.starvation_age;
            temp->type = buff[j].animal.type;

            row[j].animal = temp;
        }

        for (int i = 0; i < buff[j].new_animals; ++i) {
            Animal *inside_temp = malloc(sizeof(Animal));
            inside_temp->type = buff[j].incoming_animals[i].type;
            inside_temp->breeding_age = buff[j].incoming_animals[i].breeding_age;
            inside_temp->starvation_age = buff[j].incoming_animals[i].starvation_age;
            inside_temp->modified_by_red = buff[j].incoming_animals[i].modified_by_red;
            row[j].incoming_animals[row[j].new_animals++] = inside_temp;
        }
    }
}
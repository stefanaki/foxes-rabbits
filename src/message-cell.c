#include "message-cell.h"
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

void init_message_cell(MessageCell *message, Cell *cell)
{
  message->type = cell->type;
  message->new_animals = cell->new_animals;
  for (int i = 0; i < cell->new_animals; ++i)
    message->incoming_animals[i] = *(cell->incoming_animals[i]);
  message->animal = *(cell->animal);
  return;
};

void init_message_cell_buffer(MessageCell *buff, Cell *row)
{
  for (int j = 0; j < N; ++j)
  {
    init_message_cell(&buff[j], &row[j]);
  }
}

void convert_buffer_to_row(MessageCell *buff, Cell *row)
{
  Animal *temp;

  for (int j = 0; j < N; ++j)
  {
    row[j].type = buff[j].type;

    for (int i = 0; i < buff[j].new_animals; ++i)
    {
      temp = create_animal(buff[j].incoming_animals[i].type);
      temp->breeding_age = buff[j].incoming_animals[i].breeding_age;
      temp->starvation_age = buff[j].incoming_animals[i].starvation_age;
      temp->modified_by_red = buff[j].incoming_animals[i].modified_by_red;

      row[j].incoming_animals[row[j].new_animals + i + 1] = temp;
    }

    row[j].new_animals += buff[j].new_animals;
    if (buff[j].type == ANIMAL)
    {
      temp = create_animal(buff[j].animal.type);
      temp->breeding_age = buff[j].animal.breeding_age;
      temp->starvation_age = buff[j].animal.starvation_age;
      temp->modified_by_red = buff[j].animal.modified_by_red;
      row[j].animal = temp;
    }
  }
}
#include "serial.h"
#include "world.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

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

bool fox_sees_rabbit(Cell *cell, char type)
{
  return type == FOX && cell->animal && animal_type(cell->animal, RABBIT);
}

Cell *compute_next_position(World *world, int i, int j, char animal_type)
{
  int C = i * N + j;
  int p = 0;
  int rabbit_p = 0;
  Cell *available_cells[4];        // top, right, bottom, left
  Cell *available_rabbit_cells[4]; // top, right, bottom, left

  // top
  if (i - 1 >= 0 && world->grid[i - 1][j].type == EMPTY)
  {
    available_cells[p++] = &world->grid[i - 1][j];
  }
  if (i - 1 >= 0 && fox_sees_rabbit(&world->grid[i - 1][j], animal_type))
  {
    available_rabbit_cells[rabbit_p++] = &world->grid[i - 1][j];
  }

  // right
  if (j + 1 < N && world->grid[i][j + 1].type == EMPTY)
  {
    available_cells[p++] = &world->grid[i][j + 1];
  }
  if (j + 1 < N && fox_sees_rabbit(&world->grid[i][j + 1], animal_type))
  {
    available_rabbit_cells[rabbit_p++] = &world->grid[i][j + 1];
  }

  // bottom
  if (i + 1 < M && world->grid[i + 1][j].type == EMPTY)
  {
    available_cells[p++] = &world->grid[i + 1][j];
  }
  if (i + 1 < M && fox_sees_rabbit(&world->grid[i + 1][j], animal_type))
  {
    available_rabbit_cells[rabbit_p++] = &world->grid[i + 1][j];
  }

  // left
  if (j - 1 >= 0 && world->grid[i][j - 1].type == EMPTY)
  {
    available_cells[p++] = &world->grid[i][j - 1];
  }
  if (j - 1 >= 0 && fox_sees_rabbit(&world->grid[i][j - 1], animal_type))
  {
    available_rabbit_cells[rabbit_p++] = &world->grid[i][j - 1];
  }

  if (p == 0 && rabbit_p == 0)
  {
    return NULL;
  }

  int n = rabbit_p > 0 ? rabbit_p : p;

  int res = C % n;

  if (rabbit_p > 0)
  {
    return available_rabbit_cells[res];
  }

  return available_cells[res];
}

void resolve_conflicts(Cell *cell, int turn)
{

  // Function that resolves conflicts that might appear on a cell
  Animal *incoming;
  if (cell->new_animals == 0)
  {
    cell->type = cell->animal ? ANIMAL : EMPTY;
    return;
  }

  for (int i = 0; i < cell->new_animals; i++)
  {
    incoming = cell->incoming_animals[i];
    if (cell->animal == NULL)
    {
      cell->animal = incoming;
      cell->type = ANIMAL;
      continue;
    }

    if (incoming != NULL)
    {
      if (cell->animal->type == FOX)
      {
        if (incoming->type == RABBIT)
        {
          // Animal on cell is FOX and incoming animal is RABBIT = FOX eats RABBIT
          // Update: I think this will never happen
          cell->animal->starvation_age = 0;
        }
        else
        {
          // Animal on cell is FOX and incoming animal is
          // FOX = FOX with largest starvation age survives
          if (cell->animal->starvation_age > incoming->starvation_age)
          {
            incoming = NULL;
            cell->animal->starvation_age = 0;
          }
          else if (incoming->starvation_age > cell->animal->starvation_age)
          {
            cell->animal = incoming;
            incoming->starvation_age = 0;
          }
          else
          {
            // Check for the largest breeding age
            if (cell->animal->breeding_age > incoming->breeding_age)
            {
              incoming = NULL;
              cell->animal->starvation_age = 0;
            }
            else if (incoming->breeding_age > cell->animal->breeding_age)
            {
              cell->animal = incoming;
              incoming->starvation_age = 0;
            }
            else
            {
              // Both foxes die
              incoming = NULL;
              cell->type = EMPTY;
            }
          }
        }
      }
      else
      {
        if (incoming->type == FOX)
        {
          // Animal on cell is RABBIT and incoming animal is
          // FOX = FOX eats RABBIT
          cell->animal = incoming;
          incoming->starvation_age = 0;
        }
        else
        {
          // Animal on cell is RABBIT and incoming animal is
          // RABBIT = RABBIT with largest breeding age survives
          if (cell->animal->breeding_age >= incoming->breeding_age)
          {
            incoming = NULL;
          }
          else
          {
            cell->animal = incoming;
          }
        }
      }
    }
  }

  if (cell->animal)
  {
    // check for death
    if (starvation_status(cell->animal))
    {
      kill_animal(cell);
    }
  }

  // reset turn
  if (cell->animal && turn == 1)
  {
    cell->animal->modified_by_red = false;
  }

  // reset new animal array
  cell->new_animals = 0;
}

void serial_implementation(World *world)
{
  int i, j, gen, turn, col_offset;
  Cell *initial_pos = NULL, *landing_pos = NULL;

  for (gen = 0; gen < generations; ++gen)
  {
    for (turn = 0; turn < 2; ++turn)
    {
      col_offset = turn;
      for (i = 0; i < M; ++i)
      {
        for (j = col_offset; j < N; j += 2)
        {
          initial_pos = &world->grid[i][j];

          if (initial_pos->type != ANIMAL)
            continue;

          if (initial_pos->animal->modified_by_red && turn == 1)
          {
            initial_pos->animal->modified_by_red = false;
            continue;
          }

          // mark animal as modified
          if (turn == 0)
          {
            initial_pos->animal->modified_by_red = true;
          }

          landing_pos =
              compute_next_position(world, i, j, initial_pos->animal->type);

          // increase ages
          initial_pos->animal->breeding_age++;
          initial_pos->animal->starvation_age++;

          // move animal
          if (landing_pos != NULL)
          {
            if (initial_pos->animal->type == FOX && initial_pos->animal->breeding_age >= fox_breeding)
            {
              initial_pos->incoming_animals[initial_pos->new_animals++] = create_animal(FOX);
              initial_pos->animal->breeding_age = 0;
            }
            else if (initial_pos->animal->type == RABBIT && initial_pos->animal->breeding_age >= rabbit_breeding)
            {
              initial_pos->incoming_animals[initial_pos->new_animals++] = create_animal(RABBIT);
              initial_pos->animal->breeding_age = 0;
            }

            landing_pos->incoming_animals[landing_pos->new_animals++] = initial_pos->animal;

            initial_pos->type = EMPTY;
            initial_pos->animal = NULL;
          }
        }
        col_offset = col_offset == 0 ? 1 : 0;
      }

      for (int k = 0; k < M; k++)
      {
        for (int l = 0; l < N; l++)
        {
          if (world->grid[k][l].type != ROCK)
          {
            resolve_conflicts(&world->grid[k][l], turn);
          }
        }
      }

      // Printing board
      printf("===========\nGen %d, Subgen %s\n", gen + 1, turn == 0 ? "red" : "black");
      for (int k = 0; k < M; k++)
      {
        for (int l = 0; l < N; l++)
        {
          if (world->grid[k][l].type == ANIMAL &&
              world->grid[k][l].animal->type == FOX)
            printf("F %d %d\t", world->grid[k][l].animal->starvation_age, world->grid[k][l].animal->breeding_age);
          else if (world->grid[k][l].type == ANIMAL && world->grid[k][l].animal->type == RABBIT)
            printf("R\t");
          else if (world->grid[k][l].type == ROCK)
            printf("*\t");
          else
            printf("-\t");
        }
        printf("\n");
      }
    }
  }
}
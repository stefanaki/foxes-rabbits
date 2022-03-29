#include "serial.h"
#include "world.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>

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

extern uint8_t debug;

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
    available_cells[p++] = &world->grid[i - 1][j];
  else if (i - 1 >= 0 && fox_sees_rabbit(&world->grid[i - 1][j], animal_type))
    available_rabbit_cells[rabbit_p++] = &world->grid[i - 1][j];

  // right
  if (j + 1 < N && world->grid[i][j + 1].type == EMPTY)
    available_cells[p++] = &world->grid[i][j + 1];
  else if (j + 1 < N && fox_sees_rabbit(&world->grid[i][j + 1], animal_type))
    available_rabbit_cells[rabbit_p++] = &world->grid[i][j + 1];

  // bottom
  if (i + 1 < M && world->grid[i + 1][j].type == EMPTY)
    available_cells[p++] = &world->grid[i + 1][j];
  else if (i + 1 < M && fox_sees_rabbit(&world->grid[i + 1][j], animal_type))
    available_rabbit_cells[rabbit_p++] = &world->grid[i + 1][j];

  // left
  if (j - 1 >= 0 && world->grid[i][j - 1].type == EMPTY)
    available_cells[p++] = &world->grid[i][j - 1];
  else if (j - 1 >= 0 && fox_sees_rabbit(&world->grid[i][j - 1], animal_type))
    available_rabbit_cells[rabbit_p++] = &world->grid[i][j - 1];

  if (p == 0 && rabbit_p == 0)
    return NULL;

  int n = rabbit_p > 0 ? rabbit_p : p;
  int res = C % n;

  return (rabbit_p > 0) ? available_rabbit_cells[res] : available_cells[res];
}

void resolve_merge(Animal **current, Animal *incoming)
{
  if ((*current)->type == FOX && incoming->type == RABBIT)
  {
    free(incoming);
    incoming = NULL;
    (*current)->starvation_age = 0;
  }
  else if ((*current)->type == FOX && incoming->type == FOX)
  {
    if (!(*current)->starvation_age)
    {
      free(incoming);
      incoming = NULL;
      (*current)->starvation_age = 0;
    }
    else if (!incoming->starvation_age)
    {
      *current = incoming;
      incoming->starvation_age = 0;
    }
    else if ((*current)->breeding_age > incoming->breeding_age)
    {
      free(incoming);
      incoming = NULL;
    }
    else if (incoming->breeding_age > (*current)->breeding_age)
    {
      *current = incoming;
    }
    else if ((*current)->starvation_age < incoming->starvation_age)
    {
      free(incoming);
      incoming = NULL;
    }
    else if (incoming->starvation_age < (*current)->starvation_age)
    {
      *current = incoming;
    }
  }
  else if ((*current)->type == RABBIT && incoming->type == FOX)
  {
    *current = incoming;
    incoming->starvation_age = 0;
  }
  else if ((*current)->type == RABBIT && incoming->type == RABBIT)
  {
    if ((*current)->breeding_age >= incoming->breeding_age)
    {
      free(incoming);
      incoming = NULL;
    }
    else
    {
      *current = incoming;
    }
  }
}

void resolve_conflicts(Cell *cell)
{

  if (!cell->new_animals)
  {
    cell->type = (cell->animal) ? ANIMAL : EMPTY;
    return;
  }

#pragma omp parallel for
  for (int i = 0; i < cell->new_animals; i++)
  {
    // Function that resolves conflicts that might appear on a cell
    {
      if (!cell->animal)
      {
        modify_cell(cell, ANIMAL, cell->incoming_animals[i]);
        continue;
      }
    }

    resolve_merge(&cell->animal, cell->incoming_animals[i]);
  }
}

void parallel_implementation(World *world)
{
  bool col_offset;
  int i, j, gen, turn;
  // Cell *initial_pos = NULL, *landing_pos = NULL;

  for (gen = 0; gen < generations; ++gen)
  {
    for (turn = 0; turn < 2; ++turn)
    {
#pragma omp parallel for private(col_offset, i, j)
      for (i = 0; i < M; ++i)
      {
        col_offset = turn == 1 ? i % 2 == 0 : i % 2 == 1;
        for (j = col_offset; j < N; j += 2)
        {
          Cell *initial_pos = &world->grid[i][j];

          if (initial_pos->type != ANIMAL)
            continue;
          else if (turn && initial_pos->animal->modified_by_red)
          {
            initial_pos->animal->modified_by_red = false;
            continue;
          }

          // increase starvation
          initial_pos->animal->starvation_age++;
          initial_pos->animal->breeding_age++;

          Cell *landing_pos;
          landing_pos = compute_next_position(world, i, j, initial_pos->animal->type);

          // move animal
          if (landing_pos)
          {
            // mark animal as modified
            initial_pos->animal->modified_by_red = !turn;

            if (breeding_status(initial_pos->animal))
            {
              Animal *aux = create_animal(initial_pos->animal->type);
              aux->modified_by_red = !turn;

              change_breeding_age(initial_pos->animal, 0);
              initial_pos->incoming_animals[initial_pos->new_animals++] = aux;
            }
            // refactor this to instead of having an array, we copy the world for each cell and we do merges like we did before
            landing_pos->incoming_animals[landing_pos->new_animals++] = initial_pos->animal;

            modify_cell(initial_pos, EMPTY, NULL);
          }
        }
      }

#pragma omp parallel for
      for (int k = 0; k < M; k++)
      {
        for (int l = 0; l < N; l++)
        {
          if (world->grid[k][l].type != ROCK)
          {

            resolve_conflicts(&world->grid[k][l]);

            // reset new animal array
            world->grid[k][l].new_animals = 0;

            if (turn && world->grid[k][l].animal) // Last Sub_Gen and animal
            {
              if (starvation_status(world->grid[k][l].animal))
                kill_animal(&world->grid[k][l]);

              // reset turn
              if (world->grid[k][l].animal) // CANNOT RESET TO AN EMPTY CELL
                world->grid[k][l].animal->modified_by_red = false;
            }
          }
        }
      }

      // Printing board
      if (debug)
      {
        print_board(world, gen, turn);
      }
    }
  }
}

void print_board(World *world, int gen, int turn)
{
  printf("Generation %d, %s\n", gen + 1, (!turn) ? "red" : "black");
  for (int l = 0; l <= N; l++)
    printf("---");
  printf("\n   ");
  for (int l = 0; l < N; l++)
    printf("%02d|", l);
  for (int k = 0; k < M; k++)
  {
    printf("\n");
    printf("0%d:", k);
    for (int l = 0; l < N; l++)
    {
      if (world->grid[k][l].type == ANIMAL && world->grid[k][l].animal->type == FOX)
        printf(" F");
      // printf("%d%d", world->grid[k][l].animal->breeding_age, world->grid[k][l].animal->starvation_age);
      else if (world->grid[k][l].type == ANIMAL && world->grid[k][l].animal->type == RABBIT)
        printf(" R");
      //  printf("0%d", world->grid[k][l].animal->breeding_age);
      else if (world->grid[k][l].type == ROCK)
        printf(" *");
      else
        printf("  ");
      printf("|");
    }
  }
  printf("\n");
  for (int l = 0; l <= N; l++)
    printf("---");
  printf("\n");
}

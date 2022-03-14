#include <stddef.h>
#include "serial.h"
#include "world.h"

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

Cell *compute_next_position(World *world, int i, int j, char animal_type) {
  int C = i * N + j;
  int p = 0;
  int available_cells[4] =
      { false, false, false, false }; // top, right, bottom, left

  if (i - 1 >= 0 && world->grid[i - 1][j].type == EMPTY) {
    ++p;
    available_cells[0] = true;
  } else if (i - 1 >= 0 && animal_type == FOX && world->grid[i - 1][j].animal &&
             world->grid[i - 1][j].animal->type == RABBIT)
    return &world->grid[i - 1][j];

  if (j + 1 < N && world->grid[i][j + 1].type == EMPTY) {
    ++p;
    available_cells[0] = true;
  } else if (j + 1 < N && animal_type == FOX && world->grid[i][j + 1].animal &&
             world->grid[i][j + 1].animal->type == RABBIT)
    return &world->grid[i][j + 1];

  if (i + 1 < M && world->grid[i + 1][j].type == EMPTY) {
    ++p;
    available_cells[1] = true;
  } else if (i + 1 < M && animal_type == FOX && world->grid[i + 1][j].animal &&
             world->grid[i + 1][j].animal->type == RABBIT)
    return &world->grid[i + 1][j];

  if (j - 1 >= 0 && world->grid[i][j - 1].type == EMPTY) {
    ++p;
    available_cells[2] = true;
  } else if (j - 1 >= 0 && animal_type == FOX && world->grid[i][j - 1].animal &&
             world->grid[i][j - 1].animal->type == RABBIT)
    return &world->grid[i][j - 1];

  int res = C % p, idx = 0;
  while (idx < res) {
    if (!available_cells[idx])
      ++idx;
  }

  if (idx == 0)
    return &world->grid[i - 1][j];
  else if (idx == 1)
    return &world->grid[i][j + 1];
  else if (idx == 2)
    return &world->grid[i + 1][j];
  else
    return &world->grid[i][j - 1];
}

void resolve_conflicts(Cell *c_1, Cell *c_2) {}

void serial_implementation(World *world) {
  int i, j, gen, turn, col_offset;
  Cell *initial_pos, *landing_pos;

  for (gen = 0; gen < generations; ++gen) {
    for (turn = 0; turn < 2; ++turn) {
      col_offset = turn;
      for (i = 0; i < M; ++i) {
        for (j = col_offset; j < N; j += 2) {
          initial_pos = &world->grid[i][j];
          if (initial_pos->modified_by_red || !initial_pos->animal)
            continue;

          if (initial_pos->animal->type == FOX &&
              initial_pos->animal->starvation_age == fox_starvation) {
            kill_animal(initial_pos);
          }

          landing_pos =
              compute_next_position(world, i, j, initial_pos->animal->type);

          if (landing_pos != NULL) {
            landing_pos->incoming_animal = initial_pos->animal;
            initial_pos->modified_by_red = true;

            if (landing_pos->animal->type == RABBIT &&
                landing_pos->animal->breeding_age >= rabbit_breeding) {
              insert_animal(initial_pos, RABBIT);
              landing_pos->animal->breeding_age = 0;
            } else if (landing_pos->animal->type == FOX &&
                       landing_pos->animal->breeding_age >= fox_breeding) {
              insert_animal(initial_pos, FOX);
              landing_pos->animal->breeding_age = 0;
            } else { // Animal moves to neighbouring cell but does not breed
              initial_pos->animal = NULL;
              landing_pos->animal->breeding_age++;
            }
          } else { // Animal stays in current cell
            initial_pos->animal->breeding_age++;
          }
          if (landing_pos->incoming_animal->type == FOX)
            landing_pos->incoming_animal->starvation_age++;
        }
        col_offset = col_offset == 0 ? 1 : 0;
      }
      resolve_conflicts(initial_pos, landing_pos);
    }
  }
}
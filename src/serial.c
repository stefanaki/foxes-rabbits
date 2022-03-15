#include "serial.h"
#include "world.h"
#include <stddef.h>
#include <stdio.h>

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

bool fox_sees_rabbit(Cell *cell, char type) {
  return type == FOX && cell->animal && animal_type(cell->animal, RABBIT);
}

Cell *compute_next_position(World *world, int i, int j, char animal_type) {
  int C = i * N + j;
  int p = 0;
  bool available_cells[4] = {false, false, false,
                            false}; // top, right, bottom, left

  if (i - 1 >= 0 && world->grid[i - 1][j].type == EMPTY) {
    ++p;
    available_cells[0] = true;
  }
  if (i - 1 >= 0 && fox_sees_rabbit(&world->grid[i - 1][j], animal_type))
    return &world->grid[i - 1][j];

  if (j + 1 < N && world->grid[i][j + 1].type == EMPTY) {
    ++p;
    available_cells[1] = true;
  } 
  if (j + 1 < N && fox_sees_rabbit(&world->grid[i][j + 1], animal_type))
    return &world->grid[i][j + 1];

  if (i + 1 < M && world->grid[i + 1][j].type == EMPTY) {
    ++p;
    available_cells[2] = true;
  } 
  if (i + 1 < M && fox_sees_rabbit(&world->grid[i + 1][j], animal_type))
    return &world->grid[i + 1][j];

  if (j - 1 >= 0 && world->grid[i][j - 1].type == EMPTY) {
    ++p;
    available_cells[3] = true;
  }
  if (j - 1 >= 0 && fox_sees_rabbit(&world->grid[i][j - 1], animal_type))
    return &world->grid[i][j - 1];

  if (p == 0)
    return NULL;

  int res = C % p, idx = 0;
  while (idx < res) {
    printf("res: %d\tidx: %d\n", res, idx);
    if (available_cells[idx])
      ++idx;
  }

  if (idx == 1)
    return &world->grid[i - 1][j];
  else if (idx == 2)
    return &world->grid[i][j + 1];
  else if (idx == 3)
    return &world->grid[i + 1][j];
  else
    return &world->grid[i][j - 1];
}

void resolve_conflicts(Cell *cell) {
  // Function that resolves conflicts that might appear on a cell
  Animal *current, *incoming;

  if (!cell || cell->type == EMPTY)
    return;

  current = cell->animal;
  if ((incoming = cell->incoming_animal) != NULL) {
    if (current->type == FOX) {
      if (incoming->type == RABBIT) {
        // Animal on cell is FOX and incoming animal is RABBIT = FOX eats RABBIT
        // Update: I think this will never happen
        kill_animal(cell);
        cell->animal = incoming;
        incoming->starvation_age = 0;
      } else {
        // Animal on cell is FOX and incoming animal is
        // FOX = FOX with largest starvation age survives
        if (current->starvation_age > incoming->starvation_age) {
          free(incoming);
          current->starvation_age = 0;
        } else if (incoming->starvation_age > current->starvation_age) {
          kill_animal(cell);
          cell->animal = incoming;
          incoming->starvation_age = 0;
        } else {
          // Check for the largest breeding age
          if (current->breeding_age > incoming->breeding_age) {
            free(incoming);
            current->starvation_age = 0;
          } else if (incoming->breeding_age > current->breeding_age) {
            kill_animal(cell);
            cell->animal = incoming;
            incoming->starvation_age = 0;
          } else {
            // Both foxes die
            kill_animal(cell);
            free(incoming);
            cell->type = EMPTY;
          }
        }
      }
    } else {
      if (incoming->type == FOX) {
        // Animal on cell is RABBIT and incoming animal is
        // FOX = FOX eats RABBIT
        kill_animal(cell);
        cell->animal = incoming;
        incoming->starvation_age = 0;
        cell->type = FOX;
      } else {
        // Animal on cell is RABBIT and incoming animal is
        // RABBIT = RABBIT with largest breeding age survives
        if (current->breeding_age >= incoming->breeding_age) {
          free(incoming);
        } else if (incoming->breeding_age > current->breeding_age) {
          kill_animal(cell);
          cell->animal = incoming;
        }
      }
    }
  }

  cell->incoming_animal = NULL;
}

void serial_implementation(World *world) {
  int i, j, gen, turn, col_offset;
  Cell *initial_pos = NULL, *landing_pos = NULL;

  printf("ok\n");

  for (gen = 0; gen < generations; ++gen) {
    for (turn = 0; turn < 2; ++turn) {
      col_offset = turn;
      printf("%d\n", turn);
      for (i = 0; i < M; ++i) {
        for (j = col_offset; j < N; j += 2) {
          printf("%d\t%d\n", i, j);
          initial_pos = &world->grid[i][j];

          if (initial_pos->type == EMPTY)
            continue;

          printf("animal type: %d\n", initial_pos->animal->type);

          if (initial_pos->modified_by_red || !initial_pos->animal) {
            initial_pos->modified_by_red = false;
            continue;
          }

          if (starvation_status(initial_pos->animal)) {
            printf("time to die\n");
            kill_animal(initial_pos);
            continue;
          }

          printf("ok until here\n");

          landing_pos =
              compute_next_position(world, i, j, initial_pos->animal->type);

          if (landing_pos)
            printf("ok next pos\npos_animal: %d\n", landing_pos->type);

          if (landing_pos != NULL) {
            landing_pos->incoming_animal = initial_pos->animal;
            initial_pos->modified_by_red = true;

            // move_animal(initial_pos, landing_pos); // why???
            initial_pos->type = EMPTY;
            initial_pos->animal = NULL;

          } else { // Animal stays in current cell
            initial_pos->animal->breeding_age++;
          }

          printf("okokokok\n");

          if (landing_pos && animal_type(landing_pos->incoming_animal, FOX))
            landing_pos->incoming_animal->starvation_age++;

          printf("okokokok1234\n");

          resolve_conflicts(initial_pos);
          resolve_conflicts(landing_pos);
        }
        col_offset = col_offset == 0 ? 1 : 0;
      }
    }
  }
}
#ifndef MPI_IMPL_LIB_H__
#define MPI_IMPL_LIB_H__

#include <mpi.h>
#include "cell.h"
#include "world.h"
#include "worldgen.h"

void generate_element_mpi(int, char, uint32_t *, Cell **, int, int);

// Generate world subgrid based on process id
Cell **generate_world_subgrid(int, int);

// Compute next position modified
void compute_next_position(Cell **, int, int, char, int, int, Cell *, int *);

// Ask for data from cell that doesn't belong in the same process
Cell get_cell_from_rank(int, int, int);

// Modify cell that doesn't belong to the same process
void modify_cell_from_rank(int, int, int, Cell);

// Resolve conflicts modified
void resolve_conflicts(Cell *);

// Send generation result to master
void send_result_to_master(Cell **, int, int, int *);

// Implementation and process reduction
void mpi_implementation(Cell **, int, int, MPI_Datatype, int *);

// Print the board
void print_board(Cell **, int, int, int);

#endif /* MPI_IMPL_LIB_H__ */
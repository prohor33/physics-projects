#include "manager.h"
#include "solver.h"
#include "grid.h"
#include "cell.h"
#include "parameters.h"
#include "out_result.h"

void Manager::Initialize(int argc, char *argv[]) {

  SetParameters();

  MPI_Init (&argc, &argv);  /* starts MPI */

  int rank, size;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);  /* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size);  /* get number of processes */

  PARAMETERS->SetUseParallelComputing(size == 0);

  if (PARAMETERS->GetUseParallelComputing()) {
    cout << "number of processes = " << size << endl;
    cout << "current process id = " << rank << endl;
  }

  PARAMETERS->SetProcessID(rank);

  InitializeSolver();
}

void Manager::InitializeSolver() {

  // Here we should initialize only
  // grid for this process, it's depends on process_id

  int rank = PARAMETERS->GetProcessID();

  vector<int> grid_size(3);

  grid_size[sep::X] = 10;
  grid_size[sep::Y] = 10;
  grid_size[sep::Z] = 10;

  vector<int> grid_start(3);

  grid_start[sep::X] = 10;
  grid_start[sep::Y] = 10;
  grid_start[sep::Z] = 10;

  if (PARAMETERS->GetGridFromInputFile())
    SOLVER->grid_ = new Grid("cube.grid", grid_start, grid_size);
  else
    SOLVER->grid_ = new Grid();

  int some_magic_numbers = 0; // this numbers depend on
  // process_id and grids map configuration

  SOLVER->SetGridNeighbors(
            GridNeighbor(some_magic_numbers, some_magic_numbers),
            GridNeighbor(some_magic_numbers, some_magic_numbers),
            GridNeighbor(some_magic_numbers, some_magic_numbers)
            );

}

void Manager::SetParameters() {
  // we will use this function if main appear need to be clear
}

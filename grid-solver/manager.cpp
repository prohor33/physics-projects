#include "manager.h"
#include "solver.h"
#include "grid.h"
#include "cell.h"
#include "parameters.h"
#include "out_result.h"
#include "grid_file_reader.h"


void Manager::Initialize(int argc, char *argv[]) {

  SetParameters();

  MPI_Init (&argc, &argv);  /* starts MPI */

  int rank, size;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);  /* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size);  /* get number of processes */

  PARAMETERS->SetUseParallelComputing(size != 1);

  if (PARAMETERS->GetUseParallelComputing()) {
    cout << "size = " << size << endl;
    cout << "rank = " << rank << endl;
  }

  PARAMETERS->SetProcessID(rank);

  InitializeSolver();

  cout << "Initialization complete" << endl;
}

void Manager::InitializeSolver() {

  if (!PARAMETERS->GetGridFromInputFile()) {

    SOLVER->SetGridNeighbors(
              GridNeighbor(-1, -1),
              GridNeighbor(-1, -1),
              GridNeighbor(-1, -1)
              );

    SOLVER->grid_ = new Grid();

    return;
  }

  // Here we should initialize only
  // grid for this process, it's depends on process_id

  int rank = PARAMETERS->GetProcessID();
  int size = PARAMETERS->GetProcessesQ();

  GRID_FILE_READER->ReadFile("our_small.grid");

  int n, m, p;

  n = GRID_FILE_READER->grid_config()->size[sep::X];
  m = GRID_FILE_READER->grid_config()->size[sep::Y];
  p = GRID_FILE_READER->grid_config()->size[sep::Z];

  // TODO: here should be size of provess grid, not whole grd
  SOLVER->SetWholeCellsQuantity(n * m * p);

  vector<int> grid_size(3);
  vector<int> grid_start(3);

  if (!PARAMETERS->GetUseParallelComputing()) {
    // Singletone case

    SOLVER->SetGridNeighbors(
              GridNeighbor(-1, -1),
              GridNeighbor(-1, -1),
              GridNeighbor(-1, -1)
              );

    grid_start[sep::X] = 0;
    grid_start[sep::Y] = 0;
    grid_start[sep::Z] = 0;

    grid_size[sep::X] = n;
    grid_size[sep::Y] = m;
    grid_size[sep::Z] = p;

    SOLVER->grid_ = new Grid(grid_start, grid_size);

    return;
  }

  // Parallel computing case

  if (rank == size-1) {
    // last process

    grid_size[sep::X] = n - n/size*(size-1);
  } else {

    grid_size[sep::X] = n/size;
  }

  grid_size[sep::Y] = m;
  grid_size[sep::Z] = p;

  grid_start[sep::X] = rank * n/size;
  grid_start[sep::Y] = 0;
  grid_start[sep::Z] = 0;

  cout << "rank " << rank << ": start = " << grid_start[sep::X] <<
    " size = " << grid_size[sep::X] << endl;

  SOLVER->SetGridNeighbors(
            GridNeighbor(rank > 0 ? rank-1 : -1, rank < size-1 ? rank+1 : -1),
            GridNeighbor(-1, -1),
            GridNeighbor(-1, -1)
            );

  SOLVER->grid_ = new Grid(grid_start, grid_size);
}

void Manager::SetParameters() {
  // we will use this function if main appear need to be clear
}

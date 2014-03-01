#include "manager.h"
#include "solver.h"
#include "grid.h"
#include "cell.h"
#include "parameters.h"
#include "out_result.h"
#include "grid_file_reader.h"
#include "parallel.h"
#include "flow_keeper.h"

void Manager::Initialize(int argc, char *argv[]) {

  PARAMETERS->OutTime();

  SetParameters();

  PARALLEL->InitializeMPI(argc, argv);

  InitializeSolver();

  // must be after solver initialization
  if (PARAMETERS->GetUseFlowKeeper())
    FLOW_KEEPER->InitFlowAreas();

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

  GRID_FILE_READER->ReadFile(PARAMETERS->GetInputGridFilename());

  int n, m, p;

  n = GRID_FILE_READER->grid_config()->size[sep::X];
  m = GRID_FILE_READER->grid_config()->size[sep::Y];
  p = GRID_FILE_READER->grid_config()->size[sep::Z];

  // TODO: here should be size of process grid, not whole grid
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

  int rank = PARAMETERS->GetProcessID();
  int size = PARAMETERS->GetProcessesQ();

  int norm_size = n/size + 1;

  if (rank == size-1) {
    // last process

    grid_size[sep::X] = n - norm_size*(size-1);
  } else {

    grid_size[sep::X] = norm_size;
  }

  grid_size[sep::Y] = m;
  grid_size[sep::Z] = p;

  grid_start[sep::X] = rank * norm_size;
  grid_start[sep::Y] = 0;
  grid_start[sep::Z] = 0;

  cout << "rank " << rank << ": start = " << grid_start[sep::X] <<
    " size = " << grid_size[sep::X] << endl;

  SOLVER->SetGridNeighbors(
            GridNeighbor(rank < size-1 ? rank+1 : -1, rank > 0 ? rank-1 : -1),
            GridNeighbor(-1, -1),
            GridNeighbor(-1, -1)
            );

  SOLVER->grid_ = new Grid(grid_start, grid_size);
}

void Manager::SetParameters() {
  // we will use this function if main appear need to be clear
}

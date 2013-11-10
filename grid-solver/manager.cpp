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

  PARAMETERS->SetProcessID(rank);

  InitializeSolver();


}

void Manager::InitializeSolver() {

  // Here we should initialize only
  // grid for this process, it's depends on process_id

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

  PARAMETERS->SetMolMass(1, 2);
  PARAMETERS->SetTimeStep(0.1);

  // without second gas for a while
  PARAMETERS->SetSecondGasIsActive(false);

  OUT_RESULT->SetOutputType(OutResult::OUT_FOR_PYTHON);

}

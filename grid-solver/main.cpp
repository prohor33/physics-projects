#include "main.h"
#include "solver.h"
#include "parameters.h"
#include "manager.h"


int main(int argc, char *argv[]) {

  MANAGER->Initialize(argc, argv);

  SOLVER->RunComputation();

  return 0;
}

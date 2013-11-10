#include "main.h"
#include "solver.h"
#include "parameters.h"
#include "initializer.h"


int main(int argc, char *argv[]) {

  PARAMETERS->SetMolMass(1, 2);
  PARAMETERS->SetTimeStep(0.1);

  INITIALIZER->InitializeSolver();

  return 0;
}

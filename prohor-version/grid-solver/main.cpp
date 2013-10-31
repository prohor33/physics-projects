#include "main.h"
#include "solver.h"
#include "parameters.h"

int main(int argc, char *argv[]) {

  PARAMETERS->SetMolMass(1, 2);
  PARAMETERS->SetTimeStep(0.1);

  return 0;
}

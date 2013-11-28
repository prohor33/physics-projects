#include "main.h"
#include "solver.h"
#include "parameters.h"
#include "manager.h"
#include "out_result.h"


int main(int argc, char *argv[]) {

  PARAMETERS->SetMolMass(1.0, 2.0);
  PARAMETERS->SetTimeStep(0.02);

  MANAGER->Initialize(argc, argv);

  cout << "initialization complete" << endl;

  SOLVER->Compute();

  OUT_RESULT->SetOutputType(OutResult::OUT_FOR_PYTHON);
  OUT_RESULT->ProcessParameters(sep::FIRST);
  OUT_RESULT->OutParameters(sep::FIRST);

  return 0;
}

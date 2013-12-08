#include "main.h"
#include "solver.h"
#include "parameters.h"
#include "manager.h"
#include "out_result.h"

int main(int argc, char *argv[]) {

  PARAMETERS->SetLimiter(sep::MC);
  PARAMETERS->SetMolMass(1.0, 2.0);
  PARAMETERS->SetTimeStep(0.02/4.8);

  // without second gas for a while
  PARAMETERS->SetSecondGasIsActive(false);

  PARAMETERS->SetUseCheckingMassConservation(false); // 1e-13 now

  PARAMETERS->SetUseZAxis(true);

  PARAMETERS->SetGridFromInputFile(true);

  MANAGER->Initialize(argc, argv);

  cout << "initialization complete" << endl;

  SOLVER->Compute();

  OUT_RESULT->ProcessParameters(sep::FIRST);
  OUT_RESULT->SetOutputType(OutResult::OUT_FOR_PYTHON);
  OUT_RESULT->OutParameters(sep::FIRST);

  return 0;
}

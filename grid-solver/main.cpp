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
  PARAMETERS->SetSecondGasIsActive(true);

  PARAMETERS->SetUseCheckingMassConservation(false); // 1e-13 now

  PARAMETERS->SetUseZAxis(false);

  MANAGER->Initialize(argc, argv);

  cout << "initialization complete" << endl;

  SOLVER->Compute();

  OUT_RESULT->ProcessParameters(sep::SECOND);
  OUT_RESULT->SetOutputType(OutResult::OUT_FOR_PYTHON);
  OUT_RESULT->OutParameters(sep::SECOND);

  return 0;
}

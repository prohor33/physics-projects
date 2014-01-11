#include "main.h"
#include "solver.h"
#include "parameters.h"
#include "manager.h"
#include "out_result.h"
#include "parallel.h"


int main(int argc, char *argv[]) {

  PARAMETERS->SetLimiter(sep::MC);
  PARAMETERS->SetMolMass(1.0, 2.0);
  PARAMETERS->SetTimeStep(0.02/4.8);
  PARAMETERS->SetSpeedQuantity(16); // for debug (should be 20)
  PARAMETERS->SetPCut(4.8);

  // without second gas for a while
  PARAMETERS->SetSecondGasIsActive(false);

  PARAMETERS->SetUseCheckingMassConservation(false); // delta is 1e-13 now

  PARAMETERS->SetUseZAxis(true);

  PARAMETERS->SetGridFromInputFile(true);

  PARAMETERS->SetUseCollisionIntegral(false);

  OUT_RESULT->SetOutputType(OutResult::OUT_FOR_PYTHON);

  PARAMETERS->Initialize(); // should be first initialization

  MANAGER->Initialize(argc, argv);

  SOLVER->Compute();

  //OUT_RESULT->ProcessParameters(sep::FIRST);
  //OUT_RESULT->SetOutputType(OutResult::OUT_FOR_PYTHON);
  //OUT_RESULT->OutParameters(sep::FIRST);

  // TODO: should translate this ti SOLVER->Finalize() o smt like that
  PARALLEL->FinalizeMPI();

  return 0;
}

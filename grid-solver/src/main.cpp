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
  PARAMETERS->SetT1T2(1.0, 0.8);
  PARAMETERS->SetStartFlow(0.1);

  PARAMETERS->SetInputGridFilename("../config/income.grid");

  // without second gas for a while
  PARAMETERS->SetSecondGasIsActive(false);

  PARAMETERS->SetUseCheckingMassConservation(true); // delta is 1e-13 now
  PARAMETERS->SetUseZAxis(false);
  PARAMETERS->SetGridFromInputFile(true);
  PARAMETERS->SetUseStartTemperature(true);
  PARAMETERS->SetUseFlowKeeper(false);

  PARAMETERS->SetUseCollisionIntegral(false);

  //PARAMETERS->SetLogType(sep::LOG_FILE);
  PARAMETERS->SetLogType(sep::CONSOLE);

  PARAMETERS->SetIterationQ(10);

  // output type will changed wether singletone mode is active
  OUT_RESULT->SetOutputType(sep::OUT_BINARY_FILE);

  PARAMETERS->Initialize(); // should be first initialization

  MANAGER->Initialize(argc, argv);

  SOLVER->Compute();

  // TODO: should translate this to SOLVER->Finalize() or smth like that
  PARALLEL->FinalizeMPI();

  return 0;
}

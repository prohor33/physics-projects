#include "main.h"
#include "solver.h"
#include "parameters.h"
#include "manager.h"
#include "out_result.h"


int main(int argc, char *argv[]) {

  MANAGER->Initialize(argc, argv);

  cout << "initialization complete" << endl;

  SOLVER->Compute();

  OUT_RESULT->SetOutputType(OutResult::OUT_FOR_PYTHON);
  OUT_RESULT->ProcessParameters();
  OUT_RESULT->OutParameters();

  return 0;
}

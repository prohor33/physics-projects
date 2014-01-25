#include "solver.h"
#include "out_result.h"
#include "parameters.h"
#include "integral.h"
#include "parallel.h"

string sep::int_to_string(int i) {

    stringstream ss;
    std::string s;
    ss << i;
    s = ss.str();

    return s;
};


void Solver::Compute() {

  OUT_RESULT->CheckMassConservation(sep::FIRST);

  MeasureTime();

  int iterations = PARAMETERS->GetIterationQ();

  PARAMETERS->OutTime();

  cout << "Let's do " << iterations << " iterations. ";
  cout << "Start computing..." << endl;

  for (int i=0; i<iterations; i++) {

    ComputeIteration(1.0); // like_a_tau = 1.0, because of time_step = 0.02smth

    PARAMETERS->OutTime();
    cout << "Iteration " << i << " done." << endl;
  }

  OUT_RESULT->ProcessParameters(sep::FIRST);

  OUT_RESULT->OutParameters(sep::FIRST);

  PARAMETERS->OutTime();
  cout << "Outputting results done." << endl;

  MeasureTime(iterations);

  OUT_RESULT->CheckMassConservation(sep::FIRST);
}


void Solver::ComputeIteration(double dt) {

  // here we should have some magic for 3D case, I  guess
  // but for 2D case things are simple
  // something like: dt/2 - x, dt - y, dt/2 - x

  MakeStep(sep::X, dt);

  MakeStep(sep::Y, dt);

  if (PARAMETERS->GetUseZAxis())
    MakeStep(sep::Z, dt);

  if (PARAMETERS->GetUseCollisionIntegral()) {

    if (!PARAMETERS->GetSecondGasIsActive()) {
      cout << "Error: no second gas, but integral." << endl;
      exit(-1);
    }

    INTEGRAL->Iteration();
  }
}


void Solver::MakeStep(sep::Axis axis, double dt) {

  // and exchange obtained data in edge zone between grids
  ExchangeEdgeZoneSpeed();

  // just let all grids compute their half speed
  // since they all have necessary data for doing it
  grid_->ComputeHalfSpeed(axis, dt);

  // then we should exchange edge zone between grids
  ExchangeEdgeZoneHalfSpeed();

  grid_->ComputeSpeed(axis, dt);
}


void Solver::ExchangeEdgeZoneSpeed() {

  if (!PARAMETERS->GetUseParallelComputing())
    return;

  PARALLEL->ExchangeEdgeZone(sep::Speed);
}

void Solver::ExchangeEdgeZoneHalfSpeed() {

  if (!PARAMETERS->GetUseParallelComputing())
    return;

  PARALLEL->ExchangeEdgeZone(sep::HalfSpeed);
}


void Solver::MeasureTime(int iterations) {

  static bool start = true;

  if (start) {

    start = false;

    start_computing_ = clock();
  } else {
    // end of computing

    double time = float(clock () - start_computing_) /  CLOCKS_PER_SEC;

    cout << "Taken time: " << (int)(time/60.0) << " m " <<
      ((int)time)%60 << " s." << endl;

    time /= iterations;

    cout << "Average time for one iteration: " << (int)(time/60.0) << " m " <<
      ((int)time)%60 << " s." << endl;
  }
}

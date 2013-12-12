#include "solver.h"
#include "out_result.h"
#include "parameters.h"

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

  int iterations = 1;

  for (int i=0; i<iterations; i++) {
    ComputeIteration(1.0); // like_a_tau = 1.0, because of time_step = 0.02smth
    cout << "Iteration " << i << " done." << endl;
  }

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

}


void Solver::MakeStep(sep::Axis axis, double dt) {

  // just let all grids compute their half speed
  // since they all have necessary data for doing it
  grid_->ComputeHalfSpeed(axis, dt);

  // then we should exchange edge zone between grids
  ExchangeEdgeZoneHalfSpeed();

  grid_->ComputeSpeed(axis, dt);

  // and exchange obtained data in edge zone between grids
  ExchangeEdgeZoneSpeed();

}


void Solver::ExchangeEdgeZoneHalfSpeed() {
  // TODO: to implement
  //cout << "ExchangeEdgeZoneHalfSpeed" << endl;
}


void Solver::ExchangeEdgeZoneSpeed() {
  // TODO: to implement
  //cout << "ExchangeEdgeZoneSpeed" << endl;
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

    time /= whole_quantity_of_cells_; // second per cell
    time *= 1000.0 / iterations;  // 1000 iterations per cell

    cout << "Time for 1 cell x1000 iteration: " << (int)(time/60.0) << " m " <<
      ((int)time)%60 << " s." << endl;
  }
}

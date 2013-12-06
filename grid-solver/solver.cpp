#include "solver.h"
#include "out_result.h"

string sep::int_to_string(int i) {

    stringstream ss;
    std::string s;
    ss << i;
    s = ss.str();

    return s;
};


void Solver::Compute() {

  OUT_RESULT->CheckMassConservation(sep::FIRST);

  for (int i=0; i<1; i++) {
    ComputeIteration(1.0); // like_a_tau = 1.0, because of time_step = 0.02smth
    cout << "Iteration " << i << " done." << endl;
  }

  OUT_RESULT->CheckMassConservation(sep::FIRST);
}


void Solver::ComputeIteration(double dt) {

  // here we should have some magic for 3D case, I  guess
  // but for 2D case things are simple
  // something like:

  MakeStep(sep::X, dt);

  MakeStep(sep::Y, dt);

  // for debug we comment it
  //MakeStep(sep::X, dt/2);

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

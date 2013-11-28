#include "solver.h"

string sep::int_to_string(int i) {

    stringstream ss;
    std::string s;
    ss << i;
    s = ss.str();

    return s;
};


void Solver::Compute() {

  for (int i=0; i<2; i++) {
    ComputeIteration(0.01);
    cout << "iteration " << i << " done" << endl;
  }

}


void Solver::ComputeIteration(double dt) {

  // here we should have some magic for 3D case, I  guess
  // but for 2D case things are simple
  // something like:

  MakeStep(sep::X, dt/2);

  MakeStep(sep::Y, dt);

  MakeStep(sep::X, dt/2);

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
  cout << "ExchangeEdgeZoneHalfSpeed" << endl;
}


void Solver::ExchangeEdgeZoneSpeed() {
  // TODO: to implement
  cout << "ExchangeEdgeZoneSpeed" << endl;
}

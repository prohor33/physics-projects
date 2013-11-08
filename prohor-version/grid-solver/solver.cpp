#include "solver.h"

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
  ComputeHalfSpeed(axis, dt);

  // then we should exchange edge zone between grids


  ComputeSpeed(axis, dt);

  // and exchange obtained data between grids

}


void Solver::ComputeHalfSpeed(double dt, sep::Axis axis) {

}

void Solver::ComputeSpeed(double dt, sep::Axis axis) {

}

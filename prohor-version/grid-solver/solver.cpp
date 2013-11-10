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
  ExchangeEdgeZoneHalfSpeed();

  ComputeSpeed(axis, dt);

  // and exchange obtained data in edge zone between grids
  ExchangeEdgeZoneSpeed();

}


void Solver::ExchangeEdgeZoneHalfSpeed() {
  // TODO: to implement
}


void Solver::ExchangeEdgeZoneSpeed() {
  // TODO: to implement
}


void Solver::ComputeHalfSpeed(sep::Axis axis, double dt) {

  vector<vector<vector<Grid*> > >::iterator cii_x;
  vector<vector<Grid*> >::iterator cii_xy;
  vector<Grid*>::iterator cii_xyz;

  for (cii_x=grids_.begin(); cii_x!=grids_.end(); ++cii_x) {
    for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
      for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {
        (*cii_xyz)->ComputeHalfSpeed(axis, dt);
      }
    }
  }

}

void Solver::ComputeSpeed(sep::Axis axis, double dt) {

  vector<vector<vector<Grid*> > >::iterator cii_x;
  vector<vector<Grid*> >::iterator cii_xy;
  vector<Grid*>::iterator cii_xyz;

  for (cii_x=grids_.begin(); cii_x!=grids_.end(); ++cii_x) {
    for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
      for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {
        (*cii_xyz)->ComputeSpeed(axis, dt);
      }
    }
  }

}

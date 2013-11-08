#ifndef _SOLVER_H_
#define _SOLVER_H_

#include "grid.h"

class Solver {

 public:

  static Solver* Instance() {
    static Solver Solver_;
    return &Solver_;
  }

  vector<vector<vector<Grid> > >& grids() {
    return grids_;
  }

  void ComputeIteration(double dt);

  void MakeStep(sep::Axis axis, double dt);

  void ComputeHalfSpeed(double dt, sep::Axis axis);

  void ComputeSpeed(double dt, sep::Axis axis);

 private:

  Solver() {};
  ~Solver() {};

  vector<vector<vector<Grid> > > grids_;
};

#define SOLVER Solver::Instance()

#endif  // _SOLVER_H_
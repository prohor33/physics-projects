#ifndef _SOLVER_H_
#define _SOLVER_H_

#include "main.h"
#include "grid.h"

class Initializer;

class Solver {

  friend class Initializer;

 public:

  static Solver* Instance() {
    static Solver Solver_;
    return &Solver_;
  }

  vector<vector<vector<Grid*> > >& grids() {
    return grids_;
  }

  void ComputeIteration(double dt);

  void MakeStep(sep::Axis axis, double dt);

  void ExchangeEdgeZoneHalfSpeed();

  void ExchangeEdgeZoneSpeed();

  void ComputeHalfSpeed(sep::Axis axis, double dt);

  void ComputeSpeed(sep::Axis axis, double dt);

 private:

  Solver() {};
  ~Solver() {};

 protected:

  vector<vector<vector<Grid*> > > grids_;

};

#define SOLVER Solver::Instance()

#endif  // _SOLVER_H_

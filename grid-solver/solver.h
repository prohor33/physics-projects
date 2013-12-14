#ifndef _SOLVER_H_
#define _SOLVER_H_

#include "main.h"
#include "grid.h"

class Manager;
class OutResult;

struct GridNeighbor {

  GridNeighbor(
      int next,
      int prev
      ) :
      next(next),
      prev(prev)
  {};

public:
  int next;
  int prev;
};

class Solver {

  friend class Manager;
  friend class OutResult;

 public:

  static Solver* Instance() {
    static Solver Solver_;
    return &Solver_;
  }

  Grid* grid() {
    return grid_;
  }

  void Compute();

  void ComputeIteration(double dt);

  void MakeStep(sep::Axis axis, double dt);

  void ExchangeEdgeZoneHalfSpeed();

  void ExchangeEdgeZoneSpeed();

  void ComputeHalfSpeed(sep::Axis axis, double dt);

  void ComputeSpeed(sep::Axis axis, double dt);

  void SetGridNeighbors(GridNeighbor x, GridNeighbor y, GridNeighbor z) {
    neighbors_.clear();
    neighbors_.push_back(x);
    neighbors_.push_back(y);
    neighbors_.push_back(z);
  }

  vector<GridNeighbor> GetGridNeighbors() { return neighbors_; };

  void SetWholeCellsQuantity(int x) {
    whole_quantity_of_cells_ = x;
  }

 void MeasureTime(int iterations=0);

 private:

  Solver() {};
  ~Solver() {};

 protected:

  Grid* grid_;

  vector<GridNeighbor> neighbors_; // for x, y, z

  clock_t start_computing_;

  int whole_quantity_of_cells_;

};

#define SOLVER Solver::Instance()

#endif  // _SOLVER_H_

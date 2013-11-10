#ifndef _GRID_H_
#define _GRID_H_

#include "cell.h"

class Grid;

struct GridNeighbor {

  GridNeighbor(
      Grid* next,
      Grid* prev,
      Grid* that
      ) :
      next(next),
      prev(prev),
      that(that)
  {};

public:
  Grid *next;
  Grid *prev;
  Grid *that;
};

class Grid {

 public:
  Grid() {};
  ~Grid() {};

  void ComputeHalfSpeed(sep::Axis axis, double dt);

  void ComputeSpeed(sep::Axis axis, double dt);

  vector<vector<vector<Cell*> > >& cells() {
    return cells_;
  }

 private:

  vector<vector<vector<Cell*> > > cells_;

  vector<GridNeighbor> neighbor_;  // for x, y, z axis

};

#endif  // _GRID_H_

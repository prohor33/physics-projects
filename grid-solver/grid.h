#ifndef _GRID_H_
#define _GRID_H_

#include "cell.h"


class Grid {

 public:
  Grid();
  ~Grid() {};

  void ComputeHalfSpeed(sep::Axis axis, double dt);

  void ComputeSpeed(sep::Axis axis, double dt);

  vector<vector<vector<Cell*> > >& cells() {
    return cells_;
  }

 private:

  vector<vector<vector<Cell*> > > cells_;
  vector<vector<vector<Cell*> > > second_cells_;  // second gas cells

};

#endif  // _GRID_H_

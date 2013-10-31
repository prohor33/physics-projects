#ifndef _GRID_H_
#define _GRID_H_

#include "cell.h"

class Grid {

 public:

  vector<vector<vector<Cell> > >& cells() {
    return cells_;
  }

 private:

  Grid() {};
  ~Grid() {};

  vector<vector<vector<Cell> > > cells_;
};

#endif  // _GRID_H_

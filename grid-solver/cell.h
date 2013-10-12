#ifndef _CELL_H_
#define _CELL_H_

#include "main.h"

class Cell;

struct Neighbor {
  Neighbor(Cell* next, Cell* prev, Cell* that) :
      next(next), prev(prev), that(that) {};
  Cell *next;
  Cell *prev;
  Cell *that;
};

class Cell {
 public:
  Cell() {};
  ~Cell() {};
  vector<vector<vector<double> > >& speed() {
    return speed_;
  }
 private:
  vector<vector<vector<double> > > speed_;
  vector<Neighbor> neighbor;  // for x, y, z axis
};

#endif // _CELL_H_

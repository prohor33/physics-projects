#ifndef _CELL_CONTAINER_H_
#define _CELL_CONTAINER_H_

#include "cell.h"

class CellContainer {

 public:

  static CellContainer* Instance() {
    static CellContainer CellContainer_;
    return &CellContainer_;
  }

  vector<vector<vector<Cell> > >& cells() {
    return cells_;
  }

 private:

  CellContainer() {};
  ~CellContainer() {};

  vector<vector<vector<Cell> > > cells_;
};

#define CELL_CONTAINER CellContainer::Instance()

#endif  // _CELL_CONTAINER_H_

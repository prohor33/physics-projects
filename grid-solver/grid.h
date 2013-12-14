#ifndef _GRID_H_
#define _GRID_H_

#include "cell.h"


class Grid {

 public:

  // for initialization grid without file (singleton mode)
  Grid();

  // for initialization grid from file
  Grid(std::vector<int> start, std::vector<int> size);

  ~Grid() {};

  void ComputeHalfSpeed(sep::Axis axis, double dt);
  void ComputeHalfSpeed(sep::GasNumb gas_numb, sep::Axis axis, double dt);

  void ComputeSpeed(sep::Axis axis, double dt);
  void ComputeSpeed(sep::GasNumb gas_numb, sep::Axis axis, double dt);

  vector<vector<vector<Cell*> > >& cells(sep::GasNumb gas_number) {
    return cells_[gas_number];
  }

 private:

  void InitGasCells(
      sep::GasNumb gas_number,
      std::vector<int> start,
      std::vector<int> size
      );

  void InitGasCells(sep::GasNumb gas_number);

  vector<vector<vector<Cell*> > > cells_[2];   // two gases
};

#endif  // _GRID_H_

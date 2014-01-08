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

  Cell***** cells() {
    return cells_;
  }

  Cell**** cells(sep::GasNumb gas_number) {
    return cells_[gas_number];
  }

  vector<int>& size() {
    return size_;
  }

 private:

  void InitGasCells(
      sep::GasNumb gas_number,
      std::vector<int> start,
      std::vector<int> size
      );

  void InitGasCells(sep::GasNumb gas_number);

  Cell**** cells_[2];  // two gases

  vector<int> size_;
  vector<int> start_;
};

#endif  // _GRID_H_

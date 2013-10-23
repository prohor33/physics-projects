#ifndef _CELL_H_
#define _CELL_H_

#include "main.h"

class Cell;

struct Neighbor {

  Neighbor(
      Cell* next,
      Cell* prev,
      Cell* that
      ) :
      next(next),
      prev(prev),
      that(that)
  {};

  Cell *next;
  Cell *prev;
  Cell *that;
};

class Cell {
 public:
  enum GasNumb {
    FIRST = 0,
    SECOND = 1
  };

  Cell(GasNumb gas_numb) :
    gas_numb_(gas_numb)
  {};

  ~Cell() {};

  void ComputeHalfSpeed(double dt,
      sep::Axis axis);

  inline double& speed(const int* coord) {
    return speed_[coord[sep::X]][coord[sep::Y]][coord[sep::Z]];
  }
  inline double& speed_half(const int* coord) {
    return speed_half_[coord[sep::X]][coord[sep::Y]][coord[sep::Z]];
  }

  inline double MolMass();

  inline double H();

  inline double P(sep::Axis axis,
      const int* coord);

  inline double Limiter(sep::Axis axis,
      const int* coord);

 private:

  vector<vector<vector<double> > > speed_;
  vector<vector<vector<double> > > speed_half_;

  vector<Neighbor> neighbor_;  // for x, y, z axis
  GasNumb gas_numb_;
};

#endif // _CELL_H_

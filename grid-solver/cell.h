#ifndef _CELL_H_
#define _CELL_H_

#include "main.h"

class Cell;

struct CellNeighbor {

  CellNeighbor(
      Cell* next,
      Cell* prev
      ) :
      next(next),
      prev(prev)
  {};

public:
  Cell *next;
  Cell *prev;
};

class OutResult;

class Cell {

  friend class OutResult;

 public:

  enum GasNumb {
    FIRST = 0,
    SECOND = 1
  };

  enum CellType {
    NORMAL = 0,
    FAKE = 1,
    OBTAINED = 2
  };

  Cell(GasNumb gas_numb, CellType type=NORMAL);

  ~Cell() {};

  void ComputeHalfSpeed(sep::Axis axis,
      double dt);

  void ComputeSpeed(sep::Axis axis, double dt);

  void ComputeHalfSpeedPrevIsBorder(sep::Axis axis,
      double dt);

  void ComputeHalfSpeedNextIsBorder(sep::Axis axis,
      double dt);

  inline double& speed(vector<int> coord) {
    return speed_[GetIndex(coord)];
  }
  inline double& speed_half(vector<int> coord) {
    return speed_half_[GetIndex(coord)];
  }

  inline double MolMass() const;

  inline double H() const;

  inline double P(sep::Axis axis,
      vector<int> coord) const;

  inline double Limiter(sep::Axis axis,
      vector<int> coord);

  inline vector<int> GetSpeedCoord(int index);

  int GetIndex(vector<int> coord);

  CellType type() { return type_; };

  double& wall_t() { return wall_t_; };

  void SetNeighbors(CellNeighbor x, CellNeighbor y, CellNeighbor z) {
    neighbor_.clear();
    neighbor_.push_back(x);
    neighbor_.push_back(y);
    neighbor_.push_back(z);
  }

  // for debug
  int coord_x, coord_y, coord_z;

 private:

  vector<double> speed_;
  vector<double> speed_half_;

  vector<CellNeighbor> neighbor_;  // for x, y, z axis
  GasNumb gas_numb_;
  CellType type_;

  double wall_t_;
};

#endif // _CELL_H_

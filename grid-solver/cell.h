#ifndef _CELL_H_
#define _CELL_H_

#include "main.h"

class Cell;

struct CellNeighbor {

  CellNeighbor(
      Cell* next,
      Cell* prev
      );

public:
  Cell *next;
  Cell *prev;
};

class OutResult;

class Cell {

  friend class OutResult;

 public:

  enum CellType {
    NONE = 0,
    NORMAL = 1,
    FAKE = 2,
    OBTAINED = 3
  };

  Cell(sep::GasNumb gas_numb, CellType type=NORMAL);

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

  double MolMass() const;

  inline double H() const;

  double P(sep::Axis axis,
      vector<int> coord) const;

  inline double P2(vector<int> coord) const;

  inline double Limiter(sep::Axis axis,
      vector<int> coord);

  vector<int> GetSpeedCoord(int index);

  int GetIndex(vector<int> coord);

  CellType type() { return type_; };

  vector<double>& wall_t() { return wall_t_; };

  void SetNeighbors(CellNeighbor x, CellNeighbor y, CellNeighbor z) {
    neighbor_.clear();
    neighbor_.push_back(x);
    neighbor_.push_back(y);
    neighbor_.push_back(z);
  }

  void SetSpaceCoord(int x, int y, int z) {
    space_coord_ = vector<int>(3);
    space_coord_[sep::X] = x;
    space_coord_[sep::Y] = y;
    space_coord_[sep::Z] = z;
  }

  vector<int> space_coord() { return space_coord_; };

 private:

  vector<double> speed_;
  vector<double> speed_half_;

  vector<CellNeighbor> neighbor_;  // for x, y, z axis
  sep::GasNumb gas_numb_;
  CellType type_;

  vector<int> space_coord_;

  vector<double> wall_t_; // for different axes
};

#endif // _CELL_H_

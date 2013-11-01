#include "grid.h"

void Grid::ComputeHalfSpeed(double dt, sep::Axis axis) {

  vector<vector<vector<Cell*> > >::iterator cii_z;
  vector<vector<Cell*> >::iterator cii_y;
  vector<Cell*>::iterator cii_x;

  for (cii_x=cells_.begin(); cii_x!=cells_.end(); ++cii_x) {
    for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
      for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {
        (*cii_xyz)->ComputeHalfSpeed(dt, axis);
      }
    }
  }
}

void Grid::ComputeSpeed() {

}

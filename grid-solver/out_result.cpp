#include "out_result.h"
#include "solver.h"
#include "grid.h"
#include "cell.h"


void OutResult::OutParameters() {

  // TODO: here we should evoke functions for temperature, concentration, etc
  // but for test we can just print single speed in the each cell

  ofstream out_file;
  out_file.open ("test_result");

  int z = 0;

  vector<vector<vector<Cell*> > >::iterator cii_x;
  vector<vector<Cell*> >::iterator cii_xy;
  vector<Cell*>::iterator cii_xyz;

  vector<double>::iterator cii;

  vector<int> coord(3);

  for (cii_x=SOLVER->grid_->cells().begin(); cii_x!=SOLVER->grid_->cells().end(); ++cii_x) {
    for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
      for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {

        if ((*cii_xyz)->type() == Cell::NORMAL) {

          for (cii=(*cii_xyz)->speed_.begin();
            cii!=(*cii_xyz)->speed_.end(); ++cii) {

            coord = (*cii_xyz)->GetSpeedCoord((int)(cii-(*cii_xyz)->speed_.begin()));

            out_file <<
                coord[sep::X] << " " <<
                coord[sep::Y] << " " <<
                coord[sep::Z] << " " <<
                (*cii_xyz)->speed(coord) << endl;
          }
        }
      }
    }
  }

  out_file.close();

}

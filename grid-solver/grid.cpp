#include "grid.h"
#include "cell.h"


Grid::Grid() {

  // for test let's create rectangle grid n x m
  // with only one gas
  int n, m;
  n = 20;
  m = 30;
  Cell* cell;

  // initialize cells for first gas
  for (int i=0; i<n;i++) {

    vector<vector<Cell*> > cells_yz;

    for (int j=0; j<m; j++) {

      if (i == 0 || i == n-1 || j == 0 || j == m-1) {
        // fake cells

        cell = new Cell(Cell::FIRST, Cell::FAKE);
      }
      else {
        // normal cells

        cell = new Cell(Cell::FIRST, Cell::NORMAL);

        if (i == 1 || i == n-2 || j == 1 || j == m-2) {
          // cells with temperature

          if (i == 0 || i == n-1)
            cell->wall_t() = 1.0;
          if (j == 0 || j == m-1)
            cell->wall_t() = 0.6;
        }
      }

      cell->coord_x = i;
      cell->coord_y = j;
      cell->coord_z = 0;

      vector<Cell*> cells_z;
      cells_z.push_back(cell);

      cells_yz.push_back(cells_z);

    }

    cells_.push_back(cells_yz);
  }

  // initialize neighbors for cells
  for (int i=0; i<n;i++) {
    for (int j=0; j<m; j++) {

      if (i == 0 || i == n-1 || j == 0 || j == m-1) {
        // fake cells

        cells_[i][j][0]->SetNeighbors(
            CellNeighbor(NULL, NULL),
            CellNeighbor(NULL, NULL),
            CellNeighbor(NULL, NULL)
            );
      }
      else {

        cells_[i][j][0]->SetNeighbors(
            CellNeighbor(cells_[i+1][j][0], cells_[i-1][j][0]),
            CellNeighbor(cells_[i][j+1][0], cells_[i][j-1][0]),
            CellNeighbor(NULL, NULL)
            );
      }
    }
  }

}


void Grid::ComputeHalfSpeed(sep::Axis axis, double dt) {

  vector<vector<vector<Cell*> > >::iterator cii_x;
  vector<vector<Cell*> >::iterator cii_xy;
  vector<Cell*>::iterator cii_xyz;

  // first gas
  for (cii_x=cells_.begin(); cii_x!=cells_.end(); ++cii_x) {
    for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
      for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {
        (*cii_xyz)->ComputeHalfSpeed(axis, dt);
      }
    }
  }

  cout << "ComputeHalfSpeed for first gas done" << endl;

  // second gas
  for (cii_x=second_cells_.begin(); cii_x!=second_cells_.end(); ++cii_x) {
    for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
      for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {
        (*cii_xyz)->ComputeHalfSpeed(axis, dt);
      }
    }
  }

  cout << "ComputeHalfSpeed for second gas done" << endl;
}

void Grid::ComputeSpeed(sep::Axis axis, double dt) {

  vector<vector<vector<Cell*> > >::iterator cii_x;
  vector<vector<Cell*> >::iterator cii_xy;
  vector<Cell*>::iterator cii_xyz;

  for (cii_x=cells_.begin(); cii_x!=cells_.end(); ++cii_x) {
    for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
      for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {
        (*cii_xyz)->ComputeSpeed(axis, dt);
      }
    }
  }

  // second gas
  for (cii_x=second_cells_.begin(); cii_x!=second_cells_.end(); ++cii_x) {
    for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
      for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {
        (*cii_xyz)->ComputeSpeed(axis, dt);
      }
    }
  }
}

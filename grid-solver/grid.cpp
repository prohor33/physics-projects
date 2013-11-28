#include "grid.h"
#include "cell.h"


Grid::Grid() {

  // initialize cells for first gas
  InitGasCells(sep::FIRST);

  // initialize cells for second gas
  InitGasCells(sep::SECOND);
}


void Grid::InitGasCells(sep::GasNumb gas_numb) {

  vector<vector<vector<Cell*> > >& cells = cells_[gas_numb];

  // for test let's create rectangle grid n x m
  // with only one gas
  int n, m;
  n = 5;
  m = 5;
  Cell* cell;

  // initialize cells for our gas
  for (int i=0; i<n;i++) {

    vector<vector<Cell*> > cells_yz;

    for (int j=0; j<m; j++) {

      if (i == 0 || i == n-1 || j == 0 || j == m-1) {
        // fake cells

        cell = new Cell(sep::FIRST, Cell::FAKE);
      }
      else {
        // normal cells

        cell = new Cell(sep::FIRST, Cell::NORMAL);

        if (i == 1 || i == n-2 || j == 1 || j == m-2) {
          // cells with temperature

          if (i == 1 || i == n-2)
            cell->wall_t() = 1.0;
          if (j == 1 || j == m-2)
            cell->wall_t() = 0.6;
        }
      }

      cell->SetSpaceCoord(i, j, 0);

//      cout << "space_coord: " << cell->space_coord()[0] << " " <<
//          cell->space_coord()[1] << " " << cell->space_coord()[2] << " " << endl;

      vector<Cell*> cells_z;
      cells_z.push_back(cell);

      cells_yz.push_back(cells_z);

    }

    cells.push_back(cells_yz);
  }

  // initialize neighbors for cells
  for (int i=0; i<n;i++) {
    for (int j=0; j<m; j++) {

      if (i == 0 || i == n-1 || j == 0 || j == m-1) {
        // fake cells

        cells[i][j][0]->SetNeighbors(
            CellNeighbor(NULL, NULL),
            CellNeighbor(NULL, NULL),
            CellNeighbor(NULL, NULL)
            );
      }
      else {

        cells[i][j][0]->SetNeighbors(
            CellNeighbor(cells[i+1][j][0], cells[i-1][j][0]),
            CellNeighbor(cells[i][j+1][0], cells[i][j-1][0]),
            CellNeighbor(NULL, NULL)
            );
      }
    }
  }
  cout << "============ size cells of " << gas_numb << "= " << cells_[gas_numb].size() << endl;
}


void Grid::ComputeHalfSpeed(sep::Axis axis, double dt) {

  ComputeHalfSpeed(sep::FIRST, axis, dt);
  cout << "ComputeHalfSpeed for first gas done" << endl;

  ComputeHalfSpeed(sep::SECOND, axis, dt);
  cout << "ComputeHalfSpeed for second gas done" << endl;
}


void Grid::ComputeHalfSpeed(sep::GasNumb gas_numb, sep::Axis axis, double dt) {

  vector<vector<vector<Cell*> > >::iterator cii_x;
  vector<vector<Cell*> >::iterator cii_xy;
  vector<Cell*>::iterator cii_xyz;

  // first gas
  for (cii_x=cells_[gas_numb].begin(); cii_x!=cells_[gas_numb].end(); ++cii_x) {
    for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
      for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {
        (*cii_xyz)->ComputeHalfSpeed(axis, dt);
      }
    }
  }
}


void Grid::ComputeSpeed(sep::Axis axis, double dt) {
  ComputeSpeed(sep::FIRST, axis, dt);
  ComputeSpeed(sep::SECOND, axis, dt);
}


void Grid::ComputeSpeed(sep::GasNumb gas_numb, sep::Axis axis, double dt) {

  vector<vector<vector<Cell*> > >::iterator cii_x;
  vector<vector<Cell*> >::iterator cii_xy;
  vector<Cell*>::iterator cii_xyz;

  for (cii_x=cells_[gas_numb].begin(); cii_x!=cells_[gas_numb].end(); ++cii_x) {
    for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
      for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {
        (*cii_xyz)->ComputeSpeed(axis, dt);
      }
    }
  }
}

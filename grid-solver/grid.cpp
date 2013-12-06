#include "grid.h"
#include "cell.h"
#include "parameters.h"


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
  int n, m, p;
  n = 6;
  m = 8;
  p = 5;
  Cell* cell;

  p = PARAMETERS->GetUseZAxis() ? p : 1;

  // initialize cells for our gas
  for (int i=0; i<n;i++) {

    vector<vector<Cell*> > cells_yz;

    for (int j=0; j<m; j++) {

      vector<Cell*> cells_z;

      for (int k=0; k<p; k++) {

        if (i == 0 || i == n-1 ||
            j == 0 || j == m-1 ||
            ((k == 0 || k == p-1) && PARAMETERS->GetUseZAxis())) {
          // fake cells

          cell = new Cell(gas_numb, Cell::FAKE);
        }
        else {
          // normal cells

          cell = new Cell(gas_numb, Cell::NORMAL);

          if (i == 1 || i == n-2 ||
              j == 1 || j == m-2 ||
              k == 1 || k == p-2) {
            // cells with temperature

            // TODO: here we have temperature intersections on different axis
            if (i == 1 || i == n-2)
              cell->wall_t() = 0.8;
            if (j == 1 || j == m-2)
              cell->wall_t() = 1.2;
            if (k == 1 || k == p-2)
              cell->wall_t() = 0.7;
          }
        }

        cell->SetSpaceCoord(i, j, k);

        cells_z.push_back(cell);
      }

      cells_yz.push_back(cells_z);

    }

    cells.push_back(cells_yz);
  }

  // initialize neighbors for cells
  for (int i=0; i<n;i++) {
    for (int j=0; j<m; j++) {
      for (int k=0; k<p; k++) {

        if (i == 0 || i == n-1 ||
            j == 0 || j == m-1 ||
            ((k == 0 || k == p-1) && PARAMETERS->GetUseZAxis())) {
            // fake cells

          cells[i][j][k]->SetNeighbors(
              CellNeighbor(NULL, NULL),
              CellNeighbor(NULL, NULL),
              CellNeighbor(NULL, NULL)
              );
        }
        else {

          cells[i][j][k]->SetNeighbors(
              CellNeighbor(cells[i+1][j][k], cells[i-1][j][k]),
              CellNeighbor(cells[i][j+1][k], cells[i][j-1][k]),
              CellNeighbor(PARAMETERS->GetUseZAxis() ? cells[i][j][k+1] : NULL,
                  PARAMETERS->GetUseZAxis() ? cells[i][j][k-1] : NULL)
              );
        }
      }
    }
  }
  //cout << "============ size cells of " << gas_numb << "= " << cells_[gas_numb].size() << endl;
}


void Grid::ComputeHalfSpeed(sep::Axis axis, double dt) {

  ComputeHalfSpeed(sep::FIRST, axis, dt);

  if (PARAMETERS->GetSecondGasIsActive())
    ComputeHalfSpeed(sep::SECOND, axis, dt);
}


void Grid::ComputeHalfSpeed(sep::GasNumb gas_numb, sep::Axis axis, double dt) {

  vector<vector<vector<Cell*> > >::iterator cii_x;
  vector<vector<Cell*> >::iterator cii_xy;
  vector<Cell*>::iterator cii_xyz;

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

  if (PARAMETERS->GetSecondGasIsActive())
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

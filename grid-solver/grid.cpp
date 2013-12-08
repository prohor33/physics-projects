#include "grid.h"
#include "cell.h"
#include "parameters.h"
#include "grid_file_reader.h"


Grid::Grid() {

  // initialize cells for first gas
  InitGasCells(sep::FIRST);

  // initialize cells for second gas
  InitGasCells(sep::SECOND);
}


Grid::Grid(
    std::string file_name,
    std::vector<int> grid_start,
    std::vector<int> size
    ) {

  // initialize cells for first gas
  InitGasCells(sep::FIRST, file_name, grid_start, size);

  if (PARAMETERS->GetSecondGasIsActive()) {
    // initialize cells for second gas
    InitGasCells(sep::SECOND, file_name, grid_start, size);
  }
}


void Grid::InitGasCells(
    sep::GasNumb gas_numb,
    std::string file_name,
    std::vector<int> grid_start,
    std::vector<int> size
    ) {

  // TODO: we should initialize only grid for that process

  GRID_FILE_READER->ReadFile(file_name);

  vector<vector<vector<Cell*> > >& cells = cells_[gas_numb];

  int n, m, p;

  n = GRID_FILE_READER->grid_config()->size[sep::X];
  m = GRID_FILE_READER->grid_config()->size[sep::Y];
  p = GRID_FILE_READER->grid_config()->size[sep::Z];
  Cell* cell;

  p = PARAMETERS->GetUseZAxis() ? p : 1;

  // initialize cells for our gas
  for (int i=0; i<n;i++) {

    vector<vector<Cell*> > cells_yz;

    for (int j=0; j<m; j++) {

      vector<Cell*> cells_z;

      for (int k=0; k<p; k++) {

        switch (GRID_FILE_READER->cells()[i][j][k]->type) {
        case CellInitData::CIDT_NONE:
          cells_z.push_back(NULL);
          cout << "Warning: Empty cell is created" << endl;
          break;
        case CellInitData::CIDT_NORMAL:
        case CellInitData::CIDT_FAKE:

          if (GRID_FILE_READER->cells()[i][j][k]->type == CellInitData::CIDT_NORMAL)
            cell = new Cell(gas_numb, Cell::NORMAL);
          else
            cell = new Cell(gas_numb, Cell::FAKE);

          // TODO: here we have temperature intersections on different axis
          if (i == 1 || i == n-2)
            cell->wall_t() = GRID_FILE_READER->cells()[i][j][k]->T_start[sep::X];
          if (j == 1 || j == m-2)
            cell->wall_t() = GRID_FILE_READER->cells()[i][j][k]->T_start[sep::Y];
          if (k == 1 || k == p-2)
            cell->wall_t() = GRID_FILE_READER->cells()[i][j][k]->T_start[sep::Z];


          cell->SetSpaceCoord(i, j, k);

          cells_z.push_back(cell);
          break;
        default:
          cout << "Error: get wrong cell type during reading input file" << endl;
          return;
          break;
        }
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

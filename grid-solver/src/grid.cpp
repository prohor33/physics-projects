#include "grid.h"
#include "cell.h"
#include "parameters.h"
#include "solver.h"
#include "grid_file_reader.h"


Grid::Grid() {

  // this will be not from file grid

  // initialize cells for first gas
  InitGasCells(sep::FIRST);

  if (PARAMETERS->GetSecondGasIsActive()) {
    // initialize cells for second gas
    InitGasCells(sep::SECOND);
  }
}


Grid::Grid(
    std::vector<int> start,
    std::vector<int> size
    ) {

  // initialize cells for first gas
  InitGasCells(sep::FIRST, start, size);

  if (PARAMETERS->GetSecondGasIsActive()) {
    // initialize cells for second gas
    InitGasCells(sep::SECOND, start, size);
  }

  // release input file data
  GRID_FILE_READER->cells().clear();
}


void Grid::InitGasCells(
    sep::GasNumb gas_numb,
    std::vector<int> start,
    std::vector<int> size
    ) {

  Cell****& cells = cells_[gas_numb];

  Cell* cell;

  for (sep::Axis ax=sep::X; ax<=sep::Z; ax = (sep::Axis)((int)ax + 1)) {

    if (SOLVER->GetGridNeighbors()[ax].prev != -1) {
      start[ax] -= 2;
      size[ax] += 2;
    }

    if (SOLVER->GetGridNeighbors()[ax].next != -1)
      size[ax] += 2;
  }

  int n, m, p;

  n = size[sep::X];
  m = size[sep::Y];
  p = size[sep::Z];

  size_ = vector<int>(3);
  size_ = size;
  start_ = vector<int>(3);
  start_ = start;

  double T1, T2, T, p_flow, p_flow_max;
  double n1, n2, n3, n4, P; // P - concentration here
  sep::Axis flow_axis = sep::X;
  int D = GRID_FILE_READER->grid_config()->D;
  T1 = PARAMETERS->GetT1();
  T2 = PARAMETERS->GetT2();
  p_flow_max = PARAMETERS->GetStartFlow();
  double mass = PARAMETERS->GetMolMass(gas_numb);
  n1 = PARAMETERS->GetConcentration(0);
  n2 = PARAMETERS->GetConcentration(1);
  n3 = PARAMETERS->GetConcentration(2);
  n4 = PARAMETERS->GetConcentration(3);
  cells = new Cell***[size[sep::X]];

  // initialize cells for our gas
  for (int i=0; i<n;i++) {

    cells[i] = new Cell**[size[sep::Y]];

    for (int j=0; j<m; j++) {

      cells[i][j] = new Cell*[size[sep::Z]];

      for (int k=0; k<p; k++) {

        p_flow = 0.0;
        T = 1.0;
        double n_up, n_down;
        if (PARAMETERS->GetUseStartTemperature()) {
          // TODO: This is only for x axis splitting with mpi
//          n_up = n2 + (n1 - n2) * i / 
          if (j > D - 2 && j < m - D + 1) {
            T = T2 + (T1 - T2) * (double)(j - D + 2) / (m - 2*D + 3);
          }
          else {
            T = j < D ? T2 : T1;
            if (j < D) {
 //             n = 
            }
          }
        }
        if (PARAMETERS->GetUseFlowKeeper() &&
            (j <= D -2 || j >= m - D + 1)) {
          int y = 0;
          int z = k - 1;
          int l = p - 3;
          if (j >= m - D + 1) {
            y = j - (m - D + 1);
          }
          else if (j <= D - 2) {
            y = j - 1;
          }
          double coef = pow(l, 6) / 36.0;
          //p_flow = 6.0 *  mass * y * (l - y) * z * (l - z) / coef;
          p_flow = 0.0;
        }
        CellInitData* gfr_cell = GRID_FILE_READER->
           cells()[start[sep::X]+i][start[sep::Y]+j][start[sep::Z]+k];

        switch  (gfr_cell->type) {
        case CellInitData::CIDT_NONE:

          cell = new Cell(gas_numb, sep::NONE);

          cell->SetSpaceCoord(i, j, k);

          cells[i][j][k] = cell;
          //cout << "Warning: Empty cell is created" << endl;
          break;

        case CellInitData::CIDT_NORMAL:
        case CellInitData::CIDT_FAKE:
          // debug
          // creating only fake cells!
          cout << "Normal cell is created" << endl;
          if (gfr_cell->type ==
              CellInitData::CIDT_NORMAL) {
            cout << "Normal cell is created" << endl;
            cell = new Cell(gas_numb, sep::NORMAL, T, flow_axis, p_flow, n);
          }
          else
            cell = new Cell(gas_numb, sep::FAKE, T, flow_axis, p_flow, n);

          cell->wall_t()[sep::X] =
            gfr_cell->T_start[sep::X];
          cell->wall_t()[sep::Y] =
            gfr_cell->T_start[sep::Y];
          cell->wall_t()[sep::Z] =
            gfr_cell->T_start[sep::Z];

          cell->SetSpaceCoord(i, j, k);

          cells[i][j][k] = cell;
          break;
        default:
          cout << "Error: get wrong cell type during reading input file" <<
            endl;
          return;
          break;
        }

        // Detect obtained cells
        if (i < 2 && SOLVER->GetGridNeighbors()[sep::X].prev != -1)
          cell->obtained() = true;
        if (i >= n-2 && SOLVER->GetGridNeighbors()[sep::X].next != -1)
          cell->obtained() = true;
        if (j < 2 && SOLVER->GetGridNeighbors()[sep::Y].prev != -1)
          cell->obtained() = true;
        if (j >= m-2 && SOLVER->GetGridNeighbors()[sep::Y].next != -1)
          cell->obtained() = true;
        if (k < 2 && SOLVER->GetGridNeighbors()[sep::Z].prev != -1)
          cell->obtained() = true;
        if (k >= p-2 && SOLVER->GetGridNeighbors()[sep::Z].next != -1)
          cell->obtained() = true;
      }
    }
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

  // this will be not from file grid

  Cell****& cells = cells_[gas_numb];

  // for test let's create rectangle grid n x m
  // with only one gas
  int n, m, p;
  n = 6;
  m = 8;
  p = 5;

  size_ = vector<int>(3);
  start_ = vector<int>(3);
  size_[sep::X] = n;
  size_[sep::Y] = m;
  size_[sep::Z] = p;
  start_[sep::X] = 0;
  start_[sep::Y] = 0;
  start_[sep::Z] = 0;

  Cell* cell;

  p = PARAMETERS->GetUseZAxis() ? p : 1;

  cells = new Cell***[n];

  // initialize cells for our gas
  for (int i=0; i<n;i++) {

    cells[i] = new Cell**[m];

    for (int j=0; j<m; j++) {

      cells[i][j] = new Cell*[p];

      for (int k=0; k<p; k++) {

        if (i == 0 || i == n-1 ||
            j == 0 || j == m-1 ||
            ((k == 0 || k == p-1) && PARAMETERS->GetUseZAxis())) {
          // fake cells

          cell = new Cell(gas_numb, sep::FAKE);
        }
        else {
          // normal cells

          cell = new Cell(gas_numb, sep::NORMAL);

          if (i == 1 || i == n-2 ||
              j == 1 || j == m-2 ||
              k == 1 || k == p-2) {
            // cells with temperature

            if (i == 1 || i == n-2)
              cell->wall_t()[sep::X] = 0.8;
            if (j == 1 || j == m-2)
              cell->wall_t()[sep::Y] = 1.2;
            if (k == 1 || k == p-2)
              cell->wall_t()[sep::Z] = 0.7;
          }
        }

        cell->SetSpaceCoord(i, j, k);

        cells[i][j][k] = cell;
      }
    }
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

  for(int i=0; i<size_[sep::X]; i++) {
    for(int j=0; j<size_[sep::Y]; j++) {
      for(int k=0; k<size_[sep::Z]; k++) {
        cells_[gas_numb][i][j][k]->ComputeHalfSpeed(axis, dt);
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

  for(int i=0; i<size_[sep::X]; i++) {
    for(int j=0; j<size_[sep::Y]; j++) {
      for(int k=0; k<size_[sep::Z]; k++) {
        cells_[gas_numb][i][j][k]->ComputeSpeed(axis, dt);
      }
    }
  }
}

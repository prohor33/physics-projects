#include "flow_keeper.h"
#include "solver.h"
#include "grid.h"
#include "cell.h"
#include "grid_file_reader.h"
#include "parameters.h"

void FlowKeeper::InitFlowAreas() {

  int D = GRID_FILE_READER->grid_config()->D;
  int size_yz = D - 2;
  int indent_x = GRID_FILE_READER->grid_config()->indent;
  const vector<int>& grid_size = SOLVER->grid()->size();
  const int* whole_grid_size =
    GRID_FILE_READER->grid_config()->size;
  const vector<int>& grid_start = SOLVER->grid()->start();
  vector<int> coord(3), size(3), indent(3);
  int sp_coord_x, sp_coord_y, sp_coord_z;
  sp_coord_x = whole_grid_size[sep::X] - 2;
  sp_coord_y = whole_grid_size[sep::Y] - size_yz - 1;

  // left up

  indent[sep::X] = indent_x;
  indent[sep::Y] = 0;
  indent[sep::Z] = 0;

  size[sep::X] = 1;
  size[sep::Y] = size_yz;
  size[sep::Z] = size_yz;

  coord[sep::X] = 1;
  coord[sep::Y] = sp_coord_y;
  coord[sep::Z] = 1;

  // TODO: this works only if mpi split in X axis
  if (coord[sep::X] > grid_start[sep::X] &&
      coord[sep::X] + size[sep::X] < grid_start[sep::X] + grid_size[sep::X])
    areas_.push_back(new FlowAreaData(coord, size, indent));

  // right up

  coord[sep::X] = sp_coord_x;
  coord[sep::Y] = sp_coord_y;
  coord[sep::Z] = 1;

  indent[sep::X] = -indent_x;

  if (coord[sep::X] > grid_start[sep::X] &&
      coord[sep::X] + size[sep::X] < grid_start[sep::X] + grid_size[sep::X])
    areas_.push_back(new FlowAreaData(coord, size, indent));

  // left down

  coord[sep::X] = 1;
  coord[sep::Y] = 1;
  coord[sep::Z] = 1;

  indent[sep::X] = indent_x;

  if (coord[sep::X] > grid_start[sep::X] &&
      coord[sep::X] + size[sep::X] < grid_start[sep::X] + grid_size[sep::X])
    areas_.push_back(new FlowAreaData(coord, size, indent));

  // right down

  coord[sep::X] = sp_coord_x;
  coord[sep::Y] = 1;
  coord[sep::Z] = 1;

  indent[sep::X] = -indent_x;

  if (coord[sep::X] > grid_start[sep::X] &&
      coord[sep::X] + size[sep::X] < grid_start[sep::X] + grid_size[sep::X])
    areas_.push_back(new FlowAreaData(coord, size, indent));
}


// does all the stuff
void FlowKeeper::SetFlow() {

  for (vector<FlowAreaData*>::iterator it = areas_.begin();
      it != areas_.end(); ++it) {

    SetAreaConsumption(**it);
    SetAreaFlow(**it);
  }
}


void FlowKeeper::SetAreaFlow(FlowAreaData& area) {

  area.consumption = 0.0f;
  Grid* grid = SOLVER->grid();
  int D = GRID_FILE_READER->grid_config()->D;
  int l = area.size[sep::Y] - 1;

  double p_coef = 1.0f;
  int i_loc, j_loc, k_loc;
  for(int i=area.coord[sep::X]; i<area.coord[sep::X]+area.size[sep::X]; i++) {
    for(int j=area.coord[sep::Y]; j<area.coord[sep::Y]+area.size[sep::Y]; j++) {
      for(int k=area.coord[sep::Z]; k<area.coord[sep::Z]+area.size[sep::Z];
          k++) {

        int y = j - area.coord[sep::Y];
        int z = k - area.coord[sep::Z];
        double coef = pow(l, 6) / 36.0;
        p_coef = y * (l - y) * z * (l - z) / coef;

        i_loc = i - grid->start()[sep::X];
        j_loc = j - grid->start()[sep::Y];
        k_loc = k - grid->start()[sep::Z];

        Cell* cell = grid->cells()[sep::FIRST][i_loc][j_loc][k_loc];
        if (cell)
          SetCellFlow(area, cell, p_coef);
      }
    }
  }
}

void FlowKeeper::SetCellFlow(FlowAreaData& area,
    Cell* cell, double p_coef) {

  if (cell->type() != sep::NORMAL) {
    cout << "error: flow cell is null or obtaied" << endl;
    return;
  }

  vector<double>::iterator cii;
  vector<int> coord(4);

  double u_x = 0.0;
  double p, p2;
  double m = cell->MolMass();
  double C = 0.0;
  // TODO: Hack
  //area.indent_consumption = 6.0;
  double p_flow = 1.0 * p_coef * m * area.indent_consumption;
  double T1 = PARAMETERS->GetT1();
  double T2 = PARAMETERS->GetT2();
  double n = 0.0f;
  double T_flow;

  if (area.coord[sep::Y] < SOLVER->grid()->size()[sep::Y]/2)
    T_flow = T2;
  else
    T_flow = T1;
  // process C
  for (cii=cell->speed().begin();
    cii!=cell->speed().end(); ++cii) {

    coord =
      cell->GetSpeedCoord((int)(cii-cell->speed().begin()));

    p2 = cell->P2_with_flow(coord, sep::X, p_flow);

    C += exp((-1.0f)*
        p2 / (2.0f * m * T_flow));
  }

  // n0 = 1.0f
  C = 1.0f / C;

  for (cii=cell->speed().begin();
    cii!=cell->speed().end(); ++cii) {

    coord =
      cell->GetSpeedCoord((int)(cii-cell->speed().begin()));

    p = cell->P(sep::X, coord);
    p2 = cell->P2_with_flow(coord, sep::X, p_flow);

    (*cii) = C * exp((-1.0f)*
        p2 / (2.0f * m * T_flow));

    u_x += (*cii) * p / m;

    n += (*cii);
  }
  u_x /= n;
  area.consumption += u_x;
}

void FlowKeeper::SetAreaConsumption(FlowAreaData& area) {

  area.indent_consumption = 0.0f;
  Grid* grid = SOLVER->grid();

  double coef = 1.0f;
  int i_loc, j_loc, k_loc;
  for(int i=area.coord[sep::X]; i<area.coord[sep::X]+area.size[sep::X]; i++) {
    for(int j=area.coord[sep::Y]; j<area.coord[sep::Y]+area.size[sep::Y]; j++) {
      for(int k=area.coord[sep::Z]; k<area.coord[sep::Z]+area.size[sep::Z];
          k++) {

        i_loc = i - grid->start()[sep::X] + area.indent[sep::X];
        j_loc = j - grid->start()[sep::Y] + area.indent[sep::Y];
        k_loc = k - grid->start()[sep::Z] + area.indent[sep::Z];

        Cell* cell = grid->cells()[sep::FIRST][i_loc][j_loc][k_loc];
        if (cell)
          SetCellConsumption(area, cell);
        else
          cout << "Error: null cell pointer" << endl << endl;
      }
    }
  }
}

void FlowKeeper::SetCellConsumption(FlowAreaData& area, Cell* cell) {

  if (cell->type() != sep::NORMAL) {
    cout << "error: flow cell is null or obtaied" << endl;
    return;
  }

  vector<double>::iterator cii;
  vector<int> coord(4);

  double u_x = 0.0f;
  double n = 0.0f;
  double p;
  double m = cell->MolMass();

  for (cii=cell->speed().begin();
    cii!=cell->speed().end(); ++cii) {

    coord =
      cell->GetSpeedCoord((int)(cii-cell->speed().begin()));
    p = cell->P(sep::X, coord);

    u_x += (*cii) * p / m;

    n += (*cii);
  }
  u_x /= n;
  area.indent_consumption += u_x;
}

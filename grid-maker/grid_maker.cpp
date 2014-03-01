#include "grid_maker.h"

using namespace std;

// Initialize empty grid
void GridMaker::FillInGridWithNulls(int n, int m, int p) {

  for (int i=0; i<n; i++) {

    vector<vector<CellInitData*> > vec_yz;
    cells_.push_back(vec_yz);

    for (int j=0; j<m; j++) {

      vector<CellInitData*> vec_z;
      cells_[i].push_back(vec_z);

      for (int k=0; k<p; k++) {

        cells_[i][j].push_back(new CellInitData(CellInitData::CIDT_NULL));
      }
    }
  }
}


// Build sample grid
void GridMaker::BuildCubeGrid() {

  bool use_z_axis = true;

  // for test let's create rectangle grid n x m
  int n, m, p;
  n = 6;
  m = 8;
  p = 5;

  p = use_z_axis ? p : 1;

  FillInGridWithNulls(n, m, p);

  for (int i=0; i<n; i++) {

    for (int j=0; j<m; j++) {

      for (int k=0; k<p; k++) {

        CellInitData* &cell = cells_[i][j][k];

        if (i == 0 || i == n-1 ||
            j == 0 || j == m-1 ||
            ((k == 0 || k == p-1) && use_z_axis)) {
          // fake cells

          cell = new CellInitData(CellInitData::CIDT_FAKE);
        }
        else {
          // normal cells

          cell = new CellInitData(CellInitData::CIDT_NORMAL);

          if (i == 1 || i == n-2 ||
              j == 1 || j == m-2 ||
              k == 1 || k == p-2) {
            // cells with temperature

            if (i == 1 || i == n-2)
              cell->T_start[sep::X] = 0.8;
            if (j == 1 || j == m-2)
              cell->T_start[sep::Y] = 1.2;
            if (k == 1 || k == p-2)
              cell->T_start[sep::Z] = 0.7;
          }
        }

        cell->coord[sep::X] = i;
        cell->coord[sep::Y] = j;
        cell->coord[sep::Z] = k;
      }
    }
  }
}


// Output file format is supported by grid-solver module
void GridMaker::OutToFile(std::string file_name) {

  FILE* f = fopen(file_name.c_str(), "wb");
  if (!f) {
    cout << "Error: can't open " << file_name << endl;
    return;
  }

  fwrite(new GridDataSplitter(GridDataSplitter::GDST_NEXT_IS_GRID_CONFIG),
      sizeof(GridDataSplitter), 1, f);

  fwrite(grid_config_,
      sizeof(GridConfig), 1, f);

  int n, m, p;

  n = grid_config_->size[sep::X];
  m = grid_config_->size[sep::Y];
  p = grid_config_->size[sep::Z];

  for (int i=0; i<n; i++) {

    for (int j=0; j<m; j++) {

      for (int k=0; k<p; k++) {

        fwrite(new GridDataSplitter(GridDataSplitter::GDST_NEXT_IS_CELL_DATA),
            sizeof(GridDataSplitter), 1, f);

        fwrite(cells_[i][j][k],
            sizeof(CellInitData), 1, f);
      }
    }
  }

  fwrite(new GridDataSplitter(GridDataSplitter::GDST_END_OF_FILE),
      sizeof(GridDataSplitter), 1, f);

  fclose(f);
}


// Add box with fake cells around it
void GridMaker::AddBox(vector<int> start, vector<int> size,
   double T_start, vector<bool> without_fakes, bool flat_z) {

  int n, m, p;

  // add 2D case
  if (flat_z) {
    start[sep::Z] = 0;
    size[sep::Z] = 1;
    without_fakes[sep::Z] = true;
  }

  n = start[sep::X] + size[sep::X];
  m = start[sep::Y] + size[sep::Y];
  p = start[sep::Z] + size[sep::Z];

  for (int i=start[sep::X]; i<n; i++) {

    for (int j=start[sep::Y]; j<m; j++) {

      for (int k=start[sep::Z]; k<p; k++) {

        CellInitData* &cell = cells_[i][j][k];

        // one rule: (do we need this?)
        // if cell is already exist and it's type is normal
        // we do not overwrite it
        if (cell->type == CellInitData::CIDT_NORMAL)
          continue;

        if (i == start[sep::X] || i == n-1 ||
            j == start[sep::Y] || j == m-1 ||
            k == start[sep::Z] || k == p-1) {
          // fake cells

          if (((i == start[sep::X] || i == n-1) && !without_fakes[sep::X]) ||
              ((j == start[sep::Y] || j == m-1) && !without_fakes[sep::Y]) ||
              ((k == start[sep::Z] || k == p-1) && !without_fakes[sep::Z])) {
            // only for some edges

            cell = new CellInitData(CellInitData::CIDT_FAKE);
          }
        }
        else {
          // normal cells

          cell = new CellInitData(CellInitData::CIDT_NORMAL);

          if (i == start[sep::X]+1 || i == n-2 ||
              j == start[sep::Y]+1 || j == m-2 ||
              k == start[sep::Z]+1 || k == p-2) {
            // cells with temperature

            if (i == start[sep::X]+1 || i == n-2)
              cell->T_start[sep::X] = T_start;
            if (j == start[sep::Y]+1 || j == m-2)
              cell->T_start[sep::Y] = T_start;
            if (k == start[sep::Z]+1 || k == p-2)
              cell->T_start[sep::Z] = T_start;
          }
        }

        cell->coord[sep::X] = i;
        cell->coord[sep::Y] = j;
        cell->coord[sep::Z] = k;
      }
    }
  }
}


// Build H type grid
void GridMaker::BuildOurMainGrid(int grid_option, bool flat_z) {

  int n, m, p, D, l, d, h;
  int gaps_q;

  switch (grid_option) {
  case 0:
    n = 35;
    m = 25;
    p = 10;

    D = p;
    l = m - 2*D;
    d = 4;
    h = 3;
    gaps_q = 3;
    break;
  case 1:
    n = 70;
    m = 50;
    p = 20;

    D = p;
    l = m - 2*D;
    d = 4;
    h = 3;
    gaps_q = 5;
    break;
  case 2:
    n = 100;
    m = 70;
    p = 30;

    D = p;
    l = m - 2*D;
    d = 6;
    h = 7;
    gaps_q = 5;
    break;
  }

  // Add 2D case
  if (flat_z) {
    p = 1;
  }

  int x_space = (n - ((gaps_q - 1)*(d + h) + d))/2;

  // Fill in config info

  grid_config_->size[sep::X] = n;
  grid_config_->size[sep::Y] = m;
  grid_config_->size[sep::Z] = p;
  grid_config_->D = D;
  grid_config_->indent = x_space/2;

  FillInGridWithNulls(n, m, p);

  vector<int> start = vector<int>(3);
  vector<int> size = vector<int>(3);
  vector<bool> without_fakes = vector<bool>(3);

  // Add first box

  without_fakes[sep::X] = false;
  without_fakes[sep::Y] = false;
  without_fakes[sep::Z] = false;

  start[sep::X] = 0;
  start[sep::Y] = D + l;
  start[sep::Z] = 0;

  size[sep::X] = n;
  size[sep::Y] = D;
  size[sep::Z] = D;

  AddBox(start, size, 1.0, without_fakes, flat_z);

  // Add second box

  without_fakes[sep::X] = false;
  without_fakes[sep::Y] = false;
  without_fakes[sep::Z] = false;

  start[sep::X] = 0;
  start[sep::Y] = 0;
  start[sep::Z] = 0;

  size[sep::X] = n;
  size[sep::Y] = D;
  size[sep::Z] = D;

  AddBox(start, size, 0.8, without_fakes, flat_z);

  // Add gaps boxes

  without_fakes[sep::X] = false;
  without_fakes[sep::Y] = true;
  without_fakes[sep::Z] = false;

  for (int i=0; i<gaps_q; i++) {

    start[sep::X] = x_space + (d + h) * i;
    start[sep::Y] = D - 2;  // -2 because of fake cells
    start[sep::Z] = D/2 - d/2;

    size[sep::X] = d + 2; // +2 because of fake cells
    size[sep::Y] = l + 4; // +4 because of fake cells
    size[sep::Z] = d + 2; // +2 because of fake cells

    AddBox(start, size, 0.9, without_fakes, flat_z);
  }
}

#include "grid_maker.h"

using namespace std;

void GridMaker::BuildCubeGrid() {

  bool use_z_axis = true;

  // for test let's create rectangle grid n x m
  int n, m, p;
  n = 6;
  m = 8;
  p = 5;

  grid_config_ = new GridConfig();
  grid_config_->size[sep::X] = n;
  grid_config_->size[sep::Y] = m;
  grid_config_->size[sep::Z] = p;

  p = use_z_axis ? p : 1;

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

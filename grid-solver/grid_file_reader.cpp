#include "grid_file_reader.h"

using namespace std;

void GridFileReader::ReadFile(std::string file_name) {

  FILE* f = fopen(file_name.c_str(), "rb");
  if (!f) {
    cout << "Error: can't open " << file_name << endl;
    return;
  }

  GridDataSplitter* grid_data_splitter = new GridDataSplitter();

  fread(grid_data_splitter,
      sizeof(GridDataSplitter), 1, f);

  if (grid_data_splitter->type != GridDataSplitter::GDST_NEXT_IS_GRID_CONFIG) {
    cout << "Error: next data is not grid config data" << endl;
    return;
  }

  grid_config_ = new GridConfig();

  fread(grid_config_,
      sizeof(GridConfig), 1, f);

  int n, m, p;

  n = grid_config_->size[sep::X];
  m = grid_config_->size[sep::Y];
  p = grid_config_->size[sep::Z];

  cout << "Input grid: " << file_name << endl;
  cout << n << " x " << m << " x " << p << endl;

  // create empty grid m x n x p
  for (int i=0; i<n; i++) {

    vector<vector<CellInitData*> > vec_yz;
    cells_.push_back(vec_yz);

    for (int j=0; j<m; j++) {

      vector<CellInitData*> vec_z;
      cells_[i].push_back(vec_z);

      for (int k=0; k<p; k++) {

        cells_[i][j].push_back(new CellInitData(CellInitData::CIDT_NONE));
      }
    }
  }


  for (int i=0; i<n; i++) {

    for (int j=0; j<m; j++) {

      for (int k=0; k<p; k++) {

        fread(grid_data_splitter,
            sizeof(GridDataSplitter), 1, f);

        if (grid_data_splitter->type != GridDataSplitter::GDST_NEXT_IS_CELL_DATA) {
          cout << "Error: next data is not cell data" << endl;
          return;
        }

        cells_[i][j][k] = new CellInitData();

        fread(cells_[i][j][k],
            sizeof(CellInitData), 1, f);
      }
    }
  }

  fread(grid_data_splitter,
      sizeof(GridDataSplitter), 1, f);

  if (grid_data_splitter->type != GridDataSplitter::GDST_END_OF_FILE) {
    cout << "Error: next data is not end of file" << endl;
    return;
  }

  fclose(f);
}

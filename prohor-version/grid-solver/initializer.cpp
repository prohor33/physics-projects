#include "initializer.h"
#include "solver.h"
#include "grid.h"
#include "cell.h"

void Initializer::InitializeSolver() {

  // create just one grid for test
  Grid* grid = new Grid();

  vector<Grid*> grids_z;
  grids_z.push_back(grid);

  vector<vector<Grid*> > grids_yz;
  grids_yz.push_back(grids_z);

  SOLVER->grids_.push_back(grids_yz);

}

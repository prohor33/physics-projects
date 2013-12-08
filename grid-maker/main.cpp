#include "main.h"
#include "grid_maker.h"

int main() {

  GRID_MAKER->BuildCubeGrid();

  GRID_MAKER->OutToFile("cube.grid");

  return 0;
}

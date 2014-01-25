#include "main.h"
#include "grid_maker.h"

int main() {

  //GRID_MAKER->BuildCubeGrid();

  GRID_MAKER->BuildOurMainGrid();

  GRID_MAKER->OutToFile("pretty_big.grid");

  return 0;
}

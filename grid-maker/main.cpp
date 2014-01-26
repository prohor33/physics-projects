#include "main.h"
#include "grid_maker.h"

int main(int argc, char* argv[]) {

  bool flag_output_to_this_dir = false;
  int grid_config = -1;

  for (int i=1; i<argc; i++) {
    string arg = string(argv[i]);
    if (arg == "--help") {
      cout << "This is grid maker program which build grid for grid solver." <<
          endl;
      cout << "Options: " << endl;
      cout << "   -gc  --  Grid configuration option. ";
      cout << "Can be {0, 1, 2}. It sets grid size." << endl;
      cout << "   -td  --  Output to this directory option. ";
      cout << "Without this option output will be sent to ../grid-solver/";
      cout << " directory." << endl;
      return 0;
    } else if (arg == "-gc") {
      if (i >= argc-1) {
        cout << "Error: wrong argument list. ";
        cout << "Please, try --help for more info." << endl;
        return -1;
      }
      i++;
      arg = string(argv[i]);
      grid_config = stoi(arg);
      } else if (arg == "-td") {
      flag_output_to_this_dir = true;
    }
  }

  if (grid_config < 0) {
    cout << "Hm, wrong grid configuration option, " <<
        "we assume it to be 1." << endl;
    cout << "For more information please, try --help." << endl;
    grid_config = 1;
  }

  //GRID_MAKER->BuildCubeGrid();
  GRID_MAKER->BuildOurMainGrid(grid_config);

  string grid_name;

  if (!flag_output_to_this_dir) {
    grid_name += "../grid-solver/";
  }
  grid_name += "income.grid";

  GRID_MAKER->OutToFile(grid_name);

  return 0;
}

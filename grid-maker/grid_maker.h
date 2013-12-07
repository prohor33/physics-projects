#ifndef GRID_MAKER_
#define GRID_MAKER_

#include "main.h"

class GridMaker {

 public:

  static GridMaker* Instance() {
    static GridMaker GridMaker_;
    return &GridMaker_;
  }

  void BuildCubeGrid();

  void OutToFile(std::string file_name);

 private:

  GridMaker() {};
  ~GridMaker() {};
};

#define GRID_MAKER GridMaker::Instance()

#endif // GRID_MAKER_

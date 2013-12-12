#ifndef GRID_MAKER_
#define GRID_MAKER_

#include "main.h"

class GridConfig {

 public:

  GridConfig() {};
  ~GridConfig() {};

  int size[3]; // x, y and z axis

};

class GridDataSplitter {

 public:

  enum GridDataSplitterType {
    GDST_END_OF_FILE,
    GDST_NEXT_IS_CELL_DATA,
    GDST_NEXT_IS_GRID_CONFIG
  };

  GridDataSplitter() {};
  GridDataSplitter(GridDataSplitterType type) :
  type(type) {};

  ~GridDataSplitter() {};

  GridDataSplitterType type;

};

class CellInitData {

 public:

  enum CellInitDataType {
    CIDT_NULL,
    CIDT_NORMAL,
    CIDT_FAKE
  };

  CellInitData() {};
  CellInitData(CellInitDataType type) :
    type(type) {};

  ~CellInitData() {};

  int coord[3];  // for x, y and z axis

  double T_start[3];  // because of corner cells

  CellInitDataType type;
};


class GridMaker {

 public:

  static GridMaker* Instance() {
    static GridMaker GridMaker_;
    return &GridMaker_;
  }

  void FillInGridWithNulls(int n, int m, int p);

  void BuildCubeGrid();

  void BuildOurMainGrid();

  void OutToFile(std::string file_name);

  void AddBox(std::vector<int> start, std::vector<int> size,
      double T_start, std::vector<bool> without_fakes);

 private:

  GridMaker() {};
  ~GridMaker() {};

  std::vector<std::vector<std::vector<CellInitData*> > > cells_;

  GridConfig* grid_config_;
};

#define GRID_MAKER GridMaker::Instance()

#endif // GRID_MAKER_

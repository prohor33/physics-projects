#ifndef GRID_FILE_READER_
#define GRID_FILE_READER_

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
  type() {};

  ~GridDataSplitter() {};

  GridDataSplitterType type;

};

class CellInitData {

 public:

  enum CellInitDataType {
    CIDT_NONE,
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


class GridFileReader {

 public:

  static GridFileReader* Instance() {
    static GridFileReader GridFileReader_;
    return &GridFileReader_;
  }

  void ReadFile(std::string file_name);

  std::vector<std::vector<std::vector<CellInitData*> > > cells() {
    return cells_;
  }

  GridConfig* grid_config() { return  grid_config_; };

 private:

  GridFileReader() {};
  ~GridFileReader() {};

  std::vector<std::vector<std::vector<CellInitData*> > > cells_;

  GridConfig* grid_config_;
};

#define GRID_FILE_READER GridFileReader::Instance()

#endif // GRID_FILE_READER_

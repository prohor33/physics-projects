#ifndef _FLOW_KEEPER_H_
#define _FLOW_KEEPER_H_

#include "main.h"

struct FlowAreaData;
class Cell;

class FlowKeeper {

 public:

  static FlowKeeper* Instance() {
    static FlowKeeper FlowKeeper_;
    return &FlowKeeper_;
  }
  void InitFlowAreas();
  void SetFlow();
  const vector<FlowAreaData*>& areas() {
    return areas_;
  }

 private:

  void SetAreaFlow(FlowAreaData& area);
  void SetCellFlow(FlowAreaData& area,
      Cell* cell, double p_coef);
  void SetAreaConsumption(FlowAreaData& area);
  void SetCellConsumption(FlowAreaData& area, Cell* cell);

  FlowKeeper() {};
  ~FlowKeeper() {};

  vector<FlowAreaData*> areas_;
};


struct FlowAreaData {

  FlowAreaData(const vector<int>& coord_,
      const vector<int>& size_,
      const vector<int>& indent_,
      sep::EdgePos pos_)
    : coord(coord_)
    , size(size_)
    , indent(indent_)
    , consumption(0)
    , pos(pos_) {};
  ~FlowAreaData() {};

  vector<int> coord;
  vector<int> size;
  vector<int> indent;  // indent to check consumption area
  double consumption;
  double indent_consumption;
  sep::EdgePos pos;
};

#define FLOW_KEEPER FlowKeeper::Instance()

#endif  // _FLOW_KEEPER_H_
